#ifndef GETRESPONSE_HPP
# define GETRESPONSE_HPP

# include <Response.hpp>
# include <ConfigUtil.hpp>
# include <Utils.hpp>
# include <Route.hpp>

# include <sstream>
# include <time.h>

# define RETRY_AFTER_SEC "120"

class GetResponse : public Response
{
	private:
		int				m_status_code;
		std::string		m_response;

	public:
		GetResponse();
		GetResponse(const Request &r);
		GetResponse(const GetResponse &copy);
		~GetResponse();

		GetResponse& operator = (const GetResponse &copy);

		void				handleMethod();
		void				buildStartLine(ConfigUtil::status_code_map_t& m_error_files);
		void				buildHeaders();
		void				buildBody();

	private:
		std::pair<std::string, std::string>	_buildDate();
		std::pair<std::string, std::string>	_buildLocation();
		std::pair<std::string, std::string>	_buildRetryAfter();
		std::pair<std::string, std::string>	_buildAllow();
		std::pair<std::string, std::string>	_buildServer();
		std::pair<std::string, std::string>	_buildConnection();
		std::pair<std::string, std::string>	_buildContentLength();
		std::pair<std::string, std::string>	_buildContentType();
		std::pair<std::string, std::string>	_buildTransferEncoding();
};

#endif
