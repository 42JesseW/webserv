#include <Request.hpp>

Request::Request(void) :
    m_target(DFL_TARGET),
    m_filename(DFL_FILENAME),
    m_status(HTTP_STATUS_OK),
    m_port(80),
    m_done(false)
{

}

Request::Request(const Request &copy)
{
    m_status = copy.m_status;
    m_target = copy.m_target;
    m_query = copy.m_query;
    m_method = copy.m_method;
    m_version = copy.m_version;
    m_request = copy.m_request;
    m_headers = copy.m_headers;
    m_port = copy.m_port;
    m_body = copy.m_body;
    m_done = copy.m_done;
    m_filename = copy.m_filename;
    m_filesearch = copy.m_filesearch;
}

Request::~Request(void)
{

}

Request & Request::operator = (const Request &copy)
{
    if (this != &copy)
    {
        m_status = copy.m_status;
        m_target = copy.m_target;
        m_query = copy.m_query;
        m_method = copy.m_method;
        m_version = copy.m_version;
        m_request = copy.m_request;
        m_headers = copy.m_headers;
        m_port = copy.m_port;
        m_body = copy.m_body;
        m_done = copy.m_done;
        m_filename = copy.m_filename;
        m_filesearch = copy.m_filesearch;
    }
    return (*this);
}

void        Request::parse(void)
{
    headers_t::iterator it;

    parseAndSetStartLine();
    parseAndSetHeaders();
    if (!m_request.empty())
    {
        m_request.erase(0, 2);
        m_body = m_request;
    }
    it = getHeaders().find("Transfer-Encoding");
    if (it != getHeaders().end() && it->second == "chunked")
    {
        decodeRequest();
    }
    m_request.clear();
}

void        Request::appendRequestData(const char *data, ssize_t bytes_read)
{
    m_request.append(data, bytes_read);
}

void Request::setFilesearchPath(std::string path)
{
    m_filesearch = path;
    if (m_filesearch.at(m_filesearch.length() - 1) != '/')
    {
        m_filesearch.append("/");
    }
}

void Request::setStatus(int status)
{
    m_status = status;
}

void Request::setDone(bool status)
{
    m_done = status;
}

int	&Request::getStatus(void)
{
    return (m_status);
}

std::string	&Request::getTarget(void)
{
    return (m_target);
}

std::string	&Request::getQuery(void)
{
    return (m_query);
}

std::string	&Request::getMethod(void)
{
    return (m_method);
}

std::string	&Request::getVersion(void)
{
    return (m_version);
}

std::string &Request::getFilename(void)
{
    return (m_filename);
}

std::string &Request::getCGIPath(void)
{
    return (m_cgi_path);
}

int	&Request::getPort(void)
{
    return (m_port);
}

bool &Request::isDone(void)
{
    return (m_done);
}

std::map<std::string, std::string>	&Request::getHeaders(void)
{
    return (m_headers);
}

std::string	&Request::getBody(void)
{
    return (m_body);
}

void	Request::resetRequest(void)
{
    m_target = DFL_TARGET;
    m_filename = DFL_FILENAME;
    m_filename.clear();
    m_query.clear();
    m_headers.clear();
    m_body.clear();
    m_done = false;
    m_cgi_path.clear();
    m_status = HTTP_STATUS_OK;
    m_method.clear();
    m_version.clear();
    m_request.clear();
    m_port = 80;
}
