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
    std::string&                    _stripFileData(std::string& buffer);
    std::deque<std::string>&        _tokenizeFileData(std::deque<std::string>& tokens, std::string& data);
    void                            _loadFile(std::deque<std::string>& tokens);

private:
    static int                      m_client_max_body_size;
    static ConfigOption             *m_option_http;
    static ConfigOption             *m_option_client_max_body_size;
    static ConfigOption             *m_option_error_page;
    static ConfigOption             *m_option_server;
    static ConfigOption             *m_option_listen;
    static ConfigOption             *m_option_server_name;
    static ConfigOption             *m_option_location;
    static ConfigOption             *m_option_allowed_methods;
    static ConfigOption             *m_option_root;
    static ConfigOption             *m_option_autoindex;
    static ConfigOption             *m_option_index;
    static ConfigOption             *m_option_cgi_extension;
    static ConfigOption             *m_option_upload_path;
    static ConfigOption             *m_option_return;

    static ConfigOption::map_config_t&     _getConfigMap(void);

};

namespace ft {

    bool            bothAreChar(char lhs, char rhs, int c);
    std::string&    stringReplaceMultiple(std::string& str, const std::string& from, const std::string& to);

    template<class ForwardIt>
    ForwardIt uniqueChars(ForwardIt first, ForwardIt last, int c)
    {
        ForwardIt result;

        if (first == last)
            return (last);
        result = first;
        while (++first != last)
        {
            if (!bothAreChar(*result, *first, c) && ++result != first)
                *result = *first;
        }
        result++;
        return (result);
    }
}

#endif
