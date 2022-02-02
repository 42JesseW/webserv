/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ParseRequest.cpp                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: kfu <kfu@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/02 14:06:29 by kfu           #+#    #+#                 */
/*   Updated: 2022/02/02 17:55:59 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void Request::copyRequest(int socket)
{
    char    buffer[BUFF_SIZE];
    int     bytes_read;

    while ((bytes_read = read(socket, buffer, BUFF_SIZE)) > 0)
    {
        m_request.append(buffer, bytes_read);
        std::memset(buffer, 0, BUFF_SIZE);
    }
    divideRequest();
}

void Request::divideRequest()
{
    this->parseAndSetStartLine();
    this->parseAndSetHeader();
    if (!m_request.empty())
    {
        // Remove the enter after the header
        m_request.erase(0, 1);
        m_body = m_request;
    }
    m_request.clear();
}

void Request::parseAndSetStartLine()
{
    std::string token = m_request.substr(0, m_request.find(ENDLINE));
    m_start_line = token;
    m_request.erase(0,  m_request.find(ENDLINE) + 1);
}

void Request::parseAndSetHeader()
{
    std::string token;
    std::string key;
    std::string value;

    // CR (Carriage return) = ascii 13
    // LF (Line feed) = ascii 10

    while (m_request[0] != 10 && !m_request.empty())
    {
        // Putting a line untill \n into token
        token = m_request.substr(0, m_request.find(ENDLINE));

        // Setting the key value and then deleting it from m_request
        key = m_request.substr(0, m_request.find(':'));
        m_request.erase(0, m_request.find(':') + 2);
         
        // Setting the value of the key
        value = m_request.substr(0, m_request.find(ENDLINE));
        
        // Insert the pair into the map and then deleting it from m_request
        m_header.insert(std::pair<std::string, std::string>(key, value));
        m_request.erase(0, m_request.find(ENDLINE) + 1);
    }
}

void Request::printRequest()
{
    std::cout << "------------------ START LINE ------------------" << std::endl;
    std::cout << m_start_line << "\n" << std::endl;
    
    std::cout << "------------------ HEADERS ------------------" << std::endl;
    for (auto it = m_header.begin(); it != m_header.end(); ++it)
        std::cout << "{" << it->first << "} {" << it->second << "}\n";
    std::cout << std::endl;

    std::cout << "------------------ BODY ------------------" << std::endl;
    std::cout << m_body << std::endl;
}
