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

Config          *Config::m_handle = NULL;

Config::Config(void) :
    m_file_path(DFL_CONFIG_FILE_PATH),
    m_has_http_set(false)
{
    /*
     * When using vector.push_back on m_servers in C++98
     * the copy constructor is used to pre-allocate new objects.
     * In C++11 a move-constructor can be defined for this.
     *
     * Problem is that the copy constructor will also call the
     * Socket(const Socket& cpy) constructor which will create
     * copies of existing Socket object. If it pre-allocates more
     * then one then the last element is used as default which
     * creates duplicate sockets and breaks logic during parsing.
     *
     * https://stackoverflow.com/questions/26740523/vector-push-back-calling-copy-constructor-more-than-once
     */
    m_servers.reserve(MAX_SERVER_CAP);
    // TODO scan the config file for `server` keyword and reserve based on this.
}

Config::~Config(void)
{
    /* can be done safely since only created once */
    delete Config::m_handle;
}

Config&                         Config::getHandle()
{
    if (!m_handle) m_handle = new Config;
    return (*m_handle);
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
    m_has_http_set = false;

    /* set this ifstream instance to throw if opening file fails */
    file_handle.exceptions( std::ifstream::badbit | std::ifstream::failbit );
    file_handle.open(m_file_path.c_str());

    /* read whole file into stringstream buffer and copy to string object */
    buffer << file_handle.rdbuf();
    config_data = buffer.str();
    file_handle.close();

    /* tokenize the string */
    std::replace(config_data.begin(), config_data.end(), '\t', ' ');
    _stringReplaceMultiple(config_data, "\n", " \n ");
    _tokenizeString(tokens, config_data, " ");

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
                    switch (parse_level)
                    {
                        case (BASE):
                        case (HTTP):
                            if (tokens.front() == "server")
                            {
                                m_servers.push_back(Server());
                                current_server = &m_servers.at(m_servers.size() - 1);
                            }
                            option->parse(this, tokens);
                            break ;

                        case (SERVER):
                            if (tokens.front() == "location")
                            {
                                current_server->getRoutes().push_back(Route());
                                current_route = &current_server->getRoutes().at(current_server->getRoutes().size() - 1);
                                option->parse(current_route, tokens);
                            }
                            else
                                option->parse(current_server, tokens);
                            break ;

                        case (LOCATION):
                            option->parse(current_route, tokens);
                            break ;

                        default:
                            break ;
                    }
                    //std::cout << "[parse level " << option->getParseLevel() << "]\n\n";
                }
            }
        }
        tokens.pop_front();
    }
    if (!brackets.empty())
        throw std::invalid_argument("Invalid brackets in config file");
}

/*
 * // TODO testcase
 * Split the string based on the delimiter(s) and push the
 * individual tokens into the specified container.
 */
Config::tokens_t&    Config::_tokenizeString(Config::tokens_t& queue,
                                             std::string& str,
                                             const std::string& delims)
{
    std::string             sub;
    std::string::size_type  pos, prev;

    prev = 0;
    while ((pos = str.find_first_of(delims, prev)) != std::string::npos)
    {
        if (pos > prev)
        {
            sub = str.substr(prev, pos - prev);
            if (queue.empty() || !(sub == "\n" && queue.back() == "\n"))
                queue.push_back(str.substr(prev, (pos - prev)));
        }
        prev = (pos + 1);
    }
    if (prev < str.length())
        queue.push_back(str.substr(prev, std::string::npos));
    return (queue);
}

/*
 * // TODO testcase
 * Replace a substring in a string with another substring using
 * built-in string methods find() and replace().
 */
std::string&    Config::_stringReplaceMultiple(std::string& str,
                                               const std::string& from,
                                               const std::string& to)
{
    std::string::size_type  index;

    index = 0;
    while (true) {
        index = str.find(from, index);
        if (index == std::string::npos)
            break;
        str.replace(index, from.size(), to);
        index += to.size();
    }
    return (str);
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
