#include "../includes/Config.hpp"

int             Config::m_client_max_body_size = DFL_MAX_BODY_SIZE;

static ConfigOption option_http(true, 0, Config::BASE, NULL);
static ConfigOption option_client_max_body_size(false, 1, Config::HTTP, NULL);
static ConfigOption option_error_page(false, 2, Config::HTTP, NULL);
static ConfigOption option_server(true, 0, Config::HTTP, NULL);
static ConfigOption option_listen(false, 1, Config::SERVER, NULL);
static ConfigOption option_server_name(false, OPTION_ARG_INF, Config::SERVER, NULL);
static ConfigOption option_location(true, 1, Config::SERVER, NULL);
static ConfigOption option_allowed_methods(false, OPTION_ARG_INF, Config::LOCATION, NULL);
static ConfigOption option_root(false, 1, Config::LOCATION, NULL);
static ConfigOption option_autoindex(false, 1, Config::LOCATION, NULL);
static ConfigOption option_index(false, OPTION_ARG_INF, Config::LOCATION, NULL);
static ConfigOption option_cgi_extension(false, OPTION_ARG_INF, Config::LOCATION, NULL);
static ConfigOption option_upload_path(false, 1, Config::LOCATION, NULL);
static ConfigOption option_return(false, 2, Config::LOCATION, NULL);

ConfigOption    *Config::m_option_http = &option_http;
ConfigOption    *Config::m_option_client_max_body_size = &option_client_max_body_size;
ConfigOption    *Config::m_option_error_page = &option_error_page;
ConfigOption    *Config::m_option_server = &option_server;
ConfigOption    *Config::m_option_listen = &option_listen;
ConfigOption    *Config::m_option_server_name = &option_server_name;
ConfigOption    *Config::m_option_location = &option_location;
ConfigOption    *Config::m_option_allowed_methods = &option_allowed_methods;
ConfigOption    *Config::m_option_root = &option_root;
ConfigOption    *Config::m_option_autoindex = &option_autoindex;
ConfigOption    *Config::m_option_index = &option_index;
ConfigOption    *Config::m_option_cgi_extension = &option_cgi_extension;
ConfigOption    *Config::m_option_upload_path = &option_upload_path;
ConfigOption    *Config::m_option_return = &option_return;

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

    // TODO clean this up a bit (what should be a class method and what a util method in ft::)
    /* strip the config file data for easier tokenization */
    ft::stringReplaceMultiple(config_data, "\n", " \n ");
    _stripFileData(config_data);
    _tokenizeFileData(tokens, config_data);

    /* modifies the Config instance using the file data split into tokens */
    _loadFile(tokens);
}

ConfigOption::map_config_t&   Config::_getConfigMap(void)
{
    static ConfigOption::map_config_t   config_map;

    config_map["http"] = m_option_http;
    config_map["client_max_body_size"] = m_option_client_max_body_size;
    config_map["error_page"] = m_option_error_page;
    config_map["server"] = m_option_server;
    config_map["listen"] = m_option_listen;
    config_map["server_name"] = m_option_server_name;
    config_map["location"] = m_option_location;
    config_map["allowed_methods"] = m_option_allowed_methods;
    config_map["root"] = m_option_root;
    config_map["autoindex"] = m_option_autoindex;
    config_map["index"] = m_option_index;
    config_map["cgi_extensions"] = m_option_cgi_extension;
    config_map["upload_path"] = m_option_upload_path;
    config_map["return"] = m_option_return;
    return (config_map);
}

/*
** 1. replace all '\t' characters with ' '
** 2. reduce all consecutive spaces to one space
** 3. reduce all consecutive newlines to one newline
*/
std::string&                Config::_stripFileData(std::string &buffer)         // TODO write testcase
{
    std::string::iterator   new_end;

    std::replace(buffer.begin(), buffer.end(), '\t', ' ');
    new_end = ft::uniqueChars(buffer.begin(), buffer.end(), ' ');
    buffer.erase(new_end, buffer.end());
    new_end = ft::uniqueChars(buffer.begin(), buffer.end(), '\n');
    buffer.erase(new_end, buffer.end());
    return (buffer);
}

std::deque<std::string>&    Config::_tokenizeFileData(std::deque<std::string>& tokens, std::string &data) // TODO write testcase
{
    std::string::size_type  pos;
    std::string             delim = " ";        // TODO pass via argument ??

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

namespace ft {

    bool bothAreChar(char lhs, char rhs, int c)
    {
        return ((lhs == rhs) && (lhs == c));
    }

    std::string&    stringReplaceMultiple(std::string& str, const std::string& from, const std::string& to)
    {
        std::string::size_type  index;

        index = 0;
        while (true) {
            index = str.find(from, index);
            if (index == std::string::npos)
                break;
            str.replace(index, from.size(), to);
            index += 3;
        }
        return (str);
    }

}
