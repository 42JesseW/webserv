/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ParseRequest.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: kfu <kfu@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 14:06:29 by kfu           #+#    #+#                 */
/*   Updated: 2022/02/16 12:56:57 by dgiannop      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::handleRequest(int client_socket)
{
	char    buffer[BUFF_SIZE];
	int     bytes_read = 0;

	while ((bytes_read = recv(client_socket, buffer, BUFF_SIZE, MSG_DONTWAIT)) > 0)
	{
		m_request.append(buffer, bytes_read);
		std::memset(buffer, 0, BUFF_SIZE);
	}
	if (bytes_read == -1)
		m_status = 400;
	divideRequest();
	errorChecking();
}

void Request::divideRequest()
{
	this->parseAndSetStartLine();
	this->parseAndSetHeaders();
	if (!m_request.empty())
	{
		m_request.erase(0, 2);
		m_body = m_request;
	}
	m_request.clear();
}

void Request::parseAndSetStartLine()
{
	m_method = m_request.substr(0, m_request.find(' '));
	m_request.erase(0,  m_request.find(' ') + 1);
	m_request.erase(0,  m_request.find_first_not_of(' '));

	m_target = m_request.substr(0, m_request.find(' '));

	if (m_target.find('?') != std::string::npos)
	{
		m_query = m_target.substr(m_target.find('?') + 1, m_target.find(' '));
		m_target = m_target.substr(0, m_target.find('?'));
	}
	m_request.erase(0,  m_request.find(' ') + 1);
	m_request.erase(0,  m_request.find_first_not_of(' '));
	
	m_version = m_request.substr(0, m_request.find(CR));
	m_request.erase(0,  m_request.find(LF) + 1);
}

void Request::parseAndSetHeaders()
{
	std::string token;
	std::string key;
	std::string value;

	// CR (\r) = ascii 13
	// LF (\n) = ascii 10

	while (m_request[0] != 13 && !m_request.empty())
	{
		token = m_request.substr(0, m_request.find(LF));
		key = m_request.substr(0, m_request.find(':'));
		m_request.erase(0, m_request.find(':') + 2);
		value = m_request.substr(0, m_request.find(CR));
		m_headers.insert(std::pair<std::string, std::string>(key, value));
		m_request.erase(0, m_request.find(LF) + 1);
	}

	// Set host and ports
	std::string	host = m_headers["Host"];
	if (host.find(':') != std::string::npos)
	{
		host = host.substr(host.find(':') + 1, host.length() - 1);
		std::istringstream(host) >> m_port;
	}
}

void Request::printRequest()
{
	std::cout << "------------------ START LINE ------------------" << std::endl;
	std::cout << "Method: " << m_method << std::endl;
	std::cout << "Target: " << m_target << std::endl;
	std::cout << "Query: " << m_query << std::endl;
	std::cout << "Version: " << m_version << std::endl;

	std::cout << "------------------ HEADERS ------------------" << std::endl;
	std::cout << "Port: " << m_port << std::endl;
	for (auto it = m_headers.begin(); it != m_headers.end(); ++it)
		std::cout << "{" << it->first << "} {" << it->second << "}" << std::endl;

	std::cout << "------------------ BODY ------------------" << std::endl;
	std::cout << m_body << std::endl;
	std::cout << "------------------ END ------------------" << std::endl;
}
