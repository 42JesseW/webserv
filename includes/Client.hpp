#ifndef CLIENT_HPP

# define CLIENT_HPP

#include <Request.hpp>
#include <Route.hpp>

class Client
{
	private:

	public:
		int			m_socket;
		Request		m_request;
		Route		m_route;
		// Response	*m_response;

		Client();
		Client(const Client& client);
		~Client();

		Client& operator = (const Client& client);

		void		setSocket(int socket);

		int			&getSocket();
		Request		&getRequest();

		void		setCorrectRoute(std::vector<Route> &server_routes);
		void		checkRoute();
		void		checkAcceptedMethods();
		void		checkRedirects();
		void		checkFileSearchPath();
};

#endif
