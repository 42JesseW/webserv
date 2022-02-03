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
    std::deque<std::string> tokens;

    if (m_file_path.empty())
        throw std::logic_error("File path is not yet set");

    /* set this ifstream instance to throw if opening file fails */
    file_handle.exceptions( std::ifstream::badbit | std::ifstream::failbit );
    file_handle.open(m_file_path);

    /* read whole file into stringstream buffer and copy to string object */
    buffer << file_handle.rdbuf();
    config_data = buffer.str();
    file_handle.close();

    /* strip the config file data for easier tokenization */
    _stripFileData(config_data);
    _tokenizeFileData(tokens, config_data);

    /* modifies the Config instance using the file data split into tokens */
    _loadFile(tokens);
}

ConfigOption::map_config_t&   Config::_getConfigMap(void)
{
    static ConfigOption::map_config_t   config_map;

    config_map["http"] = &ft::option_http;
    config_map["client_max_body_size"] = &ft::option_client_max_body_size;
    config_map["error_page"] = &ft::option_error_page;
    config_map["server"] = &ft::option_server;
    config_map["listen"] = &ft::option_listen;
    config_map["server_name"] = &ft::option_server_name;
    config_map["location"] = &ft::option_location;
    config_map["allowed_methods"] = &ft::option_allowed_methods;
    config_map["root"] = &ft::option_root;
    config_map["autoindex"] = &ft::option_autoindex;
    config_map["index"] = &ft::option_index;
    config_map["cgi_extensions"] = &ft::option_cgi_extension;
    config_map["upload_path"] = &ft::option_upload_path;
    config_map["return"] = &ft::option_return;
    return (config_map);
}

/*
** 1. replace both '\n' and '\t' characters with ' '
** 2. reduce all consecutive spaces to one space
*/
std::string&                Config::_stripFileData(std::string &buffer)
{
    std::string::iterator   new_end;

    std::replace(buffer.begin(), buffer.end(), '\n', ' ');
    std::replace(buffer.begin(), buffer.end(), '\t', ' ');
    new_end = ft::uniqueSpaces(buffer.begin(), buffer.end());
    buffer.erase(new_end, buffer.end());
    return (buffer);
}

std::deque<std::string>&    Config::_tokenizeFileData(std::deque<std::string>& tokens, std::string &data)
{
    std::string::size_type  pos;
    std::string             delim = " ";

    while ( ( pos = data.find( delim ) ) != std::string::npos )
    {
        tokens.push_back(data.substr(0, pos));
        data.erase(0, pos + delim.length());
    }
    return (tokens);
}

void                        Config::_loadFile(std::deque<std::string> &tokens)
{
    short                                   parse_level;
    std::stack<char>                        brackets;
    ConfigOption                            *option;
    ConfigOption::map_config_t::iterator    it;
    ConfigOption::map_config_t              config;

    config = _getConfigMap();
    while (!tokens.empty())
    {
        std::cout << "TOKWORD: " << tokens.front() << "\n\n";
        it = config.find(tokens.front());
        if (it == config.end())
        {
            //std::cerr << tok_words.front() << " is not a valid option" << '\n';
        }
        else
        {
            std::cout << "Option found: " << tokens.front() << '\n';
            option = (*it).second;
            std::cout << "arg amount " << option->m_arg_amount << '\n';
            std::cout << "is nested " << option->m_is_nested << '\n';
            std::cout << "parse level " << option->m_parse_level << "\n\n";
        }
        tokens.pop_front();
    }
}

ConfigOption::ConfigOption(bool is_nested,
                           int arg_amount,
                           int parse_level,
                           int (*parse_func)(void *, int)) :
        m_is_nested(is_nested),
        m_arg_amount(arg_amount),
        m_parse_level(parse_level),
        m_parse_func(parse_func)
{

}

ConfigOption::ConfigOption(const ConfigOption& cpy) :
        m_is_nested(cpy.m_is_nested),
        m_arg_amount(cpy.m_arg_amount),
        m_parse_level(cpy.m_parse_level),
        m_parse_func(cpy.m_parse_func)
{

}

ConfigOption::~ConfigOption()
{

}
