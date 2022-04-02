#include <unittests.hpp>
#include <ServerSocket.hpp>
#include <ClientSocket.hpp>

#include <map>

#include <sys/poll.h>

#define private public

#include <Connection.hpp>


TEST(ConnectionConstruction, Default) {
    Connection  conn;

    EXPECT_TRUE(!conn.m_sock);
}

// TODO fix these tests plzzz
//TEST(ConnectionChecking, SinglePOLLINAndPOLLOUT) {
//    ServerSocket    server_socket;
//    int             server_port;
//    std::string     server_url;
//
//    ClientSocket    *client_socket;
//    int             client_fd;
//    SA_IN           client_addr;
//
//    std::string     curl_response;
//    struct pollfd   pollfds[1];
//
//    ConfigUtil      *util;
//    Connection      *conn;
//    ConfigUtil::status_code_map_t   error_files;
//
//    util = &ConfigUtil::getHandle();
//    error_files = util->getStatusCodeMap();
//
//    server_port = 8080;
//    server_url = "http://localhost:" + std::to_string(server_port);
//
//    server_socket.init("*", server_port);
//    auto curl_future = std::async(sendEmptyRequest, server_url.c_str(), &curl_response);
//    client_fd = server_socket.accept(client_addr);
//
//    client_socket = new ClientSocket(client_fd, client_addr);
//    conn = new Connection(client_socket);
//
//    pollfds->fd = client_fd;
//    pollfds->events = (POLLIN | POLLOUT);
//    do {
//        /* block until ready to read from ClientSocket */
//        EXPECT_TRUE(poll(pollfds, 1, -1) != SYS_ERROR);
//    }
//    while (!(pollfds->revents & POLLIN));
//
//    conn->readSocket();
//
//    do {
//        /* block until ready to write to ClientSocket */
//        EXPECT_TRUE(poll(pollfds, 1, -1) != SYS_ERROR);
//    }
//    while (!(pollfds->revents & POLLOUT));
//
//    conn->sendResponse(&error_files);
//    conn->close();
//    delete conn;
//    EXPECT_EQ(curl_future.get(), EXIT_SUCCESS);
//}
//
///* TODO test with multiple calls to readSocket() */
//TEST(ConnectionChecking, MultiplePOLLINAndPOLLOUT) {
//    ServerSocket    server_socket;
//    int             server_port;
//    std::string     server_url;
//
//    ClientSocket    *client_socket;
//    int             client_fd;
//    SA_IN           client_addr;
//
//    std::string     curl_response;
//    struct pollfd   pollfds[1];
//
//    ConfigUtil      *util;
//    Connection      *conn;
//    ConfigUtil::status_code_map_t   error_files;
//
//    util = &ConfigUtil::getHandle();
//    error_files = util->getStatusCodeMap();
//
//    server_port = 8080;
//    server_url = "http://localhost:" + std::to_string(server_port);
//
//    server_socket.init("*", server_port);
//    auto curl_future = std::async(sendBigRequest, server_url.c_str(), &curl_response);
//    client_fd = server_socket.accept(client_addr);
//
//    client_socket = new ClientSocket(client_fd, client_addr);
//    conn = new Connection(client_socket);
//
//    pollfds->fd = client_fd;
//    pollfds->events = (POLLIN | POLLOUT);
//    do {
//        /* block until ready to read from ClientSocket */
//        EXPECT_TRUE(poll(pollfds, 1, -1) != SYS_ERROR);
//    }
//    while (!(pollfds->revents & POLLIN));
//
//    conn->readSocket();
//
//    do {
//        /* block until ready to read from ClientSocket */
//        EXPECT_TRUE(poll(pollfds, 1, -1) != SYS_ERROR);
//    }
//    while (!(pollfds->revents & POLLIN));
//
//    conn->readSocket();
//
//    do {
//        /* block until ready to write to ClientSocket */
//        EXPECT_TRUE(poll(pollfds, 1, -1) != SYS_ERROR);
//    }
//    while (!(pollfds->revents & POLLOUT));
//
//    conn->sendResponse(&error_files);
//    conn->close();
//    delete conn;
//    EXPECT_EQ(curl_future.get(), EXIT_SUCCESS);
//}