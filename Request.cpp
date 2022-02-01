/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:27:00 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/01 11:44:15 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : m_method(INVALID) {}

Request::Request(const Request &copy)
{
    m_method = copy.m_method;
    m_start_line = copy.m_start_line;
    m_header_lines = copy.m_header_lines;
    m_body_lines = copy.m_body_lines;
}

Request::~Request() {}

Request & Request::operator=(const Request &copy)
{
    if (this != &copy)
        *this = copy;
    return (*this);
}

e_Method Request::getMethod()
{
    return (m_method);
}

std::string Request::getStartLine()
{
    return (m_start_line);
}

stringMap Request::getHeader()
{
    return (m_header_lines);
}

std::vector<std::string> Request::getBody()
{
    return (m_body_lines);
}
