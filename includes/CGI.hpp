#ifndef CGI_HPP

# define CGI_HPP

# define CR "\r"
# define LF "\n"
# define HTTP_VERSION   "HTTP/1.1"
# define CGI_VERSION    "CGI/1.1"
# define PROG_VERSION   "Webserv/1.0"

# define DFL_CGI_DIR    "cgi-bin"
# define DFL_CGI_PROG   "php-cgi"

# define UNSET_PID      (-1)
# define MAX_PIPE_SIZE  65536

# include <Common.hpp>
# include <Utils.hpp>

# include <unistd.h>

# include <iostream>

class SimpleRequest
{
private:
    typedef std::map<std::string, std::string>  headers_t;

private:
    std::string     m_method;

    /* example: http://example.com/cgi-bin/printenv.pl/foo/bar?var1=value1&var2=with%20percent%20encoding */
    std::string     m_uri;      // cgi-bin/printenv.pl
    std::string     m_path;     // /foo/bar <-- TODO
    std::string     m_query;    // var1=value1&var2=with%20percent%20encoding

    std::string     m_http_version;
    headers_t       m_headers;
    std::string     m_body;

public:
    SimpleRequest();
    SimpleRequest(const SimpleRequest& cpy);
    ~SimpleRequest();

    SimpleRequest   &operator=(const SimpleRequest& rhs);

    void            parse(const char *request);

    std::string&    getMethod(void);
    std::string&    getUri(void);
    std::string&    getPath(void);
    std::string&    getQuery(void);
    std::string&    getHttpVersion(void);
    headers_t&      getHeaders(void);
    std::string&    getBody(void);

};

/*
 * Encapsulates all operations and data related to
 * a Common Gateway Interface program used by the
 * webserver.
 */
class CGI
{
private:
    typedef std::map<std::string, std::string>  env_t;

private:
    /* program path should be checked at exec stage */
    std::string     m_program_path;
    std::string     m_request_body;
    env_t           m_environ;
    char            **m_envp;

    /*
     * m_pipe_out[0] must be added to the list of `pollfd`
     * that go through poll. Because a different type of
     * operation on the fd is needed for the `listener fd`
     * or the `client connection fd`.
     *
     * The forked process's pid is stored so that after
     * poll sets POLL_IN on m_pipe_out[0] the parent
     * process can collect the child process's exit
     * information.
     */
    pid_t           m_fork_pid;
    int             m_pipe_in[2];
    int             m_pipe_out[2];

public:
    CGI(void);
    CGI(const CGI& cpy);
    ~CGI();

    CGI&            operator = (const CGI& rhs);

    void            setProgramPath(const std::string& path);

    /*
     * initialize the CGI structure using the Request object
     */
    void            init(SimpleRequest& request);

    /*
     * Creates the file descriptor that points to the read end of
     * the pipe used to pipe the CGI program's output back to the
     * webserver.
     *
     * This file descriptor must be added the Client object and has
     * to go through poll as to know when it is ready for reading since
     * we don't know how long the CGI will take to process the request.
     */
    int             exec(void);

private:
    void            _resetInstance(void);
    void            _resetInstanceEnvp(int idx);

    char            **_environToEnvp(void);
    void            _execExitFail(void);

    /* get a value from a map but with a default value if it does not exist */
    template < class Key, class Value >
    Value           _getMapValueWithDefault(std::map<Key, Value>& map, const Key& key, const Value& value)
    {
        typename std::map<Key, Value>::const_iterator   it( map.find(key) );

        return ((it != map.end()) ? it->second : value );
    }

};

#endif
