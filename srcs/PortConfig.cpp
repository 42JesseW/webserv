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

}

PortConfig&                 PortConfig::operator = (const PortConfig &rhs)
{
    if (this != &rhs)
    {
        delete m_sock;
        m_sock = NULL;
        if (rhs.m_sock)
            m_sock = new ServerSocket(*rhs.m_sock);
        m_clients = rhs.m_clients;  // TODO deep copy of Connection*
        m_pfds = rhs.m_pfds;
    }
    return (*this);
}

void                        PortConfig::initSocket(void)
{
    struct pollfd   listener;

    m_sock->init(m_host, m_port);
    listener.fd = m_sock->getFd();
    listener.events = POLLIN;
    listener.revents = 0;
    addPollFd(listener);
}

ServerSocket                *PortConfig::getSocket(void)
{
    return (m_sock);
}

void                        PortConfig::addClient(int &fd, Connection *conn)
{
    struct pollfd   client;

    m_clients.insert(std::make_pair(fd, conn));
    client.fd = fd;
    client.events = (POLLIN | POLLOUT);
    client.revents = 0;
    addPollFd(client);
}

void                        PortConfig::addPollFd(struct pollfd &pfd)
{
    m_pfds.push_back(pfd);
}

PortConfig::pollfd_t&   PortConfig::getPollFds(void)
{
    return (m_pfds);
}

uint16_t                    PortConfig::getPort(void)
{
    return (m_port);
}

PortConfig::server_t&    PortConfig::getServers(void)
{
    return (m_server_blocks);
}

PortConfig::clients_t&  PortConfig::getClients(void)
{
    return (m_clients);
}

void                        *PortConfig::pollPort(void *port_config)
{
    ConfigUtil      *util;
    PortConfig      *port;
    int             fds_with_events;
    size_t          active_connections;

    int             client_fd;
    SA_IN           client_addr;
    ClientSocket    *client_socket;
    Connection      *client_connection;

    clients_t::iterator     client_it;
    pollfd_t::iterator      pfd_it;

    util = &ConfigUtil::getHandle();
    port = (PortConfig*)port_config;
    port->initSocket();
    for ( ; ; )
    {
        active_connections = 1 + port->getClients().size();
        fds_with_events = poll(&port->getPollFds().at(0), active_connections, POLL_TIMEOUT_MS);
        if (fds_with_events == SYS_ERROR)
        {
            /* some error handling */

        }
        if (util->isSignalled())
        {
            /* webserver has been signalled */
            break ;
        }
        if (!fds_with_events)
            continue ;

        /* look for fds with events */
        pfd_it = port->getPollFds().begin();
        for (size_t idx = 0; idx < active_connections ; ++idx, ++pfd_it )
        {
            if (!pfd_it->revents)
                continue ;
            /* TODO check for errors first */

            if (pfd_it->revents & (POLLIN))
            {
                /* if POLLIN on listener socket */
                if (pfd_it->fd == port->getSocket()->getFd())
                {
                    client_fd = port->getSocket()->accept(client_addr);
                    client_socket       = new ClientSocket(client_fd, client_addr);
                    client_connection   = new Connection(client_socket);
                    port->addClient(client_fd, client_connection);
                }
                else
                {
                    client_it           = port->getClients().find(pfd_it->fd);
                    client_connection   = client_it->second;
                    client_connection->readSocket();
                }
                usleep(POLLIN_SLEEP_MS);
            }
            else
            {
                if (pfd_it->revents & (POLLOUT))
                {
                    ConfigUtil::status_code_map_t   *error_files;
                    Route                           *route;

                    /* POLLOUT on client socket */
                    client_it           = port->getClients().find(pfd_it->fd);
                    client_connection   = client_it->second;
                    client_connection->parseRequest();

                    route               = port->getMatchingRoute(client_connection->getRequest(), &error_files);
                    client_connection->setRoute(route);
                    client_connection->sendResponse(error_files);

                    delete client_connection;
                    port->getClients().erase(client_it);
                    port->getPollFds().erase(pfd_it);
                }
            }
        }
    }
    return (NULL);
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
