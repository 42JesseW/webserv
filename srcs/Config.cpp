#include <Config.hpp>

static Config::OptionHttp                   option_http(Config::BASE);
static Config::OptionClientMaxBodySize      option_client_max_body_size(Config::HTTP);
static Config::OptionErrorPage              option_error_page(Config::HTTP);
static Config::OptionServer                 option_server(Config::HTTP);
static Config::OptionListen                 option_listen(Config::SERVER);
static Config::OptionServerName             option_server_name(Config::SERVER);
static Config::OptionLocation               option_location(Config::SERVER);
static Config::OptionAllowedMethods         option_allowed_methods(Config::LOCATION);
static Config::OptionRoot                   option_root(Config::LOCATION);
static Config::OptionAutoIndex              option_autoindex(Config::LOCATION);
static Config::OptionIndex                  option_index(Config::LOCATION);
static Config::OptionCgiExtension           option_cgi_extension(Config::LOCATION);
static Config::OptionUploadPath             option_upload_path(Config::LOCATION);
static Config::OptionReturn                 option_return(Config::LOCATION);

Config::Option  *Config::m_option_http = &option_http;
Config::Option  *Config::m_option_client_max_body_size = &option_client_max_body_size;
Config::Option  *Config::m_option_error_page = &option_error_page;
Config::Option  *Config::m_option_server = &option_server;
Config::Option  *Config::m_option_listen = &option_listen;
Config::Option  *Config::m_option_server_name = &option_server_name;
Config::Option  *Config::m_option_location = &option_location;
Config::Option  *Config::m_option_allowed_methods = &option_allowed_methods;
Config::Option  *Config::m_option_root = &option_root;
Config::Option  *Config::m_option_autoindex = &option_autoindex;
Config::Option  *Config::m_option_index = &option_index;
Config::Option  *Config::m_option_cgi_extension = &option_cgi_extension;
Config::Option  *Config::m_option_upload_path = &option_upload_path;
Config::Option  *Config::m_option_return = &option_return;

int             Config::m_client_max_body_size = DFL_MAX_BODY_SIZE;

Config::Config(void)
{
    // TODO set default error_files here
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
    file_handle.open(m_file_path.c_str());

    /* read whole file into stringstream buffer and copy to string object */
    buffer << file_handle.rdbuf();
    config_data = buffer.str();
    file_handle.close();

    /* tokenize the string */
    std::replace(config_data.begin(), config_data.end(), '\t', ' ');
    ft::stringReplaceMultiple(config_data, "\n", " \n ");
    ft::tokenizeString(tokens, config_data, " ");

    /* initialize the singleton instance */
    _mapTokens(tokens);
}

/*
 * Gradually add components to the Config class using
 * using the provided tokens read from the file.
 *
 * Will throw std::invalid_argument in the following cases:
 * - Invalid brackets (i.e. not closing or invalid order)
 * - Unknown keyword or keyword in wrong parse level
 * - Could not convert string if numeric argument expected
 */
void                            Config::_mapTokens(tokens_t& tokens)
{
    int                             parse_level;
    Option                          *option;
    std::stack<char>                brackets;
    std::stringstream               err_ss;
    Option::map_config_t            config;
    Option::map_config_t::iterator  it;
    Server                          *current_server;
    Route                           *current_route;

    parse_level = BASE;
    config = _getConfigMap();
    while (!tokens.empty())
    {
        if (tokens.front() == "#")
        {
            while (!tokens.empty() && tokens.front() != "\n")
                tokens.pop_front();
        }
        else if (tokens.front() == "{")
        {
            brackets.push(tokens.front().at(0));
            parse_level++;
        }
        else if (tokens.front() == "}")
        {
            if (brackets.empty() || brackets.top() != '{')
                throw std::invalid_argument("Invalid brackets in config file");
            // TODO check if Server | Route object is valid
            // Server needs a valid socket -> m_sock.sock_fd must not be SOCK_FD_EMPTY
            // Server needs at least one Route -> m_routes > 0
            brackets.pop();
            parse_level--;
        }
        else
        {
            if (!(tokens.front() == "\n"))
            {
                it = config.find(tokens.front());
                if (it == config.end())
                {
                    err_ss << "Invalid keyword '" << tokens.front() << "'";
                    throw std::invalid_argument(err_ss.str());
                }
                else
                {
                    std::cout << tokens.front() << '\n';
                    option = (*it).second;
                    if (parse_level != option->getParseLevel())
                    {
                        err_ss << "Option '" << tokens.front() << "' at wrong parse level";
                        throw std::invalid_argument(err_ss.str());
                    }
                    _mapTokenToObject(tokens, option, &current_server, &current_route, parse_level);
                }
            }
        }
        tokens.pop_front();
    }
    if (!brackets.empty())
        throw std::invalid_argument("Invalid brackets in config file");
}

void                                    Config::_mapTokenToObject(tokens_t& tokens,
                                                                  Option *option,
                                                                  Server **server,
                                                                  Route **route,
                                                                  int parse_level)
{
    switch (parse_level)
    {
        case (BASE):
        case (HTTP):
            if (tokens.front() == "server")
            {
                m_servers.push_back(Server());
                *server = &m_servers.at(m_servers.size() - 1);
            }
            option->parse(this, tokens);
            break ;

        case (SERVER):
            if (tokens.front() == "location")
            {
                (*server)->getRoutes().push_back(Route());
                *route = &(*server)->getRoutes().at((*server)->getRoutes().size() - 1);
                option->parse(*route, tokens);
            }
            else
                option->parse(*server, tokens);
            break ;

        case (LOCATION):
            option->parse(*route, tokens);
            break ;

        default:
            break ;
    }
    std::cout << "[parse level " << option->getParseLevel() << "]\n\n";
}

Config::Option::map_config_t&           Config::_getConfigMap(void)
{
    static Option::map_config_t   config_map;

    if (config_map.empty())
    {
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
    }
    return (config_map);
}
