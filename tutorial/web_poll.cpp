#include "classes/Config.hpp"
#include <sys/poll.h>

#define SERVER_PORT     18000
#define DFL_POLLFD_SIZE 1024
#define POLL_NO_TIMEOUT -1
#define SYS_ERROR       -1

typedef struct pollfd PFD;

int     main(void)
{
    Server      default_server;
    PFD         *poll_fds;
    nfds_t      fds_in_list;
    std::string host;           // should come from "listen" directive
    int         exit_code;

    host += DFL_SERVER_HOST;
    host += ":";
    host += std::to_string(SERVER_PORT);
    std::cout << "Connecting to " << host << '\n';
    default_server.initListener(host);

    poll_fds = (PFD *)malloc(DFL_POLLFD_SIZE * sizeof(PFD));
    if (!poll_fds)
        return (EXIT_FAILURE);

    /* add the server's port to the list of fd's to poll for */
    poll_fds->fd        = default_server.getSocket().getFileDescriptor();
    poll_fds->events    = ( POLLIN | POLLOUT );
    poll_fds->revents   = 0;

    fds_in_list = 1;
    exit_code = EXIT_SUCCESS;
    for ( ; ; )
    {
        /* let pull set .revents for all pollfd structures that have received events */
        /* TODO poll returns the number of descriptors that are read for I/O. Can be used to optimize the call */
        /* TODO scale the poll_fds size correctly based on new incoming connections for performance */
        if (poll(poll_fds, fds_in_list, POLL_NO_TIMEOUT) == SYS_ERROR)
        {
            exit_code = EXIT_FAILURE;
            break ;
        }

        /* loop all pollfd structures to check .revents */
        PFD *poll_fd = poll_fds;
        for (nfds_t idx = 0; idx < fds_in_list; idx++, poll_fd = &poll_fds[idx])
        {
            /* check for exceptions */
            if (poll_fd->revents & (POLLERR))
            {
                /* do some error checking */
                std::cout << "Some error bit is set for " << poll_fd->fd << '\n';
            }

            /* check for input */
            if (poll_fd->revents & (POLLIN))
            {
                /* if server socket then add new client socket to list of fds to scan */
                if (poll_fd->fd == default_server.getSocket().getFileDescriptor())
                {
                    /* TODO if poll_fds size is too small reallocate */

                    poll_fds[fds_in_list].fd = default_server.acceptNewConnection();
                    poll_fds[fds_in_list].events    = ( POLLIN | POLLOUT );
                    fds_in_list++;      /* increment counter list counter */
                }
                /* data from client connection is ready for reading */
                else
                {
                    if (default_server.handleConnection(poll_fd->fd) == SOCK_ERROR)
                    {
                        /* do some error checking */
                        std::cout << "Failed to handle connection for " << poll_fd->fd << '\n';
                    }
                    close(poll_fds[fds_in_list].fd);
                    poll_fds[fds_in_list].fd = -1;
                }
            }

            /* check for output */
            if (poll_fd->revents & (POLLOUT))
            {

            }

            /* reset .revents */
            poll_fd->revents = 0;
        }
    }

    return (exit_code);
}
