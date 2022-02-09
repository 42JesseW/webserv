#ifndef CONFIG_HPP

# define CONFIG_HPP
# define DFL_MAX_BODY_SIZE 1
# define DFL_SERVER_HOST "*"                    /* translates to INADDR_ANY 0.0.0.0 */
# define DFL_SERVER_PORT 80                     /* HTTP default port */
# define DFL_CONFIG_FILE_PATH "default.conf"

# include <map>
# include <stdexcept>
# include <exception>
# include <iostream>
# include <fstream>

# include "Server.hpp"

class Config
{
private:
    std::string                 m_file_path;
    std::map<int, std::string>  m_error_files;
    std::vector<Server>         m_servers;

public:
    Config(void);
    Config(const Config& config);
    ~Config(void);

    Config&     operator = (const Config& config);

    std::string&                    getFilePath();
    std::map<int, std::string>&     getErrorFiles(void);
    std::vector<Server>&            getServers(void);

    void                            setFilePath(const char *file_path);
    void                            loadFile(void);

private:
    static int            m_client_max_body_size;

};

#endif
