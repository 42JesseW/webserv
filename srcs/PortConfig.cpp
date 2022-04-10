#include <Config.hpp>

PortConfig::PortConfig(const std::string& host, uint16_t& port) :
    m_sock(new ServerSocket()),
    m_host(host),
    m_port(port)
{

}

PortConfig::PortConfig(const PortConfig &cpy)
{
    *this = cpy;
}

PortConfig::~PortConfig(void)
{
    clients_t::iterator client_it;

    delete m_sock;
    for (server_t::size_type idx = 0 ; idx < m_server_blocks.size() ; ++idx)
        delete m_server_blocks[idx];
}

PortConfig&                 PortConfig::operator = (const PortConfig &rhs)
{
    if (this != &rhs)
    {
        delete m_sock;
        m_sock = NULL;
        if (rhs.m_sock)
            m_sock = new ServerSocket(*rhs.m_sock);
    }
    return (*this);
}

void                        PortConfig::initSocket(void)
{
    m_sock->init(m_host, m_port);
}

ServerSocket                *PortConfig::getSocket(void)
{
    return (m_sock);
}

uint16_t                    PortConfig::getPort(void)
{
    return (m_port);
}

PortConfig::server_t&    PortConfig::getServers(void)
{
    return (m_server_blocks);
}

Route       *PortConfig::getMatchingRoute(Request& request, ConfigUtil::status_code_map_t **error_files)
{
    ServerConfig                        *server_config;
    Route                               *route;
    ServerConfig::routes_t::iterator    it;

    server_config = _getMatchingServerBlock(request, error_files);
    if (request.getStatus() == HTTP_STATUS_OK)
    {
        it = server_config->getRoutes().begin();
        for ( ; it != server_config->getRoutes().end() ; it++)
        {
            route = (*it);
            if (route->getBaseUrl() == request.getTarget())
            {
                return (route);
            }
        }
        request.setStatus(HTTP_STATUS_NOT_FOUND);
    }
    return (NULL);
}

/*
 * If multiple server blocks then match on the one that has
 * a matching `Host` -> `server_name`. If no matching block
 * found, then use the default (0).
 */
ServerConfig    *PortConfig::_getMatchingServerBlock(Request& request, ConfigUtil::status_code_map_t **error_files)
{
    ServerConfig                        *server;
    std::string                         host_header_value;
    server_t::iterator                  server_it;
    std::vector<std::string>::iterator  name_it;
    Request::headers_t::iterator       header_it;

    server_it = m_server_blocks.begin();
    if (m_server_blocks.size() > 1)
    {
        if ((header_it = request.getHeaders().find("Host")) != request.getHeaders().end())
        {
            host_header_value = header_it->second;
            for ( ; server_it != m_server_blocks.end() ; ++server_it )
            {
                server = (*server_it);
                name_it = std::find(server->getNames().begin(), server->getNames().end(), host_header_value);
                if (name_it != server->getNames().end())
                {
                    *error_files = &server->getErrorFiles();
                    return (server);
                }
            }
        }
    }
    *error_files = &m_server_blocks.at(0)->getErrorFiles();
    return (m_server_blocks.at(0));
}
