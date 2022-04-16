#include <Connection.hpp>

// TODO these constructors and destructors SUCK
Connection::Connection(void) : m_sock(NULL), m_route(NULL)
{

}

Connection::Connection(const Connection &cpy) : m_sock(NULL), m_route(NULL)
{
    *this = cpy;
}

Connection::Connection(ClientSocket *sock) : m_sock(sock)
{

}

Connection::~Connection(void)
{
    if (m_sock)
    {
        std::cout << "[DEBUG] Closing client connection" << '\n';
        delete m_sock;
    }
}

Connection&     Connection::operator = (const Connection &conn)
{
    if (this != &conn)
    {
        delete m_sock;
        m_sock = NULL;
        if (conn.m_sock)
            m_sock = new ClientSocket(*conn.m_sock);
        delete m_route;
        if (conn.m_route)
            m_route = new Route(*conn.m_route);
        m_request = conn.m_request;
    }
    return (*this);
}

/*
 * ClientSocket being passed must be initialised before being
 * added to the Connection object.
 */
void            Connection::setSocket(ClientSocket *sock)
{
    m_sock = sock;
}

Request&       Connection::getRequest(void)
{
    return (m_request);
}

void            Connection::readSocket(void)
{
    char    *request_data;
    ssize_t bytes_read = 0;;

    if (!m_sock)
        throw NoSocketFail();
    request_data = m_sock->recv(&bytes_read);
    if (!request_data)
    {
        /* must be handled from outside */
        m_request.setStatus(HTTP_STATUS_INTERNAL_SERVER_ERROR);
        return ;
    }

    std::cout << "[DEBUG] Read data from socket " << m_sock->getFd() << ":\n";
    // std::cout << request_data;

    m_request.appendRequestData(request_data, bytes_read);
    delete [] request_data;
}

void            Connection::parseRequest(void)
{
    std::cout << "[DEBUG] Building request object\n" << '\n';
    if (!m_request.m_request.empty())
        m_request.parse();
    else
        m_request.setStatus(HTTP_STATUS_NO_CONTENT);
}

void            Connection::sendResponse(ConfigUtil::status_code_map_t *error_files)
{
    Response   *response;

    if (m_request.getStatus() == HTTP_STATUS_NO_CONTENT)
        return ;

    checkRoute();

    
    if (m_request.getStatus() >= HTTP_STATUS_NOT_FOUND && m_request.getStatus() <= HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED) 
        response = new Response(m_request);
    else
        response = new Response(m_request, *m_route);
    response->buildResponse(*error_files);

    m_sock->send(response->getResponse().c_str());
    delete response;
}

void            Connection::close(void)
{
    if (!m_sock)
        throw NoSocketFail();
    m_sock->close();
}

void            Connection::setRoute(Route *route)
{
    m_route = route;
}

void Connection::checkRoute(void)
{
    checkAcceptedMethods();
    checkRedirects();
    checkFileSearchPath();
}

void Connection::checkAcceptedMethods(void)
{
    std::vector<std::string>::iterator it;

    if (m_request.getStatus() == HTTP_STATUS_OK)
    {
        it = m_route->getAcceptedMethods().begin();
        for ( ; it != m_route->getAcceptedMethods().end(); it++)
        {
            if (*it == m_request.getMethod())
            {
                return ;
            }
        }
        m_request.setStatus(HTTP_STATUS_METHOD_NOT_ALLOWED);
    }
}

void Connection::checkRedirects(void)
{
    if (m_request.getStatus() == HTTP_STATUS_OK)
    {
        if (m_route->getRedir() != NULL)
        {
            m_request.setStatus(m_route->getRedir()->status_code);
        }
    }
}

void Connection::checkFileSearchPath(void)
{
    if (m_route)
        m_request.setFilesearchPath(m_route->getFileSearchPath());
    if (m_request.getStatus() == HTTP_STATUS_OK)
    {
        if (m_request.getFilename().empty())
        {
            searchDefaultIndexPages();
        }
        else
        {
            if (!searchCGIExtensions())
            {
                searchFile();
            }
        }
    }
}

void Connection::searchFile(void)
{
    std::string filepath;

    filepath.append(m_route->getFileSearchPath() + m_request.getFilename());
    if (open(filepath.c_str(), O_RDONLY) != -1)
    {
        m_request.setStatus(HTTP_STATUS_OK);
    }
    else
    {
        m_request.setStatus(HTTP_STATUS_NOT_FOUND);
    }
}

void Connection::searchDefaultIndexPages(void)
{
    std::string filepath;
    std::vector<std::string>::iterator it;

    for(it = m_route->getIndexFiles().begin(); it != m_route->getIndexFiles().end(); it++)
    {
        filepath.append(m_request.m_filesearch + *it);
        if (open(filepath.c_str(), O_RDONLY) != -1)
        {
            m_request.setStatus(HTTP_STATUS_OK);
            return ;
        }
        filepath.clear();
    }
    m_request.setStatus(HTTP_STATUS_NOT_FOUND);
}

bool Connection::searchCGIExtensions(void)
{
    std::string extension = m_request.getFilename().substr(m_request.getFilename().find('.'));
    std::vector<std::string>::iterator it;

    for(it = m_route->getCgiFileExtensions().begin(); it != m_route->getCgiFileExtensions().end(); it++)
    {
        if (extension == *it)
        {
            m_request.setStatus(HTTP_STATUS_OK);
            return (true);
        }
    }
    m_request.setStatus(HTTP_STATUS_NOT_FOUND);
    return (false);
}
