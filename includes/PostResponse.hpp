#ifndef POSTRESPONSE_HPP
# define POSTRESPONSE_HPP

# include <Response.hpp>

class PostResponse : public Response
{
	public:
		int				handleMethod();
		void			buildStartLine();
};

#endif
