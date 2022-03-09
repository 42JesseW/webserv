#ifndef CLIENT_HPP

# define CLIENT_HPP

# include <Request.hpp>
# include <Response.hpp>

class Client
{
	private:

	public:
		int			m_socket;
		Request		m_request;
		Response	m_response;

		Client();
		Client(const Client& client);
		~Client();

		Client& operator = (const Client& client);

		void		setSocket(int socket);

		int			&getSocket();
		Request		&getRequest();
		Response	&getResponse();
};

#endif
