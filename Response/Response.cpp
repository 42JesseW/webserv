/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:04:54 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/01 11:44:42 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : m_method(INVALID) {}

Response::Response(const Response &copy)
{
	m_method = copy.m_method;
	m_status_codes = copy.m_status_codes;
	m_start_line = copy.m_start_line;
	m_header_lines = copy.m_header_lines;
	m_body_lines = copy.m_body_lines;
	m_response = copy.m_response;
}

Response::~Response() {}

Response & Response::operator=(const Response &copy)
{
	if (this != &copy)
		*this = copy;
	return (*this);
}

e_Method Response::getMethod()
{
	return (m_method);
}

std::string Response::getResponse()
{
	return (m_response);
}
