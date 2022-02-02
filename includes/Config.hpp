#ifndef CONFIG_HPP

# define CONFIG_HPP
# define DFL_MAX_BODY_SIZE 1
# define DFL_SERVER_HOST "*"                    /* translates to INADDR_ANY 0.0.0.0 */
# define DFL_SERVER_PORT 80                     /* HTTP default port */
# define DFL_CONFIG_FILE_PATH "default.conf"

# include <map>
# include <algorithm>
# include <stdexcept>
# include <exception>
# include <iostream>
# include <fstream>
# include <sstream>

# include "Server.hpp"

class Config
{
private:
    std::string                 m_file_path;
    std::map<int, std::string>  m_error_files;
    std::vector<Server>         m_servers;

private:
    /* don't implement constructors and assignment since singleton */
    Config(void);
    Config(Config const& config);

    void    operator = (const Config& config);

public:
    typedef enum e_levels
    {
        NONE,
        BASE,
        HTTP,
        SERVER,
        LOCATION
    } t_levels;

    typedef std::map<t_levels, std::vector<std::string> >   config_map_t;

    ~Config(void);

    /* implement a singleton pattern since Config references 1 file */
    static Config&                  getHandle(void);

    std::string&                    getFilePath();
    std::map<int, std::string>&     getErrorFiles(void);
    std::vector<Server>&            getServers(void);

    void                            setFilePath(const std::string& file_path);
    void                            loadFile(void);

private:
    static config_map_t             _getConfigOptions();

private:
    static int                      m_client_max_body_size;

};

#endif
