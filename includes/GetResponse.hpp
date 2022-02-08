#ifndef GETRESPONSE_HPP
# define GETRESPONSE_HPP

# include "Response.hpp"
# include "StatusCodes.hpp"

class GetResponse : public Response
{
	private:
		StatusCodes		m_status_map;
		int				m_status_code;

	public:
		GetResponse();
		GetResponse(const Request &r);
		GetResponse(const GetResponse &copy);
		~GetResponse();

		GetResponse& operator = (const GetResponse &copy);

		int				handleMethod();
		void			buildStartLine();
		void			buildHeaders();
		void			buildBody();

		static int		ft_stoi(std::string input);
};

#endif
