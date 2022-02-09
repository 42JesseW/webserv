#ifndef SERVER_HPP

# define SERVER_HPP

# define MAX_RECV_LINE  4096
# define POLL_NO_TIMEOUT   -1
    
# include "Route.hpp"
# include "Socket.hpp"
# include "Client.hpp"

# include <vector>
# include <map>
# include <poll.h>

struct polling_opts
{
    struct pollfd	            *pfds;
    int							fd_count;
    int                         connections;
    int                         flag;
	int							fd_size;            
};

class Server
{
private:
    Socket                      m_sock;
    std::vector<std::string>    m_names;
    std::vector<Route>          m_routes;
    std::map<int, Client>       m_clients;
	struct polling_opts			m_poll;

public:
    Server();
    Server(const Server& server);
    ~Server();

    Server& operator = (const Server& server);

    std::vector<std::string>&   getNames();
    std::vector<Route>&         getRoutes();

    int                         initListener(const std::string& host);
	int							doPolling(void);

    int							acceptNewConnection(void);
    void						handleConnection(int client_socket, int i);

private:
    void                        addToPfds(int client_socket);
    void                        delFromPfds(int i);
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
