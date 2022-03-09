#ifndef CLIENT_HPP

# define CLIENT_HPP

#include "../includes/Request.hpp"

class Client
{
private:
	Request		m_request;
	// Response	m_response;

public:
	Client();
	Client(const Client& client);
	~Client();

	Client& operator = (const Client& client);	

	Request		&getRequest();
};

#endif
