#include "Request.hpp"

void    Request::errorChecking()
{
    std::string query;

    if (getStatus() == HTTP_STATUS_OK)
    {
        checkStatusLine();
        checkHeaders();
    }
}

void    Request::checkStatusLine()
{
    if (m_method.empty() || m_version.empty())
    {
        setStatus(HTTP_STATUS_BAD_REQUEST);
    }
    else if (m_method != "GET" && m_method != "POST" && m_method != "DELETE")
    {
        setStatus(HTTP_STATUS_METHOD_NOT_ALLOWED);
    }
    else if (m_version != ALLOWED_VERSION)
    {
        setStatus(HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED);
    }
}

void    Request::checkHeaders()
{
    if (m_headers.find("Host") == m_headers.end())
    {
        setStatus(HTTP_STATUS_BAD_REQUEST);
    }
    else if (m_headers.find("Expect") != m_headers.end() && m_headers["Expect"] != "100-continue")
    {
        setStatus(HTTP_STATUS_EXPECTATION_FAILED);
    }
}
