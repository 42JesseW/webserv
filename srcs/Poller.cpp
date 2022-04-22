#include <Poller.hpp>
#include <Handler.hpp>

#define reponse "HTTP/1.1 200 OK\r\nDate: Mon, 27 Jul 2009 12:28:53 GMT\r\nServer: Apache/2.2.14 (Win32)\r\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\nContent-Length: 88\r\nContent-Type: text/html\r\nConnection: Closed"

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
                else if (poller->_checkIfCGIFd(poller->m_pfds[idx].fd))
                {
                    poller->m_dropped_fds.push(poller->m_pfds[idx].fd);
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
                    poller->_parse(poller->m_pfds[idx].fd);
                    if (poller->_checkIfCGIConnection(poller->m_pfds[idx].fd))
                    {
                        poller->_initAndExecCGI(poller->m_pfds[idx].fd);
                    }
                    else
                    {
                        poller->_respond(poller->m_pfds[idx].fd);
                        poller->m_dropped_fds.push(poller->m_pfds[idx].fd);
                    }
                }
            }
        }
        poller->_deletePollFds();
        poller->_addPollFds();
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
void            Poller::_addPollFds(void)
{
    struct pollfd       client;
    clients_t::iterator client_it;
    Connection          *connection = NULL;

    if (m_new_connection.second)
    {
        client.fd = m_new_connection.first;
        client.events = (POLLIN | POLLOUT);
        client.revents = 0;
        m_pfds.push_back(client);
        m_clients.insert(m_new_connection);
    }
    for (client_it = m_clients.begin() ; client_it != m_clients.end() ; ++client_it)
    {
        connection = client_it->second;
        if (connection->getCGI() != NULL && connection->m_cgi_added == false)
        {
            m_pfds.push_back(connection->getCGI()->getPollFdStruct());
            connection->m_cgi_added = true;
        }
    }
}

void            Poller::_deletePollFds(void)
{
    int                 dropped_client_fd;
    clients_t::iterator client_it;

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
        if (client_it != m_clients.end())
        {
            delete client_it->second;
            m_clients.erase(client_it);
        }
        m_dropped_fds.pop();
    }
}

void            Poller::_parse(int &socket_fd)
{
    clients_t::iterator it;
    Connection          *connection;

    it = m_clients.find(socket_fd);
    connection = it->second;

    connection = m_clients.find(socket_fd)->second;
    if (connection->getRequest().getHeaders().empty())
        connection->parseRequest();
}

void            Poller::_respond(int &socket_fd)
{
    clients_t::iterator             it;
    Connection                      *connection;
    Route                           *matched_route;
    ConfigUtil::status_code_map_t   *error_files = NULL;

    it = m_clients.find(socket_fd);
    connection = it->second;

    matched_route   = m_port_config->getMatchingRoute(connection->getRequest(), &error_files);
    if (matched_route != NULL && connection->getRequest().getStatus() == HTTP_STATUS_OK)
    {
        connection->setRoute(matched_route);
    }
    connection->sendResponse(error_files);
}

bool            Poller::_checkIfCGIConnection(int socket_fd)
{
    clients_t::iterator it;
    Connection          *connection;

    it = m_clients.find(socket_fd);
    connection = it->second;
    if (connection->getRequest().isCGI())
    {
        return (true);
    }
    return (false);
}

void            Poller::_initAndExecCGI(int socket_fd)
{
    clients_t::iterator it;
    Connection          *connection;

    it = m_clients.find(socket_fd);
    connection = it->second;
    if (!connection->getCGI())
    {
        connection->initCGI();
        connection->getCGI()->exec();
    }
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

bool            Poller::_checkIfCGIFd(int socket_fd)
{
    clients_t::iterator             client_it;
    Connection                      *connection = NULL;
    char                            *buff;
    ssize_t                         bytes_read;

    buff = new char[RECV_SIZE + 1];
    for (client_it = m_clients.begin() ; client_it != m_clients.end() ; ++client_it)
    {
        if (client_it->second->getCGI() != NULL && client_it->second->getCGI()->getPipeReadFd() == socket_fd)
        {
            connection = client_it->second;
            if ((bytes_read = ::read(socket_fd, buff, RECV_SIZE)) == SYS_ERROR) {
                fprintf(stderr, "Failed to read from socket: %s\n", strerror(errno));
                return (false);
            }
            else
            {
                connection->getCGI()->appendResponse(buff, bytes_read);
                connection->getSock()->send(connection->getCGI()->getResponse().c_str());
                m_dropped_fds.push(client_it->first);
            }
            return (true);
        }
    }
    return (false);
}
