#ifndef CONFIG_HPP

# define CONFIG_HPP

# define DFL_MAX_BODY_SIZE 1
# define DFL_SERVER_HOST "*"                    /* translates to INADDR_ANY 0.0.0.0 */
# define DFL_SERVER_PORT 80                     /* HTTP default port */
# define DFL_CONFIG_FILE_PATH "default.conf"

# define OPTION_ARG_INF -1

# include <map>
# include <algorithm>
# include <stdexcept>
# include <exception>
# include <iostream>
# include <fstream>
# include <sstream>

# include "Server.hpp"

class ConfigOption
{
public:
    bool    m_is_nested;      /* if option has nested options */
    int     m_arg_amount;     /* amount of options following this option after keyword found */
    int     m_parse_level;    /* parse_level at which to find option */
    // some function that can be used to modify either [Config, Server, Location] objects
    int     (*m_parse_func)(void *, int);     // void * = Object to modify and int = position of argument

    typedef std::map<std::string, ConfigOption*> map_config_t;

public:
    ConfigOption(bool is_nested, int arg_amount, int parse_level, int (*parse_func)(void *, int));
    ConfigOption(const ConfigOption& cpy);
    ~ConfigOption();

};


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

    ~Config(void);

    /* implement a singleton pattern since Config references 1 file */
    static Config&                  getHandle(void);

    std::string&                    getFilePath();
    std::map<int, std::string>&     getErrorFiles(void);
    std::vector<Server>&            getServers(void);

    void                            setFilePath(const std::string& file_path);
    void                            loadFile(void);

private:
    ConfigOption::map_config_t&     _getConfigMap(void);
    std::string&                    _stripFileData(std::string& buffer);
    std::deque<std::string>&        _tokenizeFileData(std::deque<std::string>& tokens, std::string& data);
    void                            _loadFile(std::deque<std::string>& tokens);

private:
    static int                      m_client_max_body_size;

};

namespace ft {

    ConfigOption option_http(true, 0, Config::BASE, NULL);
    ConfigOption option_client_max_body_size(false, 1, Config::HTTP, NULL);
    ConfigOption option_error_page(false, 2, Config::HTTP, NULL);
    ConfigOption option_server(true, 0, Config::HTTP, NULL);
    ConfigOption option_listen(false, 1, Config::SERVER, NULL);
    ConfigOption option_server_name(false, OPTION_ARG_INF, Config::SERVER, NULL);
    ConfigOption option_location(true, 1, Config::SERVER, NULL);
    ConfigOption option_allowed_methods(false, OPTION_ARG_INF, Config::LOCATION, NULL);
    ConfigOption option_root(false, 1, Config::LOCATION, NULL);
    ConfigOption option_autoindex(false, 1, Config::LOCATION, NULL);
    ConfigOption option_index(false, OPTION_ARG_INF, Config::LOCATION, NULL);
    ConfigOption option_cgi_extension(false, OPTION_ARG_INF, Config::LOCATION, NULL);
    ConfigOption option_upload_path(false, 1, Config::LOCATION, NULL);
    ConfigOption option_return(false, 2, Config::LOCATION, NULL);

    bool bothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }

    template<class ForwardIt>
    ForwardIt uniqueSpaces(ForwardIt first, ForwardIt last)
    {
        ForwardIt result;

        if (first == last)
            return (last);
        result = first;
        while (++first != last)
        {
            if (!bothAreSpaces(*result, *first) && ++result != first)
                *result = *first;
        }
        result++;
        return (result);
    }
}

#endif
