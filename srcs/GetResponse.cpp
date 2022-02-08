#include "../includes/GetResponse.hpp"

GetResponse::GetResponse() {}

GetResponse::GetResponse(const Request &r)
{
	m_request = r;
}

GetResponse::GetResponse(const GetResponse &copy)
{
	m_request = copy.m_request;
	m_start_line = copy.m_start_line;
	m_headers = copy.m_headers;
	m_body = copy.m_body;
	m_response = copy.m_response;
}

GetResponse::~GetResponse() {}

GetResponse & GetResponse::operator=(const GetResponse &copy)
{
	if (this != &copy)
		*this = copy;
	return (*this);
}

int					GetResponse::handleMethod()
{
	// Processes the GET Method and returns the status code
	m_status_code = 200;
	return (m_status_code);
}

std::string			GetResponse::ft_itos(int nbr)
{
	// return(static);
	return (static_cast<std::ostringstream&>((std::ostringstream() << std::dec << nbr)).str());
}

void				GetResponse::buildStartLine()
{
	std::string								str_status_code;
	std::string 							reason_phrase;
	std::map<int, std::string>::iterator	it;

	str_status_code = ft_itos(m_status_code);
	std::cout << "Status code is: " << str_status_code << std::endl;

	// it = m_status_map.find(str_status_code);
	// if (it != m_status_map.end())
	// 	reason_phrase = m_status_map->second;
	// else
	// {
	// 	// throw exception?
	// 	std::cout << "Error: status code" <<std::endl;
	// 	std::exit(EXIT_FAILURE);
	// }

	// m_start_line = HTTP_VERSION + str_status_code + reason_phrase;
	// std::cout << m_start_line << std::endl;
}
