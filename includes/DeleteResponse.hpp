#ifndef DELETERESPONSE_HPP
# define DELETERESPONSE_HPP

# include "Response.hpp"

class DeleteResponse : public Response
{
	public:
		int		handleMethod();
		void	buildStartLine();
};

#endif
