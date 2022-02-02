#include "../includes/Server.hpp"

Server::Server()
{

}

Server::Server(const Server &server) :
    m_sock(server.m_sock),
    m_names(server.m_names),
    m_routes(server.m_routes)
{

}

Server::~Server()
{

}

Server&     Server::operator = (const Server &server)
{
    if (this != &server)
    {
        *this = server;
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

int                         Server::acceptNewConnection(void) {
    int     client_socket;
    SA_IN   client_addr;
    int     addr_size;
    int     yes;

    addr_size = sizeof(SA_IN);
    client_socket = accept(m_sock.getFileDescriptor(), (SA *)&client_addr, (socklen_t *)&addr_size);
    if (client_socket == SOCK_ERROR)
    {
        /* do some error handling */
        std::exit(EXIT_FAILURE);
    }

    /* loose the 'address currently in use' error because the kernel hasn't properly cleaned everything up */
    yes = 1;
    if (setsockopt(m_sock.getFileDescriptor(), SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    /* set the socket to be non blocking so recv() and send() functions don't block */
    if (fcntl(m_sock.getFileDescriptor(), F_SETFL, O_NONBLOCK) == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }
    return (client_socket);
}

int                         Server::handleConnection(int client_socket) {
    int         n;
    uint8_t     buff[MAX_RECV_LINE + 1];
    uint8_t     recvline[MAX_RECV_LINE + 1];

    memset(recvline, 0, MAX_RECV_LINE);
    while ( (n = read(client_socket, recvline, MAX_RECV_LINE - 1) ) > 0) {
        fprintf(stdout, "\n%s\n\n%s", _bin2Hex(recvline, n), recvline);

        if (recvline[n - 1] == '\n') {
            break ;
        }
        memset(recvline, 0, MAX_RECV_LINE);
    }
    if (n == SOCK_ERROR)
    {
        /* some error handling */
        std::exit(EXIT_FAILURE);
    }

    /* create a response message with message OK status code 200 and body Hello */
    snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nHello");

    write(client_socket, (char *)buff, strlen((char *)buff));
    close(client_socket);
    return (SOCK_SUCCESS);
}

char                        *Server::_bin2Hex(const unsigned char *input, size_t len) {
    int     result_length;
    char    *result;
    // char    *hexits = "0123456789ABCDEF";
    char    hexits[17] = "0123456789ABCDEF";

    if (!input || len <= 0)
        return (NULL);
    result_length = (3 * len) + 1;
    result = (char *)std::calloc(result_length, sizeof(char));
    if (!result)
        return (NULL);
    for (size_t i = 0; i < len; i++) {
        result[i * 3]       = hexits[input[i] >> 4];
        result[(i * 3) + 1] = hexits[input[i] & 0x0F];
        result[(i * 3) + 2] = ' ';
    }
    return (result);
}
