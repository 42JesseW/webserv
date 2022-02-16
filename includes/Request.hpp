#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Common.hpp"

# define BUFF_SIZE 2048
# define CR '\r'
# define LF '\n'
# define ALLOWED_VERSION "HTTP/1.1"

class Request
{
	private:
		int										m_status;
		std::string								m_target;
		std::string								m_query;
		std::string 							m_method;
		std::string								m_version;
		std::string								m_request;
		std::map<std::string, std::string>		m_headers;
		int										m_port;
		std::string								m_body;

	public:
		Request();
		Request(const Request &Copy);
		~Request();

		Request& operator = (const Request &Copy);

		// Getters
		std::string							getRequest();
		std::string 						getMethod();
		std::map<std::string, std::string>	getHeaders();
		std::string							getBody();
		int									getStatus();

		// Parsing
		void								handleRequest(int client_socket);
		void								divideRequest();
		void 								parseAndSetStartLine();
		void 								parseAndSetHeaders();
		
		// Error handling
		void								errorChecking();
		void								checkStatusLine();
		void								checkHeaders();

		// Utils
		void								printRequest();
};

#endif
