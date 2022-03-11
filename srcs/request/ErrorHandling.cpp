#include "Request.hpp"

void    Request::errorChecking()
{
    std::string query;

    if (getStatus() == 200)
    {
        checkStatusLine();
        checkHeaders();
    }
}

void    Request::checkStatusLine()
{
    if (m_method.empty() || m_version.empty())
        setStatus(400);
    else if (m_method != "GET" && m_method != "POST" && m_method != "DELETE")
        setStatus(405);
    else if (m_version != ALLOWED_VERSION)
        setStatus(426);
}

void    Request::checkHeaders()
{
    if (m_headers.find("Host") == m_headers.end())
        setStatus(400);
    else if (m_headers.find("Expect") != m_headers.end() && m_headers["Expect"] != "100-continue")
        setStatus(417);
}
