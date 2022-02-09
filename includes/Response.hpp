#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# define HTTP_VERSION "HTTP/1.1"

# include "Common.hpp"
# include "Request.hpp"

class Response
{
	protected:
		Request 								m_request;
		std::string								m_start_line;
		std::map<std::string, std::string>		m_headers;
		std::string								m_body;
		std::string 							m_response;

	public:
		Response();
		Response(const Request &r);
		Response(const Response &copy);
		virtual ~Response();

		Response& operator = (const Response &copy);

		// Getters
		// std::string								getStartLine();
		// std::map<std::string, std::string>		getHeaders();
		// std::string								getBody();
		std::string								getResponse();

		// For each child class to define
		virtual int								handleMethod() = 0;
		virtual void							buildStartLine() = 0;
		// virtual void							buildHeaders() = 0;
		// virtual void							buildBody() = 0;
		virtual void							buildResponse();
};

#endif
