#include <Config.hpp>
#include <Server.hpp>

#include <pthread.h>

typedef void    *(*THREAD_FUNC_PTR)(void *);

int     main(int argc, char *argv[])
{
    Config      *config;
    Server      *server;
    int         exit_code;
    pthread_t   *thread_ids;

    thread_ids = NULL;
    exit_code = EXIT_SUCCESS;
    config = &Config::getHandle();
    if (argc > 1)
        config->setFilePath(argv[1]);
    try
    {
        config->loadFile();
        thread_ids = new pthread_t[config->getServers().size()];
        for (int idx = 0; idx < config->getServers().size(); ++idx)
        {
            server = &config->getServers().at(idx);
            if (pthread_create(&thread_ids[idx], NULL, (THREAD_FUNC_PTR)Server::threadedPoll, server) == SYS_ERROR)
            {
                /* error handling */
                break ;
            }
        }
        for (int idx = 0; idx < config->getServers().size(); ++idx)
            pthread_join(thread_ids[idx], NULL);
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << "[ERROR] Invalid configuration: " << e.what() << '\n';
        exit_code = EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        std::cerr << "[ERROR] Failed to start program: " << argv[0] << ": " << e.what() << '\n';
        exit_code = EXIT_FAILURE;
    }
    delete [] thread_ids;
    return (exit_code);
}
