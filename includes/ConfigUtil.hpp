#ifndef CONFIGUTIL_HPP

# define CONFIGUTIL_HPP

# define DFL_ERROR_PAGES_PATH "conf/error_pages"

# include <Common.hpp>
# include <Utils.hpp>

class ConfigUtil
{
public:
    typedef std::vector<std::string>            methods_t;
    typedef std::pair<std::string, std::string> status_code_body_t;
    typedef std::map<int, status_code_body_t>   status_code_map_t;

private:
    status_code_map_t   m_status_codes;
    methods_t           m_methods;

    static ConfigUtil   *m_handle;

public:
    ~ConfigUtil();

private:
    ConfigUtil();
    ConfigUtil(const ConfigUtil& cpy);

public:
    static ConfigUtil&                      getHandle(void);

    const methods_t&                        getDefaultMethods(void);
    const status_code_map_t&                getStatusCodeMap(void);

private:
    void                                    _setDefaultStatusCodes(void);
    void                                    _setDefaultMethods(void);
    void                                    _loadDefaultErrorFiles(void);

};

#endif
