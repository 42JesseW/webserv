#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <Request.hpp>
# include <ConfigUtil.hpp>
# include <Utils.hpp>
# include <Route.hpp>
# include <StatusCodes.hpp>

# include <time.h>
# include <ctime>
# include <cstdlib>
# include <iostream>
# include <fstream>
# include <sstream>

# define RETRY_AFTER_SEC	"120"
# define HTML_FILE_FLAG		0
# define ERROR_FILE_FLAG	1

typedef std::pair<std::string, std::string> status_code_body_t;
typedef std::map<int, status_code_body_t>   status_code_map_t;

class Response
{
	private:
		Request 								m_request;
		Route									m_route;
		int										m_status_code;
		std::string								m_start_line;
		std::map<std::string, std::string>		m_headers_map;
		std::string								m_headers_str;
		std::string								m_body;
		std::string 							m_response;

	public:
		Response();
		Response(Request &re);
		Response(Request &re, Route &ro);
		Response(const Response &copy);
		~Response();

		Response& operator = (const Response &copy);

		// Getters
		Request const &											getRequest() const;
		Route const &											getRoute() const;
		int const &												getStatusCode() const;					
		std::string const &										getStartLine() const;
		std::map<std::string, std::string> const &				getHeadersMap() const;
		std::string	const &										getHeadersStr() const;
		std::string	const &										getBody() const;
		std::string const &										getResponse() const;

		void			buildStartLine(ConfigUtil::status_code_map_t& m_error_files);
		void			buildHeaders();
		void			buildBody(ConfigUtil::status_code_map_t& m_error_files);
		void			buildResponse(ConfigUtil::status_code_map_t& m_error_files);
		void			resetResponse();

	private:
		status_code_body_t	_buildDate();
		status_code_body_t	_buildLocation();
		status_code_body_t	_buildRetryAfter();
		status_code_body_t	_buildAllow();
		status_code_body_t	_buildServer();
		status_code_body_t	_buildConnection();
		status_code_body_t	_buildContentLength();
		status_code_body_t	_buildContentType();
		status_code_body_t	_buildTransferEncoding();
		int					_readFileIntoString(const std::string &path, int error_file);
};

#endif
