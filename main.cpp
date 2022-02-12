#include <Config.hpp>
#include <Server.hpp>

#define SERVER_PORT 18000

int     main(void)
{
    Server      default_server;
    std::string host;       // should come from "listen" directive
    std::map<int, std::string>  *err_files;
    std::string dfl_port = "1800";

    host += DFL_SERVER_HOST;
    host += ":";
    host += dfl_port;
    std::cout << "Connecting to " << host << '\n';
    default_server.initListener(host);

    err_files = &Config::getHandle().getErrorFiles();
    std::cout << err_files->size() << '\n';
    default_server.doPolling();

    return (EXIT_SUCCESS);
}
