#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <Common.hpp>

# define BUFF_SIZE 20
# define CR '\r'
# define LF '\n'
# define ALLOWED_VERSION "HTTP/1.1"

class Request
{
	protected:
		std::string								m_target;
		std::string								m_query;
		std::map<std::string, std::string>		m_headers;
		std::string								m_body;
		std::string     						m_extension;

	private:
		int										m_status;
		std::string 							m_method;
		std::string								m_version;
		std::string								m_request;
		int										m_port;

		void									setHost();

	public:
		Request();
		Request(const Request &Copy);
		~Request();

		Request& operator = (const Request &Copy);

		void									handleRequest(int client_socket);
		void									divideRequest();
		void 									parseAndSetStartLine();
		void 									parseAndSetHeaders();
		
		void									setRequest(std::string new_request); // For testing only
		void									setStatus(int status);

		int										&getStatus();
		std::string								&getTarget();
		std::string								&getQuery();
		std::string								&getMethod();
		std::string								&getVersion();
		int										&getPort();
		std::map<std::string, std::string>		&getHeaders();
		std::string								&getBody();
		
		void									errorChecking();
		void									checkStatusLine();
		void									checkHeaders();

		void									resetRequest();
		void									printRequest();
};

#endif
