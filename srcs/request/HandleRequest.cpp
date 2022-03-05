#include <Request.hpp>

void Request::handleRequest(int client_socket)
{
	char    buffer[BUFF_SIZE];
	int     bytes_read = 0;

	bytes_read = recv(client_socket, buffer, BUFF_SIZE, MSG_DONTWAIT);
	this->m_request.append(buffer, bytes_read);
	std::memset(buffer, 0, BUFF_SIZE);
	std::cout << "Bytes read: " << bytes_read << std::endl;
	if (bytes_read == -1)
	{
		m_status = 400;
	}
	if (bytes_read == 0)
	{
		divideRequest();
		errorChecking();
	}	
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
	m_request.erase(0, m_request.find(' ') + 1);
	m_request.erase(0, m_request.find_first_not_of(' '));
	std::string m_url = m_request.substr(0, m_request.find(' '));
	m_request.erase(0, m_request.find(' ') + 1);
	m_request.erase(0, m_request.find_first_not_of(' '));
	m_version = m_request.substr(0, m_request.find(LF));
	m_request.erase(0, m_request.find(LF) + 1);

	if (!m_url.empty())
	{
		if (m_url.find('?') != std::string::npos)
		{
			m_query = m_url.substr(m_url.find('?') + 1);
			m_url.erase(m_url.find('?'));
		}
	}
	m_target = m_url;
}

void Request::setHost()
{
	std::string	host = m_headers["Host"];
	if (host.find(':') != std::string::npos)
	{
		host = host.substr(host.find(':') + 1, host.length() - 1);
		std::istringstream(host) >> m_port;
	}
}

void Request::parseAndSetHeaders()
{
	std::string token;
	std::string key;
	std::string value;

	// CR (\r)
	// LF (\n)

	while (m_request[0] != '\r' && !m_request.empty())
	{
		token = m_request.substr(0, m_request.find(LF));
		key = m_request.substr(0, m_request.find(':'));
		m_request.erase(0, m_request.find(':') + 2);
		value = m_request.substr(0, m_request.find(CR));
		m_headers.insert(std::pair<std::string, std::string>(key, value));
		m_request.erase(0, m_request.find(LF) + 1);
	}

	setHost();
}

void Request::printRequest()
{
	std::cout << "------------------ START LINE ------------------" << std::endl;
	std::cout << "Status: " << m_status << std::endl;
	std::cout << "Method: " << m_method << std::endl;
	std::cout << "Target: " << m_target << std::endl;
	std::cout << "Query:" << m_query << std::endl;
	std::cout << "Version: " << m_version << std::endl;
	std::cout << "Port: " << m_port << std::endl;

	std::cout << "------------------ HEADERS ------------------" << std::endl;
	for (std::map<std::string, std::string>::iterator it = m_headers.begin(); it != m_headers.end(); ++it)
		std::cout << "{" << it->first << "} {" << it->second << "}" << std::endl;

	std::cout << "------------------ BODY ------------------" << std::endl;
	std::cout << m_body << std::endl;
	std::cout << "------------------ END ------------------" << std::endl;
}
