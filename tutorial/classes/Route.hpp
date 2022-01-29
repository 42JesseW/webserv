#ifndef ROUTE_HPP

# define ROUTE_HPP

# include <string>      // TODO maybe move to more common hpp

struct s_redirect
{
    std::string url;
    int         status_code;
};

typedef struct s_redirect REDIR;

class Route
{
private:
    std::string m_base_url;
    std::string *m_accepted_methods;
    REDIR       *m_redir;
    std::string m_file_search_path;
    bool        m_has_autoindex;
    std::string *m_index_files;
    std::string *m_cgi_file_extensions;

public:
    Route();
    Route(const Route& route);

    ~Route();

    Route&  operator = (const Route& route);
};

#endif
