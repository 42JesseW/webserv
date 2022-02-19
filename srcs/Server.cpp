#include <Server.hpp>

Server::Server() : m_client_max_body_size(DFL_MAX_BODY_SIZE)
{
    m_names.push_back(DFL_SERVER_NAME);
    m_error_files = ConfigUtil::getHandle().getStatusCodeMap();
}

Server::Server(const Server &server) :
    m_sock(server.m_sock),
    m_names(server.m_names),
    m_routes(server.m_routes),
	m_clients(server.m_clients),
	m_error_files(server.m_error_files),
    m_client_max_body_size(server.m_client_max_body_size),
    m_pfds(server.m_pfds)
{

}

Server::~Server() {}

Server&     Server::operator = (const Server &server)
{
    if (this != &server)
    {
        m_sock = server.m_sock;
        m_names = server.m_names;
        m_routes = server.m_routes;
        m_clients = server.m_clients;
        m_error_files = server.m_error_files;
        m_client_max_body_size = server.m_client_max_body_size;
        m_pfds = server.m_pfds;
    }
    return (*this);
}

int&                        Server::getSockFd()
{
    return (m_sock.getFd());
}

std::vector<std::string>&   Server::getNames()
{
    return (m_names);
}

std::vector<Route>&         Server::getRoutes()
{
    return (m_routes);
}

Server::status_code_map_t&  Server::getErrorFiles()
{
    return (m_error_files);
}

unsigned int&               Server::getClientMaxBodySize()
{
    return (m_client_max_body_size);
}

std::vector<struct pollfd>& Server::getPollPfds()
{
    return (m_pfds);
}

void                        Server::setClientMaxBodySize(unsigned int size)
{
    // TODO maybe some bounds ??
    m_client_max_body_size = size;
}

void                         Server::setStatusBody(int code, const std::string &body)
{
    status_code_map_t::iterator it;

    it = m_error_files.find(code);
    if (it != m_error_files.end())
    {
        m_error_files[code] = std::make_pair(it->second.first, body);
    }
}

int                         Server::initListener()
{
    return (m_sock.init());
}

int                         Server::initListener(const std::string& host)
{
    std::stringstream   num_ss;
    std::string         address;
    short               sin_port;
    size_t              pos;

    if (ft::count(host.begin(), host.end(), ':') > 1)
        throw std::invalid_argument("Host " + host + " is invalid");

    pos = host.find(":");
    num_ss.exceptions(std::ios::failbit | std::ios::badbit);
    num_ss << host.substr(pos + 1);
    num_ss >> sin_port;
    m_sock.setPort(sin_port);

    address = host.substr(0, pos);
    m_sock.setAddress(address);

    return (m_sock.init());
}

void                        *Server::threadedPoll(void *instance)
{
    std::vector<struct pollfd>              *pfds;
    std::vector<struct pollfd>::iterator    iter;
    struct pollfd                           listen_socket_pollfd;
    Server                                  *server;

    server = (Server *)instance;
    pfds = &server->getPollPfds();
    listen_socket_pollfd.fd = server->getSockFd();
    listen_socket_pollfd.events = POLLIN;
    pfds->push_back(listen_socket_pollfd);

    for (;;)
    {
        int poll_count = poll(&pfds->at(0), pfds->size(), POLL_NO_TIMEOUT);
        if (poll_count == -1)
        {
            /* do some error handling */
            std::exit(EXIT_FAILURE);
        }

        iter = pfds->begin();
        for (size_t i = 0; i < pfds->size(); i++)
        {
            if ((pfds->at(i).revents & (POLLERR | POLLNVAL)) ||
                ((pfds->at(i).revents & POLLHUP) && !(pfds->at(i).revents & POLLIN)))
            {
                /* handle flags */
                close(pfds->at(i).fd);
                pfds->erase(iter);
            }

            if (pfds->at(i).revents & POLLIN)
            {
                if (pfds->at(i).fd == server->getSockFd())
                {
                    if (server->acceptNewConnection() < 0)
                    {
                        /* do some error handling */
                        std::exit(EXIT_FAILURE);
                    }
                }
                else
                    server->handleConnection(pfds->at(i).fd);
                usleep(2000);
            }

            if (pfds->at(i).revents & POLLOUT &&
                !(pfds->at(i).revents & (POLLERR | POLLNVAL | POLLHUP)))
            {
                char buff[4096];

                snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nThey see me pollin', they hatin'");
                send(pfds->at(i).fd, (char *)buff, strlen((char *)buff), 0);
                close(pfds->at(i).fd);
                pfds->erase(iter);
            }
            iter++;
        }
    }
    return (NULL);
}

