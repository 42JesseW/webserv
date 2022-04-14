#include <Handler.hpp>

bool Handler::post_handler(Request &request, std::string uploadPath)
{
    std::string filename;

    filename = uploadPath + request.getFilename();
    if (filename.at(0) == '/')
            filename.erase(0, 1);
    if (!filename.empty())
    {
        std::ofstream newFile;
        newFile.open(filename, std::ofstream::out | std::ofstream::binary);
        if (!newFile.is_open())
            return (false);
        newFile << request.getBody();
        newFile.close();
        return (true);
    }
    return (false);
}
