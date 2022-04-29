#include "Response.hpp"
#include "dirent.h"

void Response::_sortAndAddToBody(std::vector<std::string> directories, std::vector<std::string> files)
{
    std::sort(directories.begin(), directories.end());
    std::sort(files.begin(), files.end());

    m_body.append("<h1>Index of " + m_request.getTarget() +  "</h1>");
    for (std::vector<std::string>::iterator it = directories.begin(); it != directories.end(); it++ ) 
    {
        m_body.append(*it);
    }

    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++ ) 
    {
        m_body.append(*it);
    }
}

void Response::buildAutoIndex(void)
{
    std::vector<std::string>    directories;
    std::vector<std::string>    files;
    DIR                         *dir;
    struct dirent               *ent;

    if ((dir = opendir(m_request.m_filesearch.c_str())) != NULL) 
    {
        while ((ent = readdir (dir)) != NULL) 
        {
            if (ent->d_type == DT_DIR) 
            {
                directories.push_back("<a href=\"" + m_request.getTarget() + ent->d_name + "/\">" + ent->d_name + "/" + "</a><br>");
            }
            else
            {
                if (m_request.getTarget() == "/")
                    files.push_back("<a href=\"" + m_request.getTarget() + ent->d_name + "\">" + ent->d_name + "</a><br>");
                else
                    files.push_back("<a href=\"" + m_request.getTarget() + "/" + ent->d_name + "\">" + ent->d_name + "</a><br>");
            }
        }
        _sortAndAddToBody(directories, files);
        closedir (dir);
    }
    else
    {
        m_request.setStatus(HTTP_STATUS_NOT_FOUND);
    }
}
