#ifndef GETRESPONSE_HPP
# define GETRESPONSE_HPP

# include "Response.hpp"
# include "StatusCodes.hpp"

class GetResponse : public Response
{
	private:
		int				m_status_code;

	public:
		int				handleMethod();
		void			buildStartLine();
		void			buildHeaders();
		void			buildBody();
};

#endif
