#include "../includes/Request.hpp"

Request::Request() : m_method(INVALID) {}

Request::Request(const Request &copy)
{
    m_method = copy.m_method;
    m_start_line = copy.m_start_line;
    m_headers = copy.m_headers;
    m_body = copy.m_body;
}

Request::~Request() {}

Request & Request::operator=(const Request &copy)
{
    if (this != &copy)
    {
        m_method = copy.m_method;
        m_start_line = copy.m_start_line;
        m_headers = copy.m_headers;
        m_body = copy.m_body;
    }
    return (*this);
}

std::string Request::getRequest()
{
    return (m_request);
}

e_Method Request::getMethod()
{
    return (m_method);
}

std::string Request::getStartLine()
{
    return (m_start_line);
}

std::map<std::string, std::string> Request::getHeaders()
{
    return (m_headers);
}

std::string Request::getBody()
{
    return (m_body);
}
