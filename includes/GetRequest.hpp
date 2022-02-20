#ifndef GETREQUEST_HPP
# define GETREQUEST_HPP

#include "Request.hpp"

class GetRequest : public Request {
	public:
		GetRequest();
		GetRequest(const GetRequest &copy);
		GetRequest& operator = (const GetRequest &copy);
		~GetRequest();

		std::string		getExtension();
		void			setExtension();
};

#endif