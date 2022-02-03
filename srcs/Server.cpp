#include "../includes/Server.hpp"

Server::Server()
{
    m_poll.fd_count = 0;
    // Initialized to max clients that a server can get
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

// struct pollfd               *Server::getPointer()
// {
//     return &(m_poll.pfds[0]);
// }

/*
** defaults are passed from Config (DFL_SERVER_*)
** host species a full host <host>:<port>
*/

int                         Server::initListener(const std::string& host)
{
    std::string address;
    short       sin_port;

    if (ft::count(host.begin(), host.end(), ':') > 1)
        throw std::invalid_argument("Host " + host + " is invalid");

    /* convert the <port> from <host>:<port> to an integer */
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
    // Add the listener to the set
    m_poll.pfds[0].fd = m_sock.getFileDescriptor();
    m_poll.pfds[0].events = POLLIN;
    m_poll.fd_count = 1;

    for (;;)
    {
        // int poll_count = poll(getPointer(), m_poll.fd_count, POLL_TIMEOUT);
        int poll_count = poll(m_poll.pfds, m_poll.fd_count, POLL_TIMEOUT);

        if (poll_count == -1)
        {
            /* do some error handling */
            std::exit(EXIT_FAILURE);
        }

        // Run through the existing connections looking for requests and responses
        for (int i = 0; i < m_poll.fd_count; i++)
        {
            // Error flags
            if (m_poll.pfds[i].revents & POLLERR || m_poll.pfds[i].revents & POLLNVAL || m_poll.pfds[i].revents & POLLHUP)
			{
				/* handle flags */
			}

            // Check if someone's ready to send request
            else if (m_poll.pfds[i].revents & POLLIN)
            {
				// New connection
                if (m_poll.pfds[i].fd == m_sock.getFileDescriptor())
                {
                    if (acceptNewConnection() < 0)
                    {
                        /* do some error handling */
                        std::exit(EXIT_FAILURE);
                    }
				}
				// Existing connection - recv
                else
					handleConnection(m_poll.pfds[i].fd, i);
            }

            // Check if someone's ready to receive response
			else if (m_poll.pfds[i].revents & POLLOUT)
			{
				// build response object to the specific pfds[i].fd socket
				char buf[10] = "Hello";
				if (send(m_poll.pfds[i].revents, buf, 10, 0) == -1)
					std::exit(EXIT_FAILURE);
			}
        }
    }
    return (SOCK_SUCCESS);
}

void                 		Server::addToPfds(int client_socket)
{
    // Realloc in case there is no room in the array - not sure if needed since we are using the 
    // DFL_BACKLOG in order to alloc initially
    if (m_poll.fd_count == m_poll.fd_size)
    {
        m_poll.fd_size *= 2;
        m_poll.pfds = (struct pollfd *)realloc(m_poll.pfds, sizeof(*m_poll.pfds) * m_poll.fd_size);
    }

    m_poll.pfds[m_poll.fd_count].fd = client_socket;
    m_poll.pfds[m_poll.fd_count].events = POLLIN;

    m_poll.fd_count++;
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
    
    std::cout << "New connection on socket: " << m_sock.getFileDescriptor() << std::endl;
    /* set the socket to be non blocking so recv() and send() functions don't block */
    if (fcntl(client_socket, F_SETFL, O_NONBLOCK) == SOCK_ERROR)
    {
        /* some error handling */
        // std::exit(EXIT_FAILURE);
    }
    return (SOCK_SUCCESS);
}

void					    Server::delFromPfds(int i)
{
	m_poll.pfds[i] = m_poll.pfds[m_poll.fd_count - 1];

	m_poll.fd_count--;
}

void                         Server::handleConnection(int client_socket, int i)
{
    int     nbytes;
    char    buf[256]; 

    (void)i;
    nbytes = recv(client_socket, buf, sizeof(buf), 0);
    // Connection closed by client (==0) or got error
    if (nbytes <= 0)
	{
		close(client_socket);
		delFromPfds(i);
	}
}
