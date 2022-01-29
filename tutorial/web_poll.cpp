#include "classes/Config.hpp"
#include <sys/poll.h>

#define SERVER_PORT 18000

int     main(void)
{
    Config  server_config;
    Server  default_server;

    if (default_server.initListener(SERVER_PORT) == SOCK_ERROR)
    {
        /* do some error handling */
        return (EXIT_FAILURE);
    }


}