int                         Server::doPolling(void)
{
    std::vector<struct pollfd>::iterator    iter;
    struct pollfd                           listen_socket_pollfd;

    listen_socket_pollfd.fd = m_sock.getFd();
    listen_socket_pollfd.events = POLLIN;
    m_pfds.push_back(listen_socket_pollfd);

    for (;;)
    {
        int poll_count = poll(&m_pfds[0], m_pfds.size(), POLL_NO_TIMEOUT);
        if (poll_count == -1)
        {
            /* do some error handling */
            std::exit(EXIT_FAILURE);
        }

    	iter = m_pfds.begin();
        for (size_t i = 0; i < m_pfds.size(); i++)
        {
            if ((m_pfds[i].revents & (POLLERR | POLLNVAL)) || 
                ((m_pfds[i].revents & POLLHUP) && !(m_pfds[i].revents & POLLIN)))
			{
				/* handle flags */
		        close(m_pfds[i].fd);
		        m_pfds.erase(iter);
			}

            if (m_pfds[i].revents & POLLIN)
            {
                if (m_pfds[i].fd == m_sock.getFd())
                {
                    if (acceptNewConnection() < 0)
                    {
                        /* do some error handling */
                        std::exit(EXIT_FAILURE);
                    }
				}
                else
					handleConnection(m_pfds[i].fd);
                usleep(2000);
            }

			if (m_pfds[i].revents & POLLOUT &&
                !(m_pfds[i].revents & (POLLERR | POLLNVAL | POLLHUP)))
			{
				char buff[4096];
                
                snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nThey see me pollin', they hatin', for listener %d", m_sock.getFd());
                send(m_pfds[i].fd, (char *)buff, strlen((char *)buff), 0);
                close(m_pfds[i].fd);
		        m_pfds.erase(iter);
			}
			iter++;
        }
    }
    return (SOCK_SUCCESS);
}

void                 		Server::addToPfds(int client_socket)
{
    struct pollfd	client_socket_pollfd;

    client_socket_pollfd.fd = client_socket;
    client_socket_pollfd.events = (POLLIN | POLLOUT);
    client_socket_pollfd.revents = 0;
    m_pfds.push_back(client_socket_pollfd);
}

int                         Server::acceptNewConnection(void) 
{
    Client  new_client;
    int     client_socket;
    SA_IN   client_addr;
    int     addr_size;

    addr_size = sizeof(client_addr);
    client_socket = accept(m_sock.getFd(), (SA *)&client_addr, (socklen_t *)&addr_size);
    if (client_socket == SOCK_ERROR)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }

    if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == SOCK_ERROR)
    {
        /* some error handling */
        close(client_socket);
        std::exit(EXIT_FAILURE);
    }

	int enable = 1;
	if (setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
        /* some error handling */
		close(client_socket);
        std::exit(EXIT_FAILURE);
	}

    m_clients.insert(std::pair<int, Client>(client_socket, new_client));

    addToPfds(client_socket);

    std::cout << "New connection established on client socket: " << client_socket << " listener: " << m_sock.getFd() << std::endl;
    return (SOCK_SUCCESS);
}

void						Server::handleConnection(int client_socket)
{
    Request new_request;

    new_request.handleRequest(client_socket);
}
