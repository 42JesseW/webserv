#include <Handler.hpp>

bool Handler::post_handler(Request &request, std::string uploadPath)
{
    std::string filename;
    std::ofstream newFile;

    if (!filename.empty())
    {
        filename = uploadPath + request.getFilename();
    }
    else
    {
        std::string extension = request.getHeaders().find("Content-Type")->second;
        extension = extension.substr(extension.find('/') + 1);
        filename = "test." + extension;
    }
    newFile.open(uploadPath + filename, std::ofstream::out | std::ofstream::binary);
    if (!newFile.is_open())
        return (false);
    newFile << request.getBody();
    newFile.close();
        return (true);
    return (false);
}
