/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.cpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:27:00 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/16 17:03:24 by dgiannop      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request() : m_status(200) {}

Request::Request(const Request &copy)
{
	m_status = copy.m_status;
	m_method = copy.m_method;
	m_target = copy.m_target;
	m_version = copy.m_version;
	m_headers = copy.m_headers;
	m_body = copy.m_body;
}

Request::~Request() {}

Request & Request::operator=(const Request &copy)
{
	if (this != &copy)
	{
		m_status = copy.m_status;
		m_method = copy.m_method;
		m_target = copy.m_target;
		m_version = copy.m_version;
		m_headers = copy.m_headers;
		m_body = copy.m_body;
	}
	return (*this);
}

std::string Request::getRequest()
{
	return (m_request);
}

std::string const&  Request::getMethod() const
{
	return (m_method);
}

std::map<std::string, std::string> Request::getHeaders()
{
	return (m_headers);
}

std::string Request::getBody()
{
	return (m_body);
}

int Request::getStatus()
{
	return (m_status);
}
