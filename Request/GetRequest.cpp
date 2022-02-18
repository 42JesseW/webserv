#include "Request.hpp"

class GetRequest : public Request
{
    private:
        int             m_status;
        std::string     m_method;
        std::string     m_route;
};
