#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Common.hpp"
# include "StatusCodes.hpp"
# include "Request.hpp"

class Response
{
	private:
		Request 								m_request;
		// std::map<int, std::string>*		m_status_codes;
		std::string								m_start_line;
		std::map<std::string, std::string>		m_headers;
		std::string								m_body;
		std::string 							m_response;

	public:
		Response(const Request &r);
		Response(const Response &copy);
		~Response();

		Response& operator = (const Response &copy);

		// Getters
		// std::string								getStartLine();
		// std::map<std::string, std::string>		getHeaders();
		// std::string								getBody();
		std::string								getResponse();

		// For each child class to define
		void									handleMethod() = 0;
};

#endif
