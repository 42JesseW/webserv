#ifndef SERVER_HPP

# define SERVER_HPP

# define POLL_NO_TIMEOUT (-1)

# define DFL_MAX_BODY_SIZE 1                    /* in MB */
# define DFL_SERVER_PORT 8080                   /* HTTP default port */
# define DFL_SERVER_NAME ""
    
# include "Route.hpp"
# include "Socket.hpp"
# include "Client.hpp"

# include <vector>
# include <map>
# include <sstream>
# include <poll.h>

class Server
{
public:
    typedef std::map<int, std::string>  err_file_map_t;

private:
    /* configuration options */
    Socket                      m_sock;
    std::vector<std::string>    m_names;
    std::vector<Route>          m_routes;
    std::map<int, Client>       m_clients;
    err_file_map_t              m_error_files;
    unsigned int                m_client_max_body_size;

    std::vector<struct pollfd>	m_pfds;

public:
    Server();
    Server(const Server& server);
    ~Server();

    Server& operator = (const Server& server);

    int&                        getSockFd();
    std::vector<std::string>&   getNames();
    std::vector<Route>&         getRoutes();
    err_file_map_t&             getErrorFiles();

    void                        setClientMaxBodySize(unsigned int size);

    int                         initListener(const std::string& host);
	int							doPolling(void);

    int							acceptNewConnection(void);
    void						handleConnection(int client_socket);

private:
    void                        addToPfds(int client_socket);

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
