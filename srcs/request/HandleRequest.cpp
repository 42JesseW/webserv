#include <Request.hpp>

void Request::handleRequest(int client_socket)
{
	char    buffer[BUFF_SIZE];
	int     bytes_read = 0;

	bytes_read = recv(client_socket, buffer, BUFF_SIZE, MSG_DONTWAIT);
	if (bytes_read == -1)
	{
		setStatus(HTTP_STATUS_BAD_REQUEST);
	}
	else
	{
		m_request.append(buffer, bytes_read);
		if (bytes_read < BUFF_SIZE && !m_request.empty())
		{
			divideRequest();
			errorChecking();
			setDone(true);
		}
	}
	std::memset(buffer, 0, BUFF_SIZE);
}

void Request::divideRequest()
{
	parseAndSetStartLine();
	parseAndSetHeaders();
	if (!m_request.empty())
	{
		m_request.erase(0, 2);
		m_body = m_request;
	}
	if(getHeaders().find("Content-Encoding")->second == "chunked")
	{
		decodeRequest();
	}
	m_request.clear();
}

void Request::parseFilenamesAndCGI()
{
	if (std::count(m_target.begin(), m_target.end(), '.') > 2)
	{
		setStatus(HTTP_STATUS_TEAPOT);
	}
	else if (std::count(m_target.begin(), m_target.end(), '.') == 2)
    {
		std::string tmp = m_target.substr(m_target.find_first_of('.'));
		m_target.erase(m_target.find_first_of('.'));
		m_target.append(tmp.substr(0, tmp.find_first_of('/')));
		tmp.erase(0, tmp.find_first_of('/'));
		m_cgi_path = tmp;
		if (std::count(m_target.begin(), m_target.end(), '/') > 1)
		{
			m_filename = m_target.substr(m_target.find_last_of('/'));
			m_target.erase(m_target.find_last_of('/'));
		}
		else
		{
			m_filename = m_target.substr(m_target.find('/'));
			m_target = "/";
		}
    }
    else
    {
		m_filename = m_target.substr(m_target.find_last_of('/'));
		m_target.erase(m_target.find_last_of('/'));
		if (m_target.empty())
		{
			m_target = "/";
		}
    }
}

void Request::parseQuery(std::string url)
{
	if (!url.empty())
	{
		if (url.find('?') != std::string::npos)
		{
			m_query = url.substr(url.find('?') + 1);
			url.erase(url.find('?'));
		}
	}
	if (!url.empty())
	{
		m_target = url;
	}
}

void Request::parseAndSetStartLine()
{
	m_method = m_request.substr(0, m_request.find(' '));
	m_request.erase(0, m_request.find(' ') + 1);
	m_request.erase(0, m_request.find_first_not_of(' '));
	std::string url = m_request.substr(0, m_request.find(' '));
	m_request.erase(0, m_request.find(' ') + 1);
	m_request.erase(0, m_request.find_first_not_of(' '));
	m_version = m_request.substr(0, m_request.find(CR));
	m_request.erase(0, m_request.find(LF) + 1);

	parseQuery(url);
	parseFilenamesAndCGI();
}

void Request::setHost()
{
	if (m_headers.find("Host") == m_headers.end())
	{
		setStatus(HTTP_STATUS_BAD_REQUEST);
	}
	else 
	{
		std::string	host = m_headers["Host"];
		if (host.find(':') != std::string::npos)
		{
			host = host.substr(host.find(':') + 1, host.length() - 1);
			std::istringstream(host) >> m_port;
		}
	}
}

void Request::parseAndSetHeaders()
{
	std::string token;
	std::string key;
	std::string value;

	while (m_request[0] != '\r' && !m_request.empty())
	{
		token = m_request.substr(0, m_request.find(CR));
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
	std::cout << "Filename: " << m_filename << std::endl;
	std::cout << "CGI: " << m_cgi_path << std::endl;
	std::cout << "Query: " << m_query << std::endl;
	std::cout << "Version: " << m_version << std::endl;
	std::cout << "Port: " << m_port << std::endl;

	std::cout << "------------------ HEADERS ------------------" << std::endl;
	for (std::map<std::string, std::string>::iterator it = m_headers.begin(); it != m_headers.end(); ++it)
		std::cout << "{" << it->first << "} {" << it->second << "}" << std::endl;

	std::cout << "------------------ BODY ------------------" << std::endl;
	std::cout << m_body << std::endl;
	std::cout << "------------------ END ------------------" << std::endl;
}
