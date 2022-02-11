#include "../includes/Route.hpp"

Route::Route() : m_has_autoindex(false), m_redir(NULL)
{
    // TODO set all the defaults LOL
    m_accepted_methods.push_back("GET");
    m_accepted_methods.push_back("POST");
    m_accepted_methods.push_back("DELETE");
    m_file_search_path = DFL_FILE_SEARCH_PATH;
    m_index_files.push_back(DFL_INDEX_FILE);
}

Route::Route(const Route& route) :
    m_base_url(route.m_base_url),
    m_accepted_methods(route.m_accepted_methods),
    m_redir(NULL),
    m_file_search_path(route.m_file_search_path),
    m_has_autoindex(route.m_has_autoindex),
    m_index_files(route.m_index_files),
    m_cgi_file_extensions(route.m_cgi_file_extensions)
{
    if (route.m_redir)
    {
        m_redir = new s_redirect();
        m_redir->status_code = route.m_redir->status_code;
        m_redir->url = route.m_redir->url;
    }
}

Route::~Route()
{
    delete m_redir;
}

Route&      Route::operator = (const Route &route)
{
    if (this != &route)
    {
        *this = route;
    }
    return (*this);
}

void                        Route::setBaseUrl(const std::string &base_url)
{
    m_base_url = base_url;
}

void                        Route::setFileSearchPath(const std::string &path)
{
    m_file_search_path = path;
}

void                        Route::setAutoIndex(bool on)
{
    m_has_autoindex = on;
}

std::vector<std::string>&   Route::getAcceptedMethods(void)
{
    return (m_accepted_methods);
}
