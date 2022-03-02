#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <Common.hpp>

# define BUFF_SIZE 2048
# define CR '\r'
# define LF '\n'
# define ALLOWED_VERSION "HTTP/1.1"

class Request
{
	protected:
		int										m_status;
		std::string								m_target;
		std::string								m_query;
		std::map<std::string, std::string>		m_headers;
		std::string								m_body;
		std::string     						m_extension;

	private:
		std::string 							m_method;
		std::string								m_version;
		std::string								m_request;
		int										m_port;
		bool									m_keep_alive;

		void									setHost();
		void									setConnection();

	public:
		Request();
		Request(const Request &Copy);
		~Request();

		Request& operator = (const Request &Copy);

		// Parsing
		void									handleRequest(int client_socket);
		void									divideRequest();
		void 									parseAndSetStartLine();
		void 									parseAndSetHeaders();
		
		virtual void							setExtension();

		int										getStatus();
		std::string								getTarget();
		std::string								getQuery();
		std::map<std::string, std::string>		getHeaders();
		std::string								getBody();
		
		void									errorChecking();
		void									checkStatusLine();
		void									checkHeaders();

		void									printRequest();
		bool									keepAlive();
};

#endif
