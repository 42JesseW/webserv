#include "../includes/Config.hpp"
#include "../includes/Server.hpp"

#include <Config.hpp>

#include <sys/poll.h>

#define SERVER_PORT 18000

int     main(void)
{
    Server      default_server;
    std::string host;       // should come from "listen" directive
    std::map<int, std::string>  *err_files;

    host += DFL_SERVER_HOST;
    host += ":";
    host += std::to_string(DFL_SERVER_PORT);
    std::cout << "Connecting to " << host << '\n';
    default_server.initListener(host);

    err_files = &Config::getHandle().getErrorFiles();
    std::cout << err_files->size() << '\n';

    return (EXIT_SUCCESS);
}
