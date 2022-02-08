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

int			GetResponse::handleMethod()
{
	// Processes the GET Method and returns the status code
	m_status_code = 200;
	return (m_status_code);
}

int			GetResponse::ft_stoi(std::string input)
{
	int	nbr;
	int	sign;
	int	i;

	nbr = 0;
	sign = 1;
	i = 0;
	if (input[i] == '+' || input[i] == '-')
	{
		if (input[i] == '-')
			sign = -1;
		i++;
	}
	while (input[i] >= '0' && input[i] <= '9')
	{
		nbr = nbr * 10 + (input[i] - 48);
		i++;
	}
	return(nbr * sign);
}

void		GetResponse::buildStartLine()
{
	std::string								str_status_code;
	std::string 							reason_phrase;
	std::map<int, std::string>::iterator	it;

	str_status_code = ft_stoi(m_status_code);

	it = m_status_codes.find(str_status_code);
	if (it != m_status_codes.end())
		reason_phrase = m_status_codes->second;
	else
	{
		// throw exception?
		std::cout << "Error: status code" <<std::cout;
		std::exit(EXIT_FAILURE);
	}

	m_start_line = HTTP_VERSION + str_status_code + reason_phrase;
	std::cout << m_start_line << std::out;
}
