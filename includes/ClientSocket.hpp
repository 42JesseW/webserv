#pragma once

#include <Socket.hpp>
#define RECV_SIZE   2048

/*
 * Used to encapsulate the socket data retrieved
 * from the call to accept(). The following steps
 * can be followed to go through Request -> Response:
 * 1. char *buff = recv()           : Request
 * 2. send(<some response data>)    : Response
 */
class ClientSocket : public Socket
{
private:
    /*
     * client sockets may only be constructed from a
     * call to accept().
     */
    ClientSocket();

public:
    ClientSocket(const ClientSocket& cpy);
    ClientSocket(int fd, SA_IN& address);
    ~ClientSocket();

    ClientSocket&   operator = (const ClientSocket& sock);

    char            *recv(void);
    void            send(const char *response);

public:
    class SendFail : std::exception { };

};
