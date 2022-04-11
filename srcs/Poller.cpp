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

void            *Poller::pollPort(void *instance)
{
    ConfigUtil      *util;
    Poller          *poller;
    PortConfig      *port_config;
    int             fds_with_events;
    size_t          active_connections;

    util = &ConfigUtil::getHandle();
    poller = Poller::_initPoller(&port_config, instance);
    for ( ; ; )
    {
        poller->m_new_connection = std::make_pair(0, (Connection*)NULL);

        active_connections = poller->m_pfds.size();
        fds_with_events = poll(&poller->m_pfds.at(0), active_connections, POLL_TIMEOUT_MS);
        if (fds_with_events == SYS_ERROR) {
            util->setSignalled(SIGINT);
        }
        if (util->isSignalled()) {
            break ;
        }
        if (!fds_with_events) {
            continue ;
        }
        for (size_t idx = 0; idx < active_connections; ++idx)
        {
            if (!poller->m_pfds[idx].revents)
                continue ;

            if (!fds_with_events)
                break ;

            fds_with_events--;
            if (poller->m_pfds[idx].revents & (POLLHUP)) {
                poller->m_dropped_fds.push(poller->m_pfds[idx].fd);
            }
            else if (poller->m_pfds[idx].revents & (POLLIN))
            {
                if (poller->m_pfds[idx].fd == port_config->getSocket()->getFd()) {
                    poller->_getNewConnection();
                }
                else {
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
 * Initialise the poller:
 * - convert (void*) to Poller*
 * - set and init port_config
 * - return new Poller instance
 */
Poller            *Poller::_initPoller(PortConfig **port_config, void *instance)
{
    Poller          *poller;
    PortConfig      *pc;
    struct pollfd   listener;

    poller = static_cast<Poller*>(instance);
    pc = poller->m_port_config;
    pc->initSocket();

    listener.fd = pc->getSocket()->getFd();
    listener.events = POLLIN;
    listener.revents = 0;
    poller->m_pfds.push_back(listener);

    *port_config = pc;
    return (poller);
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
    it              = m_clients.find(socket_fd);
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

    it = m_clients.find(socket_fd);
    connection = it->second;
    connection->readSocket();
}
