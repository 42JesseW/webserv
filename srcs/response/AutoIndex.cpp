#include "Response.hpp"
#include "dirent.h"

void Response::buildAutoIndex(void)
{
    DIR             *dir;
    struct dirent   *ent;

    if ((dir = opendir(m_request.getTarget().c_str())) != NULL) 
    {
        std::cout << "WE OPENEND UP THE DIR" << std::endl;
        while ((ent = readdir (dir)) != NULL) {
            std::cout << ent->d_name << std::endl;
        }
        closedir (dir);
    }
}
