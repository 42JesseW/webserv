#include <Config.hpp>

static Config::OptionHttp                   option_http(Config::BASE);
static Config::OptionServer                 option_server(Config::HTTP);
static Config::OptionListen                 option_listen(Config::SERVER);
static Config::OptionClientMaxBodySize      option_client_max_body_size(Config::SERVER);
static Config::OptionErrorPage              option_error_page(Config::SERVER);
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

Config          *Config::handle = NULL;

Config::Config(void) :
    m_file_path(DFL_CONFIG_FILE_PATH),
    m_has_http_set(false)
{
    // TODO set default error_files here
    /*
     * use DFL_ERROR_PAGES_PATH from project directory to find .html files
     * - files are named <status_code>.html
     * - if file not found ignore it
     * - read each file into std::string and update
     * - Server has `m_error_pages` for custom pages
     * - If an error_page is request, Server first checks itself, then Config::getStatusCodeMap
     * - Config has static std::map from getStatusCodeMap for defaults
     * - this Config::Config sets defaults in std::map in Config
     * - while loading custom error_pages into Server.m_error_pages give
     *   warning if unable to find file
     */
    std::stringstream           file_name_ss;
    std::string                 file_name;
    std::stringstream           file_ss;
    std::string                 buffer;
    std::ifstream               error_file;

    status_code_map_t           *code_map;
    status_code_map_t::iterator it;

    code_map = &getStatusCodeMap();
    error_file.exceptions(std::ios::failbit | std::ios::badbit);
    for (it = code_map->begin(); it != code_map->end(); ++it)
    {
        /* will translate to <location_of_executable>/conf/error_pages/<status_code>.html */
        // TODO need a better way to do this ?
        file_name_ss << "../../" << DFL_ERROR_PAGES_PATH << "/" << it->first << ".html";
        file_name = file_name_ss.str();
        file_name_ss.str( std::string() );
        try
        {
            error_file.open(file_name);
            file_ss << error_file.rdbuf();
            code_map->at(it->first) = std::make_pair(it->second.first, file_ss.str());
        }
        catch (const std::ios::failure& e)
        {
            //std::cout << "[WARN] Could not find error_page file: " << file_name << '\n';
        }
        catch (...)
        {
            std::cerr << "[ERROR] Fatal error opening file: " << file_name << '\n';
            std::exit(EXIT_FAILURE);    // TODO necessary ?
        }
        file_ss.str( std::string() );
    }
}

Config::~Config(void)
{
    /* can be done safely since only created once */
    delete Config::handle;
}

Config&                         Config::getHandle()
{
    if (!handle) handle = new Config;
    return (*handle);
}

