/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ErrorHandling.cpp                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 13:58:06 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/11 13:50:51 by katherine     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void    Request::errorChecking()
{
    std::string query;

    if (m_status == 200)
    {
        this->checkStatusLine();
        this->checkHeaders();
    }
    if ( getenv( "PATH" ) ) // QUERY_STRING variable exists
        query = std::getenv( "PATH" ); // retrieve QUERY_STRING value
    std::cout << "Query: "<< query << std::endl;
    std::cout << "Status: " << m_status << std::endl;
}

void    Request::checkStatusLine()
{
    if (m_method.empty() || m_target.empty() || m_version.empty())
        m_status = 400;
    else if (m_method != "GET" && m_method != "POST" && m_method != "DELETE")
        m_status = 405;
    else if (m_version != ALLOWED_VERSION)
        m_status = 426;
}

void    Request::checkHeaders()
{
    if (m_headers.find("Host") == m_headers.end())
        m_status = 400;
    else if (m_headers.find("Expect") != m_headers.end() && m_headers["Expect"] != "100-continue")
        m_status = 417;
}
