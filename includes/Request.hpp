#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <Common.hpp>
# include <algorithm>

# define BUFF_SIZE 10
# define CR '\r'
# define LF '\n'
# define ALLOWED_VERSION "HTTP/1.1"
# define HTTP_STATUS_OK 200


class Request
{
	protected:
		std::string								m_target;
		std::string								m_filename;
		std::string								m_query;
		std::map<std::string, std::string>		m_headers;
		std::string								m_body;
		bool									m_done;
		std::string								m_cgi_path;

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
		void									parseQuery(std::string url);
		void									parseFilenamesAndCGI();
		
		void									setRequest(std::string new_request); // For testing only
		void									setStatus(int status);
		void									setDone(bool status);

		int										&getStatus();
		int										&getPort();
		std::string								&getTarget();
		std::string								&getQuery();
		std::string								&getMethod();
		std::string								&getVersion();
		std::string								&getFilename();
		std::string								&getBody();
		std::map<std::string, std::string>		&getHeaders();
		bool									&isDone();
		
		void									errorChecking();
		void									checkStatusLine();
		void									checkHeaders();

		void									resetRequest();
		void									printRequest();
};

#endif
