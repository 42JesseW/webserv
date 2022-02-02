#include "../includes/Config.hpp"

int Config::m_client_max_body_size = DFL_MAX_BODY_SIZE;

Config::Config(void)
{

}

Config::~Config(void)
{

}

Config&                         Config::getHandle()
{
    static Config   singletonInstance;
    return (singletonInstance);
}

std::string&                    Config::getFilePath(void)
{
    return (m_file_path);
}

std::map<int, std::string>&     Config::getErrorFiles(void)
{
    return (m_error_files);
}

std::vector<Server>&            Config::getServers(void)
{
    return (m_servers);
}

void                            Config::setFilePath(const std::string& file_path)
{
    m_file_path = file_path;
}

void                            Config::loadFile(void)
{
    std::ifstream           file_handle;
    std::stringstream       buffer;
    std::string             config_data;
    std::string             current_buffer;
    std::stack<char>        brackets;
    config_map_t            config_map;
    short                   parse_level;

    if (m_file_path.empty())
        throw std::logic_error("File path is not yet set");

    /* set this ifstream instance to throw if opening file fails */
    file_handle.exceptions( std::ifstream::badbit | std::ifstream::failbit );
    file_handle.open(m_file_path);

    /* parse file structure line by line */

    /* PARSING RULES */
    /* comments start with #. Everything after # is ignored */
    /* empty lines are ignored */
    /* brackets {} must be used to define blocks */

    /* read whole file into stringstream buffer and copy to string object */
    buffer << file_handle.rdbuf();
    config_data = buffer.str();

    parse_level = NONE;
    config_map = _getConfigOptions();
    for (std::string::iterator it = config_data.begin(); it != config_data.end(); ++it)
    {
        if (*it == '{')
        {
            brackets.push(*it);
            parse_level++;
        }
        else if (*it == '}')
        {
            if (brackets.empty() || brackets.top() != '{')
                throw std::logic_error("Invalid config file");
            brackets.pop();
            parse_level--;
        }
        else
        {

        }
    }
    if (parse_level != NONE || !brackets.empty())
        throw std::logic_error("Invalid config file");

    file_handle.close();
}

std::map<Config::t_levels, std::vector<std::string> >       Config::_getConfigOptions()
{
    static std::map<t_levels, std::vector<std::string> >    config_map;
    std::vector<std::string>                                base_options;
    std::vector<std::string>                                http_options;
    std::vector<std::string>                                server_options;
    std::vector<std::string>                                location_options;

    if (config_map.empty())
    {
        base_options.push_back("http");
        config_map[BASE] = base_options;
        http_options.push_back("client_max_body_size");
        http_options.push_back("error_page");
        http_options.push_back("server");
        config_map[HTTP] = http_options;
        server_options.push_back("listen");
        server_options.push_back("server_name");
        server_options.push_back("location");
        config_map[SERVER] = server_options;
        location_options.push_back("allowed_methods");
        location_options.push_back("root");
        location_options.push_back("autoindex");
        location_options.push_back("index");
        location_options.push_back("cgi_extensions");
        location_options.push_back("upload_path");
        location_options.push_back("return");
        config_map[LOCATION] = location_options;
    }
    return (config_map);
}
