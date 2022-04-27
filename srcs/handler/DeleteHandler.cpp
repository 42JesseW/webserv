#include <Handler.hpp>

bool Handler::delete_handler(Request &request, std::string searchPath)
{
    std::string filename;

    filename = searchPath + request.getFilename();
    if (request.getFilename().empty())
    {
        request.setStatus(HTTP_STATUS_BAD_REQUEST);
        return (false);
    }
    if (!filename.empty())
    {
        if (std::remove(filename.c_str()) != 0)
            return (false);
    }
    return (true);
}
