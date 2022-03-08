#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <Common.hpp>
# include <Request.hpp>
# include <ConfigUtil.hpp>

# define CRLF "\r\n"

class Response
{
	protected:
		Request 								m_request;
		std::string								m_start_line;
		std::map<std::string, std::string>		m_headers_map;
		std::string								m_headers_str;
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
		std::string const &							getResponse() const;

		// For each child class to define
		virtual void							handleMethod() = 0;
		virtual void							buildStartLine(ConfigUtil::status_code_map_t& m_error_files) = 0;
		virtual void							buildHeaders() = 0;
		virtual void							buildBody() = 0;
		virtual void							buildResponse(ConfigUtil::status_code_map_t& m_error_files);
};

#endif
