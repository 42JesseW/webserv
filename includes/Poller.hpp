#pragma once

#include <Config.hpp>

#include <stack>

#include <sys/poll.h>

#define POLL_TIMEOUT_MS     500
#define POLLIN_SLEEP_MS     2000

class Poller
{
public:
    typedef std::vector<struct pollfd>      pollfd_t;
    typedef std::map<int, Connection*>      clients_t;

private:
    pollfd_t                    m_pfds;
    clients_t                   m_clients;
    PortConfig                  *m_port_config;

    std::stack<int>             m_dropped_fds;
    std::pair<int, Connection*> m_new_connection;

private:
    Poller(void);

public:
    explicit Poller(PortConfig *port_config);
    Poller(const Poller& cpy);
    ~Poller(void);

    Poller&     operator = (const Poller& rhs);

    static void *pollPort(void *instance);

private:
    static Poller      *_initPoller(PortConfig **port_config, void *instance);

    void                _getNewConnection(void);
    void                _readConnectionData(int& socket_fd);
    void                _parseAndRespond(int& socket_fd);

    void                _updatePollFds(void);

};
