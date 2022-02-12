#ifndef CONFIG_HPP

# define CONFIG_HPP

# define DFL_CONFIG_FILE_PATH "default.conf"
# define DFL_MAX_BODY_SIZE 1
# define DFL_SERVER_HOST "*"                    /* translates to INADDR_ANY 0.0.0.0 */
# define DFL_SERVER_PORT 80                     /* HTTP default port */

# include <map>
# include <stack>
# include <algorithm>
# include <stdexcept>
# include <exception>
# include <iostream>
# include <fstream>
# include <sstream>

# include <Server.hpp>
# include <Utils.hpp>

class Config
{
private:
    std::string                 m_file_path;
    std::map<int, std::string>  m_error_files;
    std::vector<Server>         m_servers;

private:
    Config(Config const& config);

    void    operator = (const Config& config);

public:
    typedef enum e_levels
    {
        BASE,
        HTTP,
        SERVER,
        LOCATION
    } t_levels;

    typedef std::deque<std::string>         tokens_t;

    ~Config(void);

protected:
    static Config                   *handle;

/* don't implement constructors and assignment since singleton */
Config(void);

public:
    /* implement a singleton pattern since Config references 1 file */
    static Config&                  getHandle(void);

    std::string&                    getFilePath();
    std::map<int, std::string>&     getErrorFiles(void);
    std::vector<Server>&            getServers(void);

    void                            setFilePath(const std::string& file_path);
    void                            loadFile(void);

public:
    class Option
    {
    private:
        int     m_parse_level;    /* parse_level at which to find option */

    public:
        typedef std::map<std::string, Option*>  map_config_t;

    public:
        explicit Option(int parse_level);
        Option(const Option& cpy);
        ~Option();

        Option&         operator = (const Option& rhs);

        int             getParseLevel(void);

        // some function that can be used to modify either [Config, Server, Route] objects
        virtual void    parse(void *obj, tokens_t& tokens) = 0;

    };

private:
    static int                      m_client_max_body_size;

    void                            _mapTokens(tokens_t& tokens);
    void                            _mapTokenToObject(tokens_t& tokens,
                                                      Option *option,
                                                      Server **server,
                                                      Route **route,
                                                      int parse_level);

public:
    /*
     *          ( Option )
     * All subclass based on abstract Option
     */
    class OptionHttp : public Option
    {
    public:
        OptionHttp(int parse_level);
        OptionHttp(const OptionHttp& cpy);
        ~OptionHttp();

        OptionHttp&     operator = (const OptionHttp& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionClientMaxBodySize : public Option
    {
    public:
        OptionClientMaxBodySize(int parse_level);
        OptionClientMaxBodySize(const OptionClientMaxBodySize& cpy);
        ~OptionClientMaxBodySize();

        OptionClientMaxBodySize&     operator = (const OptionClientMaxBodySize& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionErrorPage : public Option
    {
    public:
        OptionErrorPage(int parse_level);
        OptionErrorPage(const OptionErrorPage& cpy);
        ~OptionErrorPage();

        OptionErrorPage&     operator = (const OptionErrorPage& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionServer : public Option
    {
    public:
        OptionServer(int parse_level);
        OptionServer(const OptionServer& cpy);
        ~OptionServer();

        OptionServer&     operator = (const OptionServer& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionListen : public Option
    {
    public:
        OptionListen(int parse_level);
        OptionListen(const OptionListen& cpy);
        ~OptionListen();

        OptionListen&   operator = (const OptionListen& rhs);

        void            parse(void *obj, tokens_t& tokens);

    private:
        void            _parseArg(const std::string& arg, std::string& address, short *sin_port);

    };

    class OptionServerName : public Option
    {
    public:
        OptionServerName(int parse_level);
        OptionServerName(const OptionServerName& cpy);
        ~OptionServerName();

        OptionServerName&     operator = (const OptionServerName& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionLocation : public Option
    {
    public:
        OptionLocation(int parse_level);
        OptionLocation(const OptionLocation& cpy);
        ~OptionLocation();

        OptionLocation&     operator = (const OptionLocation& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionAllowedMethods : public Option
    {
    public:
        OptionAllowedMethods(int parse_level);
        OptionAllowedMethods(const OptionAllowedMethods& cpy);
        ~OptionAllowedMethods();

        OptionAllowedMethods&     operator = (const OptionAllowedMethods& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionRoot : public Option
    {
    public:
        OptionRoot(int parse_level);
        OptionRoot(const OptionRoot& cpy);
        ~OptionRoot();

        OptionRoot&     operator = (const OptionRoot& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionAutoIndex : public Option
    {
    public:
        OptionAutoIndex(int parse_level);
        OptionAutoIndex(const OptionAutoIndex& cpy);
        ~OptionAutoIndex();

        OptionAutoIndex&     operator = (const OptionAutoIndex& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionIndex : public Option
    {
    public:
        OptionIndex(int parse_level);
        OptionIndex(const OptionIndex& cpy);
        ~OptionIndex();

        OptionIndex&     operator = (const OptionIndex& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionCgiExtension : public Option
    {
    public:
        OptionCgiExtension(int parse_level);
        OptionCgiExtension(const OptionCgiExtension& cpy);
        ~OptionCgiExtension();

        OptionCgiExtension&     operator = (const OptionCgiExtension& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionUploadPath : public Option
    {
    public:
        OptionUploadPath(int parse_level);
        OptionUploadPath(const OptionUploadPath& cpy);
        ~OptionUploadPath();

        OptionUploadPath&     operator = (const OptionUploadPath& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    class OptionReturn : public Option
    {
    public:
        OptionReturn(int parse_level);
        OptionReturn(const OptionReturn& cpy);
        ~OptionReturn();

        OptionReturn&     operator = (const OptionReturn& rhs);

        void            parse(void *obj, tokens_t& tokens);

    };

    static Config::Option                   *m_option_http;
    static Config::Option                   *m_option_client_max_body_size;
    static Config::Option                   *m_option_error_page;
    static Config::Option                   *m_option_server;
    static Config::Option                   *m_option_listen;
    static Config::Option                   *m_option_server_name;
    static Config::Option                   *m_option_location;
    static Config::Option                   *m_option_allowed_methods;
    static Config::Option                   *m_option_root;
    static Config::Option                   *m_option_autoindex;
    static Config::Option                   *m_option_index;
    static Config::Option                   *m_option_cgi_extension;
    static Config::Option                   *m_option_upload_path;
    static Config::Option                   *m_option_return;

    static Config::Option::map_config_t&    _getConfigMap(void);
};

#endif
