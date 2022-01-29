#include "Route.hpp"

Route::Route() :
    m_accepted_methods(NULL),
    m_redir(NULL),
    m_has_autoindex(false),
    m_index_files(NULL),
    m_cgi_file_extensions(NULL)
{

}

Route::Route(const Route& route)
{

}

Route::~Route()
{

}

Route&      Route::operator = (const Route &route)
{
    if (this != &route)
    {
        /* reassign values */
    }
    return (*this);
}
