#include <Poller.hpp>

Poller::Poller(PortConfig *port_config) :
    m_port_config(port_config),
    m_new_connection(std::make_pair(0, (Connection*)NULL))
{

}

/* shallow copy of port config */
Poller::Poller(const Poller &cpy) :
    m_pfds(cpy.m_pfds),
    m_port_config(cpy.m_port_config),
    m_dropped_fds(cpy.m_dropped_fds),
    m_new_connection(cpy.m_new_connection)
{

}

Poller::~Poller(void)
{
    delete m_port_config;
}

Poller&         Poller::operator = (const Poller &rhs)
{
    if (this != &rhs)
    {
        m_pfds = rhs.m_pfds;
        m_port_config = rhs.m_port_config;
        m_dropped_fds = rhs.m_dropped_fds;
        m_new_connection = rhs.m_new_connection;
    }
    return (*this);
}

void            *Poller::_pollPort(void *instance)
{
    ConfigUtil      *util;
    Poller          *poller;
    PortConfig      *port_config;
    int             fds_with_events;
    size_t          active_connections;

    util = &ConfigUtil::getHandle();
    poller = static_cast<Poller*>(instance);
    port_config = poller->m_port_config;
    port_config->initSocket();

    struct pollfd listener;
    listener.fd = port_config->getSocket()->getFd();
    listener.events = POLLIN;
    listener.revents = 0;
    poller->m_pfds.push_back(listener);

    for ( ; ; )
    {
        poller->m_new_connection = std::make_pair(0, (Connection*)NULL);

        active_connections = poller->m_pfds.size();
        fds_with_events = poll(&poller->m_pfds.at(0), active_connections, POLL_TIMEOUT_MS);
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

        for (size_t idx = 0; idx < active_connections; ++idx)
        {
            /* if no events continue to next fd */
            if (!poller->m_pfds[idx].revents)
                continue ;

            /* if no more events then we can already break */
            if (!fds_with_events)
                break ;

            fds_with_events--;
            if (poller->m_pfds[idx].revents & (POLLHUP))
            {
                /* handle close connection from client */

            }
            else if (poller->m_pfds[idx].revents & (POLLIN))
            {
                /* if POLLIN on listener socket */
                if (poller->m_pfds[idx].fd == port_config->getSocket()->getFd())
                {
                    poller->_getNewConnection();
                }
                else
                {
                    poller->_readConnectionData(poller->m_pfds[idx].fd);
                }
                usleep(POLLIN_SLEEP_MS);
            }
            else
            {
                if (poller->m_pfds[idx].revents & (POLLOUT))
                {
                    poller->_parseAndRespond(poller->m_pfds[idx].fd);
                    poller->m_dropped_fds.push(poller->m_pfds[idx].fd);
                }
            }
        }

        poller->_updatePollFds();
    }
    delete poller;
    return (NULL);
}

/*
 * Update the internal vector<struct pollfd> and client map
 * - add new connection if available in m_new_connection
 * - empty m_dropped_clients and remove from vector and map
 */
void            Poller::_updatePollFds(void)
{
    struct pollfd       client;
    clients_t::iterator client_it;
    int                 dropped_client_fd;

    if (m_new_connection.second)
    {
        client.fd = m_new_connection.first;
        client.events = (POLLIN | POLLOUT);
        client.revents = 0;
        m_pfds.push_back(client);
        m_clients.insert(m_new_connection);
    }
    while (!m_dropped_fds.empty())
    {
        dropped_client_fd = m_dropped_fds.top();

        /* remove from pollfd vector */
        for (auto it = m_pfds.begin() ; it != m_pfds.end() ; ++it)
        {
            if (it->fd == dropped_client_fd)
            {
                m_pfds.erase(it);
                break ;
            }
        }

        /* remove from client map */
        client_it = m_clients.find(dropped_client_fd);
        delete client_it->second;
        m_clients.erase(client_it);

        m_dropped_fds.pop();
    }
}

void            Poller::_parseAndRespond(int &socket_fd)
{
    ConfigUtil::status_code_map_t   *error_files;
    clients_t::iterator             it;
    Connection                      *connection;
    Route                           *matched_route;

    /* POLLOUT on client socket */
    it              = m_port_config->getClients().find(socket_fd);
    connection      = it->second;
    connection->parseRequest();

    matched_route   = m_port_config->getMatchingRoute(connection->getRequest(), &error_files);
    connection->setRoute(matched_route);
    connection->sendResponse(error_files);
}

void            Poller::_getNewConnection(void)
{
    int             fd;
    SA_IN           addr;
    ClientSocket    *socket;
    Connection      *connection;

    connection = NULL;
    fd = m_port_config->getSocket()->accept(addr);
    if (fd == SYS_ERROR)
    {
        /* some error handling */

    }
    socket      = new ClientSocket(fd, addr);
    connection  = new Connection(socket);
    m_new_connection = std::make_pair(fd, connection);
}

void            Poller::_readConnectionData(int &socket_fd)
{
    clients_t::iterator it;
    Connection          *connection;

    it = m_port_config->getClients().find(socket_fd);
    connection = it->second;
    connection->readSocket();
}

void            *Poller::pollPort(void *instance)
{
    ConfigUtil      *util;
    PortConfig      *port;
    Poller          *poller;
    int             fds_with_events;
    size_t          active_connections;

    int             client_fd;
    SA_IN           client_addr;
    ClientSocket    *client_socket;
    Connection      *client_connection;

    PortConfig::clients_t::iterator     client_it;
    pollfd_t::iterator                  pfd_it;

    util = &ConfigUtil::getHandle();
    poller = static_cast<Poller*>(instance);
    port = poller->m_port_config;
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
    delete poller;
    return (NULL);
}
