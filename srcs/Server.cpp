#include "../includes/Server.hpp"

Server::Server()
{
    m_poll.fd_count = 0;
    m_poll.connections = 0;
    m_poll.fd_size = DFL_BACKLOG;
    m_poll.pfds = (struct pollfd *)malloc(sizeof(*m_poll.pfds) * m_poll.fd_size);
}

Server::Server(const Server &server) :
    m_sock(server.m_sock),
    m_names(server.m_names),
    m_routes(server.m_routes),
    m_poll(server.m_poll)
{

}

Server::~Server()
{

}

Server&     Server::operator = (const Server &server)
{
    if (this != &server)
    {
        m_sock = server.m_sock;
        m_names = server.m_names;
        m_routes = server.m_routes;
        m_clients = server.m_clients;
    }
    return (*this);
}

std::vector<std::string>&   Server::getNames()
{
    return (m_names);
}

std::vector<Route>&         Server::getRoutes()
{
    return (m_routes);
}

int                         Server::initListener(const std::string& host)
{
    std::string address;
    short       sin_port;

    if (ft::count(host.begin(), host.end(), ':') > 1)
        throw std::invalid_argument("Host " + host + " is invalid");

    sin_port = std::atoi(host.substr(host.find(":") + 1).c_str());

    address = host.substr(0, host.find(":"));
    if (sin_port == 0 || m_sock.init(address, sin_port) == SOCK_ERROR)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }
    return (SOCK_SUCCESS);
}

int                         Server::doPolling(void)
{
    m_poll.pfds[0].fd = m_sock.getFileDescriptor();
    m_poll.pfds[0].events = POLLIN;
    m_poll.fd_count = 1;

    for (;;)
    {
        m_poll.fd_count = m_poll.connections + 1;

        int poll_count = poll(m_poll.pfds, m_poll.fd_count, POLL_NO_TIMEOUT);
        if (poll_count == -1)
        {
            /* do some error handling */
            std::exit(EXIT_FAILURE);
        }

        for (int i = 0; i < m_poll.fd_count; i++)
        {
            if (m_poll.pfds[i].revents & (POLLERR | POLLNVAL))
			{
                
				/* handle flags */
			}

            if (m_poll.pfds[i].revents & POLLHUP)
            {
		        close(m_poll.pfds[i].fd);
		        delFromPfds(i);
            }

            if (m_poll.pfds[i].revents & POLLIN)
            {
                if (m_poll.pfds[i].fd == m_sock.getFileDescriptor())
                {
                    if (acceptNewConnection() < 0)
                    {
                        /* do some error handling */
                        std::exit(EXIT_FAILURE);
                    }
				}
                else
					handleConnection(m_poll.pfds[i].fd, i);
            }

			if (m_poll.pfds[i].revents & POLLOUT)
			{
				char buff[4096];
                
                snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nThey see me pollin', they hatin'");
                write(m_poll.pfds[i].fd, (char *)buff, strlen((char *)buff));
                close(m_poll.pfds[i].fd);
		        delFromPfds(i);
			}
        }
    }
    return (SOCK_SUCCESS);
}

void                 		Server::addToPfds(int client_socket)
{
    m_poll.pfds[m_poll.fd_count].fd = client_socket;
    m_poll.pfds[m_poll.fd_count].events = (POLLIN | POLLOUT);

    m_poll.connections++;
}

int                         Server::acceptNewConnection(void) 
{
    Client  new_client;
    int     client_socket;
    SA_IN   client_addr;
    int     addr_size;

    addr_size = sizeof(SA_IN);
    client_socket = accept(m_sock.getFileDescriptor(), (SA *)&client_addr, (socklen_t *)&addr_size);
    if (client_socket == SOCK_ERROR)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }

    m_clients.insert(std::pair<int, Client>(client_socket, new_client));

    addToPfds(client_socket);

    std::cout << "New connection established on client socket: " << client_socket << std::endl;
    if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }
    return (SOCK_SUCCESS);
}

void					    Server::delFromPfds(int i)
{
	m_poll.pfds[i] = m_poll.pfds[m_poll.fd_count - 1];

	m_poll.connections--;
}

void						Server::handleConnection(int client_socket, int i)
{
    int     nbytes;
    char    buf[4096]; 

    nbytes = recv(client_socket, buf, 4095, MSG_DONTWAIT);
    if (nbytes < 0)
	{
	    close(client_socket);
	    delFromPfds(i);
	}
    else 
        buf[nbytes] = 0;
    std::cout << buf << std::endl;
}
