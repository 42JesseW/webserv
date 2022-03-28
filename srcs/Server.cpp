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

/*
 * Since Socket object is stack allocated
 * it will automatically be destructed when
 * the server destructs.
 */
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

void						Server::_handleErrorEvents(int i, pollfd_vec_t::iterator iter)
{
    if ((m_pfds[i].revents & (POLLERR | POLLNVAL)) || ((m_pfds[i].revents & POLLHUP)
        && !(m_pfds[i].revents & POLLIN)))
    {
        /* handle flags */
        close(m_pfds[i].fd);
        m_pfds.erase(iter);
    }
}

void                        Server::_handlePollin(int i)
{
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
}

void						Server::_handlePollout(int i, pollfd_vec_t::iterator iter)
{
    if (m_pfds[i].revents & POLLOUT && !(m_pfds[i].revents & (POLLERR | POLLNVAL | POLLHUP)) && !(m_pfds[i].revents & POLLIN))
    {
        Client *this_client = &m_clients.at(m_pfds[i].fd);
        Response this_response(this_client->m_request, this_client->m_route);

        this_client->m_response = this_response;
        this_client->m_response.buildResponse(m_error_files);
        send(m_pfds[i].fd, this_client->m_response.getResponse().c_str(), this_client->m_response.getResponse().length(), 0);
        close(m_pfds[i].fd);
        m_pfds.erase(iter);
        this_client->m_response.resetResponse();
    }
}

void                        *Server::threadedPoll(void *instance)
{
    struct pollfd           pollfds;
    pollfd_vec_t            *pfds;
    pollfd_vec_t::iterator  iter;
    Server                  *server;
    int                     poll_count;
    ConfigUtil              *util;

    server = (Server *)instance;
    util = &ConfigUtil::getHandle();
    pfds = &server->getPollPfds();
    pollfds.fd = server->getSockFd();
    pollfds.events = POLLIN;
    pfds->push_back(pollfds);
    for ( ;; )
    {
        poll_count = poll(&server->getPollPfds().at(0), server->getPollPfds().size(), 500);
        if (poll_count == SYS_ERROR)
        {
            /* do some error handling */
            break ;
        }

        if (util->isSignalled())
        {
            std::cout << "[INFO] Thread was signalled " << '\n';
            break ;
        }

        iter = server->getPollPfds().begin();
        for (size_t i = 0; i < server->getPollPfds().size(); i++)
        {
            server->_handleErrorEvents(i, iter);
            server->_handlePollin(i);
            server->_handlePollout(i, iter);
            iter++;
        }
    }
    return (NULL);
}

void                 		Server::_addToPfds(int client_socket)
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

    _addToPfds(client_socket);

    std::cout << "New connection established on client socket: " << client_socket << " listener: " << m_sock.getFd() << std::endl;
    return (SOCK_SUCCESS);
}

void						Server::handleConnection(int client_socket)
{
    Client *this_client = &m_clients.at(client_socket);
    this_client->setSocket(client_socket);
    this_client->m_request.handleRequest(client_socket);
    if (this_client->m_request.isDone())
    {
        this_client->setCorrectRoute(this->getRoutes());
        this_client->checkRoute();
        this_client->m_request.setDone(false);
        // this_client->m_request.printRequest();
    }
}
