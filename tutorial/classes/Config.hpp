#ifndef CONFIG_HPP

# define CONFIG_HPP
# define DFL_MAX_BODY_SIZE 1

# include "Server.hpp"
# include <map>

class Config
{
private:
    std::map<int, std::string>  m_error_files;
    std::vector<Server>         m_servers;

public:
    Config();
    Config(const Config& config);
    ~Config();

    Config&     operator = (const Config& config);

    /* map from error code to error file */
    std::map<int, std::string>&   getErrorFiles();
    std::vector<Server>&          getServers();

private:
    static int            m_client_max_body_size;

};

#endif
