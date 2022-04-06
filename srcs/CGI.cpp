#include <CGI.hpp>

CGI::CGI() :
    m_program_path(DFL_CGI_DIR "/" DFL_CGI_PROG),
    m_argv(NULL),
    m_envp(NULL),
    m_fork_pid(UNSET_PID)
{
    std::memset(m_pipe_in, 0, sizeof(m_pipe_in));
    std::memset(m_pipe_out, 0, sizeof(m_pipe_out));
}

CGI::CGI(const CGI &cpy)
{
    *this = cpy;
}

CGI::~CGI()
{
    ft::freeCharArr(&m_envp);
    ft::freeCharArr(&m_argv);
}

CGI&                CGI::operator=(const CGI &rhs)
{
    if (this != &rhs)
    {
        m_program_path = rhs.m_program_path;
        m_environ = rhs.m_environ;
        ft::freeCharArr(&m_envp);
        m_envp = _environToEnvp();
        m_args = rhs.m_args;
        ft::freeCharArr(&m_argv);
        m_argv = _argsToArgv();
        m_pipe_in[0] = rhs.m_pipe_in[0];
        m_pipe_in[1] = rhs.m_pipe_in[1];
        m_pipe_out[0] = rhs.m_pipe_out[0];
        m_pipe_out[1] = rhs.m_pipe_out[1];
    }
    return (*this);
}

void                CGI::setProgramPath(const std::string& path)
{
    m_program_path = path;
}

pid_t&              CGI::getForkedPid(void)
{
    return (m_fork_pid);
}

int&                CGI::getPipeReadFd(void)
{
    return (m_pipe_out[0]);
}

/*
 * https://en.wikipedia.org/wiki/Common_Gateway_Interface
 *
 *  Server specific variables:
 *      SERVER_SOFTWARE:    name/version of HTTP server.
 *      SERVER_NAME:        host name of the server, may be dot-decimal IP address.
 *      GATEWAY_INTERFACE:  CGI/version.
 *  Request specific variables:
 *      SERVER_PROTOCOL:    HTTP/version.
 *      SERVER_PORT:        TCP port (decimal).
 *      REQUEST_METHOD:     name of HTTP method (see above).
 *      PATH_INFO:          path suffix, if appended to URL after program name and a slash.
 *      PATH_TRANSLATED:    corresponding full path as supposed by server, if PATH_INFO is present.
 *      SCRIPT_NAME:        relative path to the program, like /cgi-bin/script.cgi.
 *      QUERY_STRING:       the part of URL after ? character. The query string may be composed of
 *                          *name=value pairs separated with ampersands (such as var1=val1&var2=val2...)
 *                          when used to submit form data transferred via GET method as defined by HTML
 *                          application/x-www-form-urlencoded.
 *      REMOTE_HOST:        host name of the client, unset if server did not perform such lookup.
 *      REMOTE_ADDR:        IP address of the client (dot-decimal).
 *      AUTH_TYPE:          identification type, if applicable.
 *      REMOTE_USER:        used for certain AUTH_TYPEs.
 *      REMOTE_IDENT:       see ident, only if server performed such lookup.
 *      CONTENT_TYPE:       Internet media type of input data if PUT or POST method are used, as provided
 *                          via HTTP header.
 *      CONTENT_LENGTH:     similarly, size of input data (decimal, in octets) if provided via HTTP header.
 *
 *      Variables passed by user agent (HTTP_ACCEPT, HTTP_ACCEPT_LANGUAGE, HTTP_USER_AGENT, HTTP_COOKIE and
 *      possibly others) contain values of corresponding HTTP headers and therefore have the same sense.
 */
