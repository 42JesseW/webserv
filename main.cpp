#include "includes/Config.hpp"
#include "includes/Server.hpp"
#include <sys/poll.h>

#define SERVER_PORT 18000

int     main(void)
{
    Server      default_server;
    std::string host;       // should come from "listen" directive
    std::string dfl_port = "1800";

    host += DFL_SERVER_HOST;
    host += ":";
    host += dfl_port;
    std::cout << "Connecting to " << host << '\n';
    default_server.initListener(host);

    default_server.doPolling();

    return (EXIT_SUCCESS);
}
