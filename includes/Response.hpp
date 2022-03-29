#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <Common.hpp>
# include <Request.hpp>
# include <ConfigUtil.hpp>
# include <Utils.hpp>
# include <Route.hpp>

# include <time.h>
# include <ctime>

# define CRLF "\r\n"
# define RETRY_AFTER_SEC "120"

typedef std::pair<std::string, std::string> string_pair_t;

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
		void			buildBody();
		void			buildResponse(ConfigUtil::status_code_map_t& m_error_files);
		void			resetResponse();

	private:
		string_pair_t	_buildDate();
		string_pair_t	_buildLocation();
		string_pair_t	_buildRetryAfter();
		string_pair_t	_buildAllow();
		string_pair_t	_buildServer();
		string_pair_t	_buildConnection();
		string_pair_t	_buildContentLength();
		string_pair_t	_buildContentType();
		string_pair_t	_buildTransferEncoding();
		int				_readFileIntoString(const std::string &path);
};

#endif
