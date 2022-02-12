#include <Config.hpp>
#include <Server.hpp>

int     main(int argc, char *argv[], char *envp[])
{
    Config  *config;
    Server  server;

    config = &Config::getHandle();
    if (argc > 1)
    {
        config->setFilePath(argv[1]);
    }
    config->loadFile();
    for (size_t idx = 0; idx < config->getServers().size(); idx++)
    {
        server = config->getServers().at(idx);
        server.doPolling();
    }
    return (EXIT_SUCCESS);
}
