#ifndef CLIENT_HPP

# define CLIENT_HPP

# include <Request.hpp>
# include <Route.hpp>
# include <Response.hpp>
# include <Socket.hpp>

class Client
{
	private:
		void		searchFile();
		void 		searchDefaultIndexPages();
		bool		seachCGIExtensions();

	public:
		Socket		m_socket;
		Request		m_request;
		Route		m_route;
		Response	m_response;

		Client();
		Client(const Client& client);
		~Client();

		Client& operator = (const Client& client);

		void		setSocketFD(int socket);

		Socket		&getSocket();
		Request		&getRequest();

		void		setCorrectRoute(std::vector<Route> &server_routes);
		void		checkRoute();
		void		checkAcceptedMethods();
		void		checkRedirects();
		void		checkFileSearchPath();
};

#endif
