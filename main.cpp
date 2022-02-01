#include "classes/Config.hpp"
#include "classes/Server.hpp"
#include <sys/poll.h>

#define SERVER_PORT 18000

int     main(void)
{
    Server      default_server;
    std::string host;       // should come from "listen" directive

    host += DFL_SERVER_HOST;
    host += ":";
    host += std::to_string(DFL_SERVER_PORT);
    std::cout << "Connecting to " << host << '\n';
    default_server.initListener(host);

    default_server.initPolling();
    default_server.eventLoop();

    return (EXIT_SUCCESS);
}