std::string&                    Config::getFilePath(void)
{
    return (m_file_path);
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

    m_servers.clear();

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
 * the provided tokens read from the file.
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
    // TODO behaviour default_server if same ports or server_names
    // TODO locations with the same base_url
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
            if (parse_level == HTTP && m_servers.empty())
                throw std::invalid_argument("http block requires at least one server");
            if (parse_level == SERVER && current_server->getRoutes().empty())
                throw std::invalid_argument("server block requires at least one route");
            if (parse_level == SERVER && current_server->getSockFd() == SOCK_FD_EMPTY)
                current_server->initListener();
            // TODO validate block
            //  -- conflicts between server blocks (same ports, same server_names)
            //  -- conflicts between location blocks (same base_url)
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
                    //std::cout << tokens.front() << '\n';
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

void                            Config::_mapTokenToObject(tokens_t& tokens,
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
    //std::cout << "[parse level " << option->getParseLevel() << "]\n\n";
}

Config::Option::map_config_t&   Config::_getConfigMap(void)
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

const std::vector<std::string>& Config::getDefaultMethods(void)
{
    static std::vector<std::string> methods;

    if (methods.empty())
    {
        methods.push_back("GET");
        methods.push_back("POST");
        methods.push_back("DELETE");
    }
    return (methods);
}

Config::status_code_map_t&      Config::getStatusCodeMap(void)
{
    static status_code_map_t    status_codes;

    if (status_codes.empty())
    {
        //------ 1xx - Informational ------
        status_codes.insert(std::make_pair(100, std::make_pair("Continue", "")));
        status_codes.insert(std::make_pair(101, std::make_pair("Switching Protocols", "")));
        status_codes.insert(std::make_pair(102, std::make_pair("Processing", "")));
        status_codes.insert(std::make_pair(103, std::make_pair("Early Hints", "")));

        //------ 2xx - Successful ------
        status_codes.insert(std::make_pair(200, std::make_pair("OK", "")));
        status_codes.insert(std::make_pair(201, std::make_pair("Created", "")));
        status_codes.insert(std::make_pair(202, std::make_pair("Accepted", "")));
        status_codes.insert(std::make_pair(203, std::make_pair("Non-Authoritative Information", "")));
        status_codes.insert(std::make_pair(204, std::make_pair("No Content", "")));
        status_codes.insert(std::make_pair(205, std::make_pair("Reset Content", "")));
        status_codes.insert(std::make_pair(206, std::make_pair("Partial Content", "")));
        status_codes.insert(std::make_pair(207, std::make_pair("Multi-Status", "")));
        status_codes.insert(std::make_pair(208, std::make_pair("Already Reported", "")));
        status_codes.insert(std::make_pair(226, std::make_pair("Im Used", "")));

        //------ 3xx - Redirection ------
        status_codes.insert(std::make_pair(300, std::make_pair("Multiple Choices", "")));
        status_codes.insert(std::make_pair(301, std::make_pair("Moved Permanently", "")));
        status_codes.insert(std::make_pair(302, std::make_pair("Found", "")));
        status_codes.insert(std::make_pair(303, std::make_pair("See Other", "")));
        status_codes.insert(std::make_pair(304, std::make_pair("Not Modified", "")));
        status_codes.insert(std::make_pair(305, std::make_pair("Use Proxy", "")));
        status_codes.insert(std::make_pair(307, std::make_pair("Temporary Redirect", "")));
        status_codes.insert(std::make_pair(308, std::make_pair("Permanent Redirect", "")));

        //------ 4xx - Client Error ------
        status_codes.insert(std::make_pair(400, std::make_pair("Bad Request", "")));
        status_codes.insert(std::make_pair(401, std::make_pair("Unauthorized", "")));
        status_codes.insert(std::make_pair(402, std::make_pair("Payment Required", "")));
        status_codes.insert(std::make_pair(403, std::make_pair("Forbidden", "")));
        status_codes.insert(std::make_pair(404, std::make_pair("Not Found", "")));
        status_codes.insert(std::make_pair(405, std::make_pair("Method Not Allowed", "")));
        status_codes.insert(std::make_pair(406, std::make_pair("Not Acceptable", "")));
        status_codes.insert(std::make_pair(407, std::make_pair("Proxy Authentication Required", "")));
        status_codes.insert(std::make_pair(408, std::make_pair("Request Timeout", "")));
        status_codes.insert(std::make_pair(409, std::make_pair("Conflict", "")));
        status_codes.insert(std::make_pair(410, std::make_pair("Gone", "")));
        status_codes.insert(std::make_pair(411, std::make_pair("Length Required", "")));
        status_codes.insert(std::make_pair(412, std::make_pair("Precondition Failed", "")));
        status_codes.insert(std::make_pair(413, std::make_pair("Content Too Large", "")));
        status_codes.insert(std::make_pair(414, std::make_pair("URI Too Long", "")));
        status_codes.insert(std::make_pair(415, std::make_pair("Unsupported Media Type", "")));
        status_codes.insert(std::make_pair(416, std::make_pair("Range Not Satisfiable", "")));
        status_codes.insert(std::make_pair(417, std::make_pair("Expectation Failed", "")));
        status_codes.insert(std::make_pair(418, std::make_pair("I'm a teapot", "")));
        status_codes.insert(std::make_pair(421, std::make_pair("Misdirected Request", "")));
        status_codes.insert(std::make_pair(422, std::make_pair("Unprocessable Content", "")));
        status_codes.insert(std::make_pair(423, std::make_pair("Locked", "")));
        status_codes.insert(std::make_pair(424, std::make_pair("Failed Dependency", "")));
        status_codes.insert(std::make_pair(425, std::make_pair("Too Early", "")));
        status_codes.insert(std::make_pair(426, std::make_pair("Upgrade Required", "")));
        status_codes.insert(std::make_pair(428, std::make_pair("Precondition Required", "")));
        status_codes.insert(std::make_pair(429, std::make_pair("Too Many Requests", "")));
        status_codes.insert(std::make_pair(431, std::make_pair("Request Header Fields Too Large", "")));
        status_codes.insert(std::make_pair(451, std::make_pair("Unavailable For Legal Reasons", "")));

        //------ 5xx - Server Error ------
        status_codes.insert(std::make_pair(500, std::make_pair("Internal Server Error", "")));
        status_codes.insert(std::make_pair(501, std::make_pair("Not Implemented", "")));
        status_codes.insert(std::make_pair(502, std::make_pair("Bad Gateway", "")));
        status_codes.insert(std::make_pair(503, std::make_pair("Service Unavailable", "")));
        status_codes.insert(std::make_pair(504, std::make_pair("Gateway Timeout", "")));
        status_codes.insert(std::make_pair(505, std::make_pair("HTTP Version Not Supported", "")));
        status_codes.insert(std::make_pair(506, std::make_pair("Variant Also Negotiates", "")));
        status_codes.insert(std::make_pair(507, std::make_pair("Insufficient Storage", "")));
        status_codes.insert(std::make_pair(508, std::make_pair("Loop Detected", "")));
        status_codes.insert(std::make_pair(510, std::make_pair("Not Extended", "")));
        status_codes.insert(std::make_pair(511, std::make_pair("Network Authentication Required", "")));
    }
    return (status_codes);
}
