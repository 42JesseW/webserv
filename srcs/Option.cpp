#include <Config.hpp>

/*
 * ( Option )
 */
Config::Option::Option(int parse_level) : m_parse_level(parse_level) { }

Config::Option::Option(const Option &cpy) : m_parse_level(cpy.m_parse_level) { }

Config::Option::~Option() { }

Config::Option&     Config::Option::operator = (const Config::Option &rhs)
{
    if (this != &rhs)
        m_parse_level = rhs.m_parse_level;
    return (*this);
}

int         Config::Option::getParseLevel(void)
{
    return (m_parse_level);
}

/*
 *                      ( OptionHttp)
 * Must be provided at least once for the config file since
 * all options are wrapped inside of it.
 */
Config::OptionHttp::OptionHttp(int parse_level) : Config::Option(parse_level) { }

Config::OptionHttp::OptionHttp(const Config::OptionHttp &cpy) : Config::Option(cpy) { }

Config::OptionHttp::~OptionHttp() { }

Config::OptionHttp&     Config::OptionHttp::operator = (const Config::OptionHttp &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

/* should sign the start of an http block. TODO there can only be one http block in the Config */
void            Config::OptionHttp::parse(void *obj, tokens_t &tokens)
{
    Config  *config;

    config = (Config*)obj;
    if (config->m_has_http_set)
        throw std::invalid_argument("Only one http block is allowed");
    config->m_has_http_set = true;
}

/*
*                  ( OptionClientMaxBodySize )
 * Can change the value of Option::m_client_max_body_size
 */
Config::OptionClientMaxBodySize::OptionClientMaxBodySize(int parse_level) : Config::Option(parse_level) { }

Config::OptionClientMaxBodySize::OptionClientMaxBodySize(const OptionClientMaxBodySize &cpy) : Config::Option(cpy) { }

Config::OptionClientMaxBodySize::~OptionClientMaxBodySize() { }

Config::OptionClientMaxBodySize&    Config::OptionClientMaxBodySize::operator = (const Config::OptionClientMaxBodySize &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void        Config::OptionClientMaxBodySize::parse(void *obj, tokens_t &tokens)
{
    Server *server;
    int     size;

    (void)obj;
    if (tokens.size() < 2)
        throw std::invalid_argument("failed to parse client_max_body_size: Not enough arguments.");
    tokens.pop_front();
    server = (Server *)obj;
    size = ft::stringToInt(tokens.front());
    if (size == FAIL_CONVERSION)
        throw std::invalid_argument("Failed to parse client_max_body_size: Invalid size.");
    server->setClientMaxBodySize(size);
}

Config::OptionErrorPage::OptionErrorPage(int parse_level) : Config::Option(parse_level) { }

Config::OptionErrorPage::OptionErrorPage(const Config::OptionErrorPage &cpy) : Config::Option(cpy) { }

Config::OptionErrorPage::~OptionErrorPage() { }

Config::OptionErrorPage&     Config::OptionErrorPage::operator = (const Config::OptionErrorPage &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

/* expects a Config* object and two arguments */
void            Config::OptionErrorPage::parse(void *obj, tokens_t &tokens)
{
    Server          *server;
    std::string     file_contents;
    std::ifstream   file_stream;
    int             status_code;

    server = (Server *)obj;
    tokens.pop_front();
    if (tokens.size() < 2)
        throw std::invalid_argument("Failed to parse error_page: Not enough arguments.");
    status_code = ft::stringToInt(tokens.front());
    if (status_code == FAIL_CONVERSION)
        throw std::invalid_argument("Failed to parse error_page: Invalid status_code.");
    tokens.pop_front();
    file_contents = ft::readFileContent(file_stream, tokens.front());
    if (!file_stream.good())
        std::cout << "[WARNING] Failed to load file: " << tokens.front() << '\n';
    else
        server->setStatusBody(status_code, file_contents);
}

Config::OptionServer::OptionServer(int parse_level) : Config::Option(parse_level) { }

Config::OptionServer::OptionServer(const Config::OptionServer &cpy) : Config::Option(cpy) { }

Config::OptionServer::~OptionServer() { }

Config::OptionServer&     Config::OptionServer::operator = (const Config::OptionServer &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void            Config::OptionServer::parse(void *obj, tokens_t &tokens)
{
    (void)obj;
    (void)tokens;
}

Config::OptionListen::OptionListen(int parse_level) : Config::Option(parse_level) { }

Config::OptionListen::OptionListen(const Config::OptionListen &cpy) : Config::Option(cpy) { }

Config::OptionListen::~OptionListen() { }

Config::OptionListen&     Config::OptionListen::operator = (const Config::OptionListen &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

/*
 * Sets the address and port for IP
 *
 * DEFAULTS to *:80
 *
 * Valid combinations are:
 * - *:<port>
 * - <address>:<port>
 * - <address>
 * - <port>
 *
 */
void            Config::OptionListen::parse(void *obj, tokens_t &tokens)
{
    Server                  *server;
    std::string             address;
    std::stringstream       full_ip;
    short                   sin_port;

    if (tokens.size() < 2)
        throw std::invalid_argument("Failed to parse listen directive: Not enough arguments");
    tokens.pop_front();
    server = (Server*)obj;
    if (server->getSockFd() != SOCK_FD_EMPTY)
        throw std::invalid_argument("You can only specify `listen` ones");
    try
    {
        _parseArg(tokens.front(), address, &sin_port);
        full_ip << address << ":" << ft::intToString(sin_port);
        server->initListener(full_ip.str());
    }
    catch (const std::invalid_argument& e)
    {
        /* Failed to parse arguments */
    }
    catch (const std::runtime_error& e)
    {
        /* failed to create socket */
    }
}

void            Config::OptionListen::_parseArg(const std::string &arg, std::string& address, short *sin_port)
{
    std::string             arg_cpy;
    std::stringstream       arg_ss;
    std::string::size_type  pos;

    arg_ss << arg;
    arg_cpy = arg_ss.str();
    arg_ss.exceptions( std::ios::failbit | std::ios::badbit );
    switch (ft::count(arg_cpy.begin(), arg_cpy.end(), ':'))
    {
        case 0:
            try
            {
                arg_ss >> *sin_port;
                address += DFL_SERVER_HOST;
            }
            catch (const std::ios::failure& e)
            {
                *sin_port = DFL_SERVER_PORT;
                address += arg_ss.str();
            }
            break ;

        case 1:
            pos = arg_cpy.find(':');
            arg_ss.seekg(pos + 1);
            try
            {
                arg_ss >> *sin_port;
                address += arg_cpy.substr(0, pos);
            }
            catch (const std::ios::failure& e)
            {
                throw std::invalid_argument("Failed to convert port in arg: " + arg_cpy);
            }
            break ;

        default:
            throw std::invalid_argument("Directive " + arg_cpy + " is invalid");
    }
}

Config::OptionServerName::OptionServerName(int parse_level) : Config::Option(parse_level) { }

Config::OptionServerName::OptionServerName(const Config::OptionServerName &cpy) : Config::Option(cpy) { }

Config::OptionServerName::~OptionServerName() { }

Config::OptionServerName&     Config::OptionServerName::operator = (const Config::OptionServerName &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void            Config::OptionServerName::parse(void *obj, tokens_t &tokens)
{
    Server  *server;

    tokens.pop_front();
    server = (Server*)obj;
    server->getNames().clear();
    while (!tokens.empty() && !(tokens.front() == "\n"))
    {
        server->getNames().push_back(tokens.front());
        tokens.pop_front();
    }
    if (tokens.empty())
        throw std::invalid_argument("Invalid config file");
}

Config::OptionLocation::OptionLocation(int parse_level) : Config::Option(parse_level) { }

Config::OptionLocation::OptionLocation(const Config::OptionLocation &cpy) : Config::Option(cpy) { }

Config::OptionLocation::~OptionLocation() { }

Config::OptionLocation&     Config::OptionLocation::operator = (const Config::OptionLocation &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void            Config::OptionLocation::parse(void *obj, tokens_t &tokens)
{
    Route   *route;

    if (tokens.size() < 2)
        throw std::invalid_argument("failed to parse location directive: Not enough arguments");
    tokens.pop_front();
    route = (Route *)obj;
    route->setBaseUrl(tokens.front());
}

Config::OptionAllowedMethods::OptionAllowedMethods(int parse_level) : Config::Option(parse_level) { }

Config::OptionAllowedMethods::OptionAllowedMethods(const Config::OptionAllowedMethods &cpy) : Config::Option(cpy) { }

Config::OptionAllowedMethods::~OptionAllowedMethods() { }

Config::OptionAllowedMethods&     Config::OptionAllowedMethods::operator = (const Config::OptionAllowedMethods &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void            Config::OptionAllowedMethods::parse(void *obj, tokens_t &tokens)
{
    Route                           *route;
    int                             count;
    const ConfigUtil::methods_t     *methods;

    count = 0;
    tokens.pop_front();
    route = (Route *)obj;
    route->getAcceptedMethods().clear();
    methods = &ConfigUtil::getHandle().getDefaultMethods();
    while (!tokens.empty() && !(tokens.front() == "\n" || count >= 3)) // TODO testcase
    {
        if (std::find(methods->begin(), methods->end(), tokens.front()) == methods->end())
            throw std::invalid_argument("Failed to parse allowed_methods");
        route->getAcceptedMethods().push_back(tokens.front());
        tokens.pop_front();
        count++;
    }
    if (tokens.empty())
        throw std::invalid_argument("Invalid config file");
}

Config::OptionRoot::OptionRoot(int parse_level) : Config::Option(parse_level) { }

Config::OptionRoot::OptionRoot(const Config::OptionRoot &cpy) : Config::Option(cpy) { }

Config::OptionRoot::~OptionRoot() { }

Config::OptionRoot&     Config::OptionRoot::operator = (const Config::OptionRoot &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

/* should sign the start of an http block. TODO there can only be one http block in the Config */
void            Config::OptionRoot::parse(void *obj, tokens_t &tokens)
{
    Route   *route;

    tokens.pop_front();
    route = (Route *)obj;
    route->setFileSearchPath(tokens.front());
}

Config::OptionAutoIndex::OptionAutoIndex(int parse_level) : Config::Option(parse_level) { }

Config::OptionAutoIndex::OptionAutoIndex(const Config::OptionAutoIndex &cpy) : Config::Option(cpy) { }

Config::OptionAutoIndex::~OptionAutoIndex() { }

Config::OptionAutoIndex&     Config::OptionAutoIndex::operator = (const Config::OptionAutoIndex &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

/* should sign the start of an http block. TODO there can only be one http block in the Config */
void            Config::OptionAutoIndex::parse(void *obj, tokens_t &tokens)
{
    Route   *route;

    tokens.pop_front();
    route = (Route *)obj;
    if (!(tokens.front() == "on" || tokens.front() == "off"))
        throw std::invalid_argument("Failed to parse autoindex directive: Invalid argument.");
    route->setAutoIndex((tokens.front() == "on") ? true : false);
}

Config::OptionIndex::OptionIndex(int parse_level) : Config::Option(parse_level) { }

Config::OptionIndex::OptionIndex(const Config::OptionIndex &cpy) : Config::Option(cpy) { }

Config::OptionIndex::~OptionIndex() { }

Config::OptionIndex&     Config::OptionIndex::operator = (const Config::OptionIndex &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void            Config::OptionIndex::parse(void *obj, tokens_t &tokens)
{
    Route   *route;

    tokens.pop_front();
    route = (Route *)obj;
    route->getIndexFiles().clear();
    while (!tokens.empty() && tokens.front() != "\n")
    {
        route->getIndexFiles().push_back(tokens.front());
        tokens.pop_front();
    }
    if (tokens.empty())
        throw std::invalid_argument("Invalid config file");
}

Config::OptionCgiExtension::OptionCgiExtension(int parse_level) : Config::Option(parse_level) { }

Config::OptionCgiExtension::OptionCgiExtension(const Config::OptionCgiExtension &cpy) : Config::Option(cpy) { }

Config::OptionCgiExtension::~OptionCgiExtension() { }

Config::OptionCgiExtension&     Config::OptionCgiExtension::operator = (const Config::OptionCgiExtension &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void            Config::OptionCgiExtension::parse(void *obj, tokens_t &tokens)
{
    Route   *route;

    tokens.pop_front();
    route = (Route *)obj;
    while (!tokens.empty() && tokens.front() != "\n")
    {
        route->getCgiFileExtensions().push_back(tokens.front());
        tokens.pop_front();
    }
    if (tokens.empty())
        throw std::invalid_argument("Invalid config file");
}

Config::OptionUploadPath::OptionUploadPath(int parse_level) : Config::Option(parse_level) { }

Config::OptionUploadPath::OptionUploadPath(const Config::OptionUploadPath &cpy) : Config::Option(cpy) { }

Config::OptionUploadPath::~OptionUploadPath() { }

Config::OptionUploadPath&     Config::OptionUploadPath::operator = (const Config::OptionUploadPath &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void            Config::OptionUploadPath::parse(void *obj, tokens_t &tokens)
{
    Route   *route;

    tokens.pop_front();
    route = (Route *)obj;
    route->setUploadPath(tokens.front());
}

Config::OptionReturn::OptionReturn(int parse_level) : Config::Option(parse_level) { }

Config::OptionReturn::OptionReturn(const Config::OptionReturn &cpy) : Config::Option(cpy) { }

Config::OptionReturn::~OptionReturn() { }

Config::OptionReturn&     Config::OptionReturn::operator = (const Config::OptionReturn &rhs)
{
    Config::Option::operator = (rhs);
    return (*this);
}

void            Config::OptionReturn::parse(void *obj, tokens_t &tokens)
{
    Route           *route;
    int             status_code;

    tokens.pop_front();
    route = (Route *)obj;
    if (tokens.size() < 2)
        throw std::invalid_argument("Failed to parse return: Not enough arguments.");
    status_code = ft::stringToInt(tokens.front());
    if (status_code == FAIL_CONVERSION)
        throw std::invalid_argument("Failed to parse return: Invalid status_code.");
    tokens.pop_front();
    route->setRedirect(status_code, tokens.front());
}
