/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:27:00 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/03 11:31:09 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

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
        *this = copy;
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
