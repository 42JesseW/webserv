#include "Route.hpp"

Route::Route() : m_has_autoindex(false)
{

}

Route::Route(const Route& route) :
    m_base_url(route.m_base_url),
    m_accepted_methods(route.m_accepted_methods),
    m_redir(route.m_redir),
    m_file_search_path(route.m_file_search_path),
    m_has_autoindex(route.m_has_autoindex),
    m_index_files(route.m_index_files),
    m_cgi_file_extensions(route.m_cgi_file_extensions)
{

}

Route::~Route()
{

}

Route&      Route::operator = (const Route &route)
{
    if (this != &route)
    {
        *this = route;
    }
    return (*this);
}
