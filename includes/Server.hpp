#ifndef SERVER_HPP

# define SERVER_HPP

# define POLL_NO_TIMEOUT (-1)

# define DFL_MAX_BODY_SIZE 1                    /* in MB */
# define DFL_SERVER_PORT 8080                   /* HTTP default port */
# define DFL_SERVER_NAME ""

# include <Common.hpp>
# include <Route.hpp>
# include <Socket.hpp>
# include <Client.hpp>
# include <Request.hpp>
# include <ConfigUtil.hpp>

# include <poll.h>

class Server
{
public:
    typedef ConfigUtil::status_code_map_t   status_code_map_t;

private:
    /* configuration options */
    Socket                      m_sock;
    std::vector<std::string>    m_names;
    std::vector<Route>          m_routes;
    std::map<int, Client>       m_clients;
    status_code_map_t           m_error_files;
    unsigned int                m_client_max_body_size;

    std::vector<struct pollfd>  m_pfds;

public:
    Server();
    Server(const Server& server);
    ~Server();

    Server& operator = (const Server& server);

    int&                        getSockFd();
    std::vector<std::string>&   getNames();
    std::vector<Route>&         getRoutes();
    status_code_map_t&          getErrorFiles();
    unsigned int&               getClientMaxBodySize();
    std::vector<struct pollfd>& getPollPfds();

    void                        setClientMaxBodySize(unsigned int size);
    void                        setStatusBody(int code, const std::string &body);

    int                         initListener(void);
    int                         initListener(const std::string& host);
	int							doPolling(void);

    int							acceptNewConnection(void);
    void						handleConnection(int client_socket);

    static void                 *threadedPoll(void *instance);

private:
    void                        addToPfds(int client_socket);

};

#endif
