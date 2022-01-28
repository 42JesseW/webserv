#include "web.h"
#include <sys/poll.h>
#include <map>

class Server
{
private:
    /* stores a map of all server_ports registered */
    std::map<int, SA_IN>    server_ports;

public:
    Server()
    {

    }

    ~Server()
    {

    }
};

int     main(void)
{
    struct pollfd   *watch_fds;
    int             server_socket_fd;
    int             server_ports[] = {18000, 18001, 18002};

    /* get server socket fd for listening on SERVER_PORT */
    server_socket_fd = setup_server(SERVER_PORT, DFL_BACKLOG);

    //watch_fds = 
}
