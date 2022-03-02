#ifndef GETRESPONSE_HPP
# define GETRESPONSE_HPP

# include <Response.hpp>
# include <ConfigUtil.hpp>
# include <sstream>

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
		// void				buildHeaders();
		void				buildBody();

		static std::string	ft_itos(int nbr);
};

#endif
