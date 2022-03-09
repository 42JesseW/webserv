#include "Request.hpp"

void    Request::errorChecking()
{
    std::string query;

    if (this->getStatus() == 200)
    {
        this->checkStatusLine();
        this->checkHeaders();
    }
}

void    Request::checkStatusLine()
{
    if (m_method.empty() || m_version.empty())
        this->setStatus(400);
    else if (m_method != "GET" && m_method != "POST" && m_method != "DELETE")
        this->setStatus(405);
    else if (m_version != ALLOWED_VERSION)
        this->setStatus(426);
}

void    Request::checkHeaders()
{
    if (m_headers.find("Host") == m_headers.end())
        this->setStatus(400);
    else if (m_headers.find("Expect") != m_headers.end() && m_headers["Expect"] != "100-continue")
        this->setStatus(417);
}
