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
    int                         initListener(const short port);

};

#endif
