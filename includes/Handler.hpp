#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <Request.hpp>

class Handler
{
    public:
        bool post_handler(Request &request, std::string uploadPath);
        bool delete_handler(Request &request, std::string searchPath);

    public:
        Handler();
        ~Handler();
};

#endif
