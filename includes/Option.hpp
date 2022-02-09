#ifndef OPTION_HPP

# define OPTION_HPP

# include <map>
# include <deque>
# include <string>
# include <vector>
# include <algorithm>

class Option
{
private:
    int     m_parse_level;    /* parse_level at which to find option */

public:
    typedef std::deque<std::string>         tokens_t;
    typedef std::map<std::string, Option*>  map_config_t;

public:
    explicit Option(int parse_level);
    Option(const Option& cpy);
    ~Option();

    Option&         operator = (const Option& rhs);

    int             getParseLevel(void);

    // some function that can be used to modify either [Config, Server, Route] objects
    virtual void    parse(void *obj, tokens_t& tokens) = 0;

protected:
    static int                      m_client_max_body_size;

};

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

    OptionListen&     operator = (const OptionListen& rhs);

    void            parse(void *obj, tokens_t& tokens);

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

#endif
