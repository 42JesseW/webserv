#ifndef SERVER_HPP

# define SERVER_HPP

# define MAX_RECV_LINE 4096

# include "Route.hpp"
# include "Socket.hpp"
# include "Client.hpp"

# include <map>
# include <sstream>

class Server
{
private:
    Socket                      m_sock;
    std::vector<std::string>    m_names;
    std::vector<Route>          m_routes;
    std::map<int, Client>       m_clients;

public:
    Server();
    Server(const Server& server);
    ~Server();

    Server& operator = (const Server& server);

    // TODO maybe a getSocketFd function ??
    std::vector<std::string>&   getNames();
    std::vector<Route>&         getRoutes();

    /* returns sys_error if failed */
    int                         initListener(const std::string& host);
    int                         acceptNewConnection(void);
    // TODO could match with Client object
    int                         handleConnection(int client_socket);

private:
    char                        *_bin2Hex(const unsigned char *input, size_t len);

};

namespace ft {

    template< class InputIt, class T >
    size_t  count( InputIt first, InputIt last, const T& value )
    {
        size_t  counter;

        counter = 0;
        while (first != last)
        {
            if (*first == value)
                counter++;
            first++;
        }
        return (counter);
    }
}

#endif
