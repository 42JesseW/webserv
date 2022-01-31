/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.cpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:04:54 by katherine     #+#    #+#                 */
/*   Updated: 2022/01/31 12:35:48 by katherine     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response() : m_method(INVALID) {}

Response::Response(const Response &copy)
{
	m_method = Copy.m_method;
	m_status_codes = Copy.m_status_codes;
	m_start_line = Copy.m_start_line;
	m_header_lines = Copy.m_header_lines;
	m_body_lines = Copy.m_body_lines;
	m_response_file = Copy.m_response_file;
}

Response::~Response()
{
	if (m_response_file.is_open())
		m_response_file.close();
}

Response & Response::operator=(const Response &copy)
{
	if (this != &Copy)
		*this = Copy;
	return (*this);
}

e_Method Response::getMethod()
{
	return (m_method);
}

std::ofstream Response::getResponseFile()
{
	return (m_response_file);
}

void Response::writeToResponseFile(std::string content)
{
	m_response_file << content;
}

void Response::writeToResponseFile(std::vector<std::string> content)
{
	std::vector<std::string>::iterator vector_iterator;
	for (vector_iterator = content.begin(); vector_iterator != content.end(); vector_iterator++)
	{
		m_response_file << *vector_iterator;
	}
}