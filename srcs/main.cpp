#include <Config.hpp>
#include <Server.hpp>

#include <iostream>

/*
 * If a signal occurs then the signal handler sets
 * the m_is_signalled flag in the Config singleton
 * instance. Both .setSignalled and .isSignalled are
 * protected by a mutex and can be used to interact
 * with this flag. The threads use .isSignalled to
 * check the flag.
 */
void            sigint_handler(int sig)
{
    ConfigUtil  *util;

    (void)sig;
    util = &ConfigUtil::getHandle();
    util->setSignalled();
    std::cout << "[INFO] Server SIGINT received " << '\n';
}

/* Seems like "\e" is GNU only and "\033" is the standard sequence. */
#define B "\033[0;38;2;42;111;240m"
#define T "\033[0;38;2;48;213;200m"
#define R "\033[m"

static char ascii_wave[] = "\n"
   "██╗    ██╗███████╗██████╗ ███████╗██╗   ██╗██████╗ ███████╗" B "           _.====.._                    \n" R
   T "██║    ██║██╔════╝██╔══██╗██╔════╝██║   ██║██╔══██╗██╔════╝" B "         ,:._       ~-_                 \n" R
   T "██║ █╗ ██║█████╗  ██████╔╝███████╗██║   ██║██████╔╝█████╗  " B "             `\\        ~-_             \n" R
   T "██║███╗██║██╔══╝  ██╔══██╗╚════██║██║   ██║██╔══██╗██╔══╝  " B "               |          `.            \n" R
   T "╚███╔███╔╝███████╗██████╔╝███████║╚██████╔╝██║  ██║██║     " B "             ,/             ~-_         \n" R
   T " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝     " B "           -''                 ~~--..__.\n" R
   B ".-..____.-..____.-..____.-.._____.-..____.-..________.-..____.-.._..-'\n" R;


int     main(int argc, char *argv[])
{
    Config      *config;
    Server      *server;
    int         exit_code;
    pthread_t   *thread_ids;

    thread_ids = NULL;
    exit_code = EXIT_SUCCESS;
    signal(SIGINT, sigint_handler);
    config = &Config::getHandle();
    if (argc > 1)
        config->setFilePath(argv[1]);
    try
    {
        std::cout << ascii_wave << '\n';
        config->loadFile();
        thread_ids = new pthread_t[config->getServers().size()];
        for (std::vector<Server>::size_type idx = 0; idx < config->getServers().size(); ++idx)
        {
            server = &config->getServers().at(idx);
            if (pthread_create(&thread_ids[idx], NULL, (THREAD_FUNC_PTR)Server::threadedPoll, server) == SYS_ERROR)
            {
                /* error handling */
                break ;
            }
        }
        for (std::vector<Server>::size_type idx = 0; idx < config->getServers().size(); ++idx)
        {
            pthread_join(thread_ids[idx], NULL);
            std::cout << "[INFO] Joined thread " << idx << '\n';
        }
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