void                CGI::init(SimpleRequest& request)
{
    /* set environment variables for the CGI request */
    m_environ["SERVER_SOFTWARE"] = PROG_NAME;
    m_environ["SERVER_NAME"] = ""; // TODO Use Server.m_names[0] or request.headers.host
    m_environ["GATEWAY_INTERFACE"] = CGI_VERSION;

    m_environ["SERVER_PROTOCOL"] = request.getHttpVersion();
    m_environ["SERVER_PORT"] = ""; // TODO Use server.m_socket.port
    m_environ["REQUEST_METHOD"] = request.getMethod();
    m_environ["PATH_INFO"] = request.getPath();
    m_environ["PATH_TRANSLATED"] = ""; // TODO build a full path using program PWD
    m_environ["SCRIPT_NAME"] = DFL_CGI_PROG; // TODO can be from a custom directive in config file ?
    m_environ["QUERY_STRING"] = request.getQuery();
    m_environ["REMOTE_HOST"] = "";      // TODO use getnameinfo()?
    m_environ["REMOTE_ADDR"] = "";      // TODO use inet_ntoa()?
    // TODO could also use std::map.at which throws exception std::out_of_range
    m_environ["CONTENT_TYPE"] = _getMapValueWithDefault(
            request.getHeaders(),
            std::string("Content-Type"),
            std::string("text/html"));
    m_environ["CONTENT_LENGTH"] = _getMapValueWithDefault(
            request.getHeaders(),
            std::string("Content-Length"),
            std::to_string(request.getBody().size())
    );

    m_envp = _environToEnvp();                                          // TODO testcase
    if (!m_envp)
        throw std::runtime_error("Failed to allocate for GGI::m_envp");

    m_args.push_back(m_program_path);                                   // TODO extra arguments must be passed here
    m_argv = _argsToArgv();
    if (!m_argv)
        throw std::runtime_error("Failed to allocate for CGI::m_argv");

    /* create the two pipes needed for .exec and make output pipe's read part `non_blocking` */
    if (pipe(m_pipe_in) == SYS_ERROR || pipe(m_pipe_out) == SYS_ERROR)  // TODO testcase
        throw std::runtime_error("Failed to create pipes for CGI object");
    fcntl(m_pipe_out[0], F_SETFL, O_NONBLOCK);

    m_request_body = request.getBody();
}

/*
 * The forked process will use execve to run the CGI program.
 * If the request body has a size large than MAX_PIPE_SIZE, then
 * a file will be used instead since this has no performance impact.
 *
 * https://unix.stackexchange.com/questions/11946/how-big-is-the-pipe-buffer
 *
 * TODO implement different implementation with request_body size
 */
int                 CGI::exec(void)
{
    pid_t   pid;

    if (m_pipe_in[0] == 0 || m_pipe_in[1] == 0)
        throw std::invalid_argument("CGI class not initialised");

    if ((pid = fork()) == SYS_ERROR)
        return (SYS_ERROR);

    if (pid == 0)
    {

        /* use dup to make sure the CGI program reads from the pipe */
        if (dup2(m_pipe_in[0], STDIN_FILENO) == SYS_ERROR)
            _execExitFail();

        /* write request body to write part of the pipe so CGI can read it */
        if (write(m_pipe_in[1], m_request_body.c_str(), m_request_body.size()) == SYS_ERROR)
            _execExitFail();
        close(m_pipe_in[1]);

        /* use dup to make sure the CGI program writes to the out pipe instead of STDOUT */
        if (dup2(m_pipe_out[1], STDOUT_FILENO) == SYS_ERROR)
            _execExitFail();

        /* call the CGI program using provided arguments. This call will take over child process */
        if (execve(m_program_path.c_str(), m_argv, m_envp) == SYS_ERROR)
        {
            /*
             * write the error description to the write end of the out pipe
             * so that the parent still gets the POLL_IN notification from poll.
             * exit(EXIT_FAILURE) should tell the parent process that it failed
             */
            _execExitFail();
        }
    }
    else
    {
        m_fork_pid = pid;
    }
    return (EXIT_SUCCESS);
}

void                CGI::_execExitFail(void)
{
    /* errno can be check after a dup (not after read or write) */
    std::string err(strerror(errno));

    write(m_pipe_out[1], err.c_str(), err.size());
    ft::freeCharArr(&m_envp);
    ft::freeCharArr(&m_argv);
    exit(EXIT_FAILURE);
}

void                CGI::reset(void)
{
    m_program_path.clear();
    m_request_body.clear();
    ft::freeCharArr(&m_envp);
    ft::freeCharArr(&m_argv);
    m_environ.clear();
    m_fork_pid = UNSET_PID;
    std::memset(m_pipe_in, 0, sizeof(m_pipe_in));
    std::memset(m_pipe_out, 0, sizeof(m_pipe_out));
}

