#ifndef SERVER_HPP

# define SERVER_HPP

# include "Route.hpp"
# include "Socket.hpp"

class Server
{
private:
    Socket                      m_sock;
    std::vector<std::string>    m_names;
    std::vector<Route>          m_routes;

public:
    Server();
    Server(const Server& server);
    ~Server();

    Server& operator = (const Server& server);

    const Socket&               getSocket();
    std::vector<std::string>&   getNames();
    std::vector<Route>&         getRoutes();

    /* returns sys_error if failed */
    int                         initListener(const std::string& host);
    int                         initPolling(void);
    int                         eventLoop(void);
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
