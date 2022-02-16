#ifndef GETRESPONSE_HPP
# define GETRESPONSE_HPP

# include "Response.hpp"
# include "StatusCodes.hpp"
# include <sstream>

class GetResponse : public Response
{
	private:
		StatusCodes		m_status_map;
		int				m_status_code;
		std::string		m_response;

	public:
		GetResponse();
		GetResponse(const Request &r);
		GetResponse(const GetResponse &copy);
		~GetResponse();

		GetResponse& operator = (const GetResponse &copy);

		int					handleMethod();
		void				buildStartLine();
		// void				buildHeaders();
		void				buildBody();

		static std::string	ft_itos(int nbr);
};

#endif