char                **CGI::_environToEnvp(void)
{
    int             idx;
    env_t::iterator it;
    std::string     tmp;
    char            **envp;

    envp = new(std::nothrow) char*[m_environ.size() + 1];
    if (!envp)
        return (NULL);
    it = m_environ.begin();
    for (idx = 0; it != m_environ.end(); ++it, ++idx)
    {
        tmp = it->first + "=" + it->second;
        envp[idx] = ft::strdup(tmp.c_str());
        if (!envp[idx])
        {
            while (idx > 0)
            {
                delete envp[idx];
                --idx;
            }
            delete [] envp;
            return (NULL);
        }
    }
    envp[idx] = NULL;
    return (envp);
}

char                **CGI::_argsToArgv(void)
{
    size_t  idx;
    char    **argv;

    argv = new(std::nothrow) char*[m_args.size() + 1];
    if (!argv)
        return (NULL);
    for (idx = 0; idx < m_args.size(); ++idx)
    {
        argv[idx] = ft::strdup(m_args[idx].c_str());
        if (!argv[idx])
        {
            while (idx > 0)
            {
                delete argv[idx];
                --idx;
            }
            delete [] argv;
            return (NULL);
        }
    }
    argv[idx] = NULL;
    return (argv);
}

SimpleRequest::SimpleRequest()
{

}

SimpleRequest::SimpleRequest(const SimpleRequest &cpy)
{
    *this = cpy;
}

SimpleRequest::~SimpleRequest()
{

}

SimpleRequest&      SimpleRequest::operator = (const SimpleRequest &rhs)
{
    if (this != &rhs)
    {
        m_method = rhs.m_method;
        m_uri = rhs.m_uri;
        m_http_version = rhs.m_http_version;
        m_headers = rhs.m_headers;
        m_body = rhs.m_body;
    }
    return (*this);
}

void                SimpleRequest::parse(const char *request)
{
    std::string header_name, header_value;
    std::string request_s(request);
    size_t      pos;
    size_t      uri_pos;
    size_t      header_pos;

    if ((pos = request_s.find(' ')) == std::string::npos)
        throw std::invalid_argument("Parse fail");
    m_method = request_s.substr(0, pos);

    request_s.erase(0, pos + 1);
    if ((pos = request_s.find(' ')) == std::string::npos)
        throw std::invalid_argument("Parse fail");

    m_uri = request_s.substr(0, pos);
    if ((uri_pos = m_uri.find('?')) != std::string::npos)
    {
        m_query = m_uri.substr(uri_pos + 1, std::string::npos);
        m_uri = m_uri.substr(0, uri_pos);
    }
    request_s.erase(0, pos + 1);

    if ((pos = request_s.find(*CR)) == std::string::npos)
        throw std::invalid_argument("Parse fail");
    m_http_version = request_s.substr(0, pos);
    if (m_http_version != HTTP_VERSION)
        throw std::invalid_argument("Invalid HTPT version " + m_http_version);
    request_s.erase(0, pos);
    request_s.erase(0, request_s.find_first_not_of(CR LF));

//    std::cout << "REQUEST LINE\n" << m_method << " | " << m_uri << " | " << m_query << " | " << m_http_version << "\n\n";
    while (!request_s.empty() && request_s.find(CR LF) != 0)
    {
        if ((pos = request_s.find(':')) == std::string::npos)
            throw std::invalid_argument("Parse fail");
        header_name = request_s.substr(0, pos);
        header_pos = request_s.find((*LF));
        /* +2 to avoid adding the extra space after ':' and -3 since we don't want the \n */
        header_value = request_s.substr(pos + 2, header_pos - pos - 3);
        request_s.erase(0, header_pos + 1);
//        std::cout << "header -> " << header_name << ": " << header_value << '\n';
        m_headers.insert(std::make_pair(header_name, header_value));
    }

    if (!request_s.empty())
    {
        /* remove CR LF */
        request_s.erase(0, 2);
        m_body = request_s;
    }
//    std::cout << "\nBODY\n" << m_body << '\n';
}

std::string&                SimpleRequest::getMethod(void)
{
    return (m_method);
}

std::string&                SimpleRequest::getUri(void)
{
    return (m_uri);
}

std::string&                SimpleRequest::getPath(void)
{
    return (m_path);
}

std::string&                SimpleRequest::getQuery(void)
{
    return (m_query);
}

std::string&                SimpleRequest::getHttpVersion(void)
{
    return (m_http_version);
}

SimpleRequest::headers_t&   SimpleRequest::getHeaders(void)
{
    return (m_headers);
}

std::string&                SimpleRequest::getBody(void)
{
    return (m_body);
}
