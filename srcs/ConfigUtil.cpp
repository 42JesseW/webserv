#include <ConfigUtil.hpp>

ConfigUtil  *ConfigUtil::m_handle = NULL;

ConfigUtil::ConfigUtil(void) : m_signal(0)
{
    /*
     * use DFL_ERROR_PAGES_PATH from project directory to find .html files
     * - files are named <status_code>.html
     * - if file not found ignore it
     * - read each file into std::string and update
     * - Server has `m_error_pages` for custom pages
     * - If an error_page is request, Server first checks itself, then Config::getStatusCodeMap
     * - Config has static std::map from getStatusCodeMap for defaults
     * - this Config::Config sets defaults in std::map in Config
     * - while loading custom error_pages into Server.m_error_pages give
     *   warning if unable to find file
     */
    _setDefaultMethods();
    _setDefaultStatusCodes();
    _loadDefaultErrorFiles();
    pthread_mutex_init( &m_signal_lock, NULL );
}

ConfigUtil::~ConfigUtil(void)
{
    delete ConfigUtil::m_handle;
}

ConfigUtil&                             ConfigUtil::getHandle(void)
{
    if (!m_handle) m_handle = new ConfigUtil;
    return (*m_handle);
}

const ConfigUtil::methods_t&            ConfigUtil::getDefaultMethods(void)
{
    return (m_methods);
}

const ConfigUtil::status_code_map_t&    ConfigUtil::getStatusCodeMap(void)
{
    return (m_status_codes);
}

void                                    ConfigUtil::_setDefaultMethods(void)
{
    m_methods.push_back("GET");
    m_methods.push_back("POST");
    m_methods.push_back("DELETE");
}

void                                    ConfigUtil::_setDefaultStatusCodes(void)
{
    //------ 1xx - Informational ------
    m_status_codes.insert(std::make_pair(100, std::make_pair("Continue", "")));
    m_status_codes.insert(std::make_pair(101, std::make_pair("Switching Protocols", "")));
    m_status_codes.insert(std::make_pair(102, std::make_pair("Processing", "")));
    m_status_codes.insert(std::make_pair(103, std::make_pair("Early Hints", "")));

    //------ 2xx - Successful ------
    m_status_codes.insert(std::make_pair(200, std::make_pair("OK", "")));
    m_status_codes.insert(std::make_pair(201, std::make_pair("Created", "")));
    m_status_codes.insert(std::make_pair(202, std::make_pair("Accepted", "")));
    m_status_codes.insert(std::make_pair(203, std::make_pair("Non-Authoritative Information", "")));
    m_status_codes.insert(std::make_pair(204, std::make_pair("No Content", "")));
    m_status_codes.insert(std::make_pair(205, std::make_pair("Reset Content", "")));
    m_status_codes.insert(std::make_pair(206, std::make_pair("Partial Content", "")));
    m_status_codes.insert(std::make_pair(207, std::make_pair("Multi-Status", "")));
    m_status_codes.insert(std::make_pair(208, std::make_pair("Already Reported", "")));
    m_status_codes.insert(std::make_pair(226, std::make_pair("Im Used", "")));

    //------ 3xx - Redirection ------
    m_status_codes.insert(std::make_pair(300, std::make_pair("Multiple Choices", "")));
    m_status_codes.insert(std::make_pair(301, std::make_pair("Moved Permanently", "")));
    m_status_codes.insert(std::make_pair(302, std::make_pair("Found", "")));
    m_status_codes.insert(std::make_pair(303, std::make_pair("See Other", "")));
    m_status_codes.insert(std::make_pair(304, std::make_pair("Not Modified", "")));
    m_status_codes.insert(std::make_pair(305, std::make_pair("Use Proxy", "")));
    m_status_codes.insert(std::make_pair(307, std::make_pair("Temporary Redirect", "")));
    m_status_codes.insert(std::make_pair(308, std::make_pair("Permanent Redirect", "")));

    //------ 4xx - Client Error ------
    m_status_codes.insert(std::make_pair(400, std::make_pair("Bad Request", "conf/error_pages/400.html")));
    m_status_codes.insert(std::make_pair(401, std::make_pair("Unauthorized", "conf/error_pages/401.html")));
    m_status_codes.insert(std::make_pair(402, std::make_pair("Payment Required", "conf/error_pages/402.html")));
    m_status_codes.insert(std::make_pair(403, std::make_pair("Forbidden", "conf/error_pages/403.html")));
    m_status_codes.insert(std::make_pair(404, std::make_pair("Not Found", "conf/error_pages/404.html")));
    m_status_codes.insert(std::make_pair(405, std::make_pair("Method Not Allowed", "conf/error_pages/405.html")));
    m_status_codes.insert(std::make_pair(406, std::make_pair("Not Acceptable", "conf/error_pages/406.html")));
    m_status_codes.insert(std::make_pair(407, std::make_pair("Proxy Authentication Required", "conf/error_pages/407.html")));
    m_status_codes.insert(std::make_pair(408, std::make_pair("Request Timeout", "conf/error_pages/408.html")));
    m_status_codes.insert(std::make_pair(409, std::make_pair("Conflict", "conf/error_pages/409.html")));
    m_status_codes.insert(std::make_pair(410, std::make_pair("Gone", "conf/error_pages/410.html")));
    m_status_codes.insert(std::make_pair(411, std::make_pair("Length Required", "conf/error_pages/411.html")));
    m_status_codes.insert(std::make_pair(412, std::make_pair("Precondition Failed", "conf/error_pages/412.html")));
    m_status_codes.insert(std::make_pair(413, std::make_pair("Content Too Large", "conf/error_pages/413.html")));
    m_status_codes.insert(std::make_pair(414, std::make_pair("URI Too Long", "conf/error_pages/414.html")));
    m_status_codes.insert(std::make_pair(415, std::make_pair("Unsupported Media Type", "conf/error_pages/415.html")));
    m_status_codes.insert(std::make_pair(416, std::make_pair("Range Not Satisfiable", "conf/error_pages/416.html")));
    m_status_codes.insert(std::make_pair(417, std::make_pair("Expectation Failed", "conf/error_pages/417.html")));
    m_status_codes.insert(std::make_pair(418, std::make_pair("I'm a teapot", "conf/error_pages/418.html")));
    m_status_codes.insert(std::make_pair(421, std::make_pair("Misdirected Request", "conf/error_pages/421.html")));
    m_status_codes.insert(std::make_pair(422, std::make_pair("Unprocessable Content", "conf/error_pages/422.html")));
    m_status_codes.insert(std::make_pair(423, std::make_pair("Locked", "conf/error_pages/423.html")));
    m_status_codes.insert(std::make_pair(424, std::make_pair("Failed Dependency", "conf/error_pages/424.html")));
    m_status_codes.insert(std::make_pair(425, std::make_pair("Too Early", "conf/error_pages/425.html")));
    m_status_codes.insert(std::make_pair(426, std::make_pair("Upgrade Required", "conf/error_pages/426.html")));
    m_status_codes.insert(std::make_pair(428, std::make_pair("Precondition Required", "conf/error_pages/428.html")));
    m_status_codes.insert(std::make_pair(429, std::make_pair("Too Many Requests", "conf/error_pages/429.html")));
    m_status_codes.insert(std::make_pair(431, std::make_pair("Request Header Fields Too Large", "conf/error_pages/431.html")));
    m_status_codes.insert(std::make_pair(451, std::make_pair("Unavailable For Legal Reasons", "conf/error_pages/451.html")));

    //------ 5xx - Server Error ------
    m_status_codes.insert(std::make_pair(500, std::make_pair("Internal Server Error", "conf/error_pages/500.html")));
    m_status_codes.insert(std::make_pair(501, std::make_pair("Not Implemented", "conf/error_pages/501.html")));
    m_status_codes.insert(std::make_pair(502, std::make_pair("Bad Gateway", "conf/error_pages/502.html")));
    m_status_codes.insert(std::make_pair(503, std::make_pair("Service Unavailable", "conf/error_pages/503.html")));
    m_status_codes.insert(std::make_pair(504, std::make_pair("Gateway Timeout", "conf/error_pages/504.html")));
    m_status_codes.insert(std::make_pair(505, std::make_pair("HTTP Version Not Supported", "conf/error_pages/505.html")));
    m_status_codes.insert(std::make_pair(506, std::make_pair("Variant Also Negotiates", "conf/error_pages/506.html")));
    m_status_codes.insert(std::make_pair(507, std::make_pair("Insufficient Storage", "conf/error_pages/507.html")));
    m_status_codes.insert(std::make_pair(508, std::make_pair("Loop Detected", "conf/error_pages/508.html")));
    m_status_codes.insert(std::make_pair(510, std::make_pair("Not Extended", "conf/error_pages/510.html")));
    m_status_codes.insert(std::make_pair(511, std::make_pair("Network Authentication Required", "conf/error_pages/511.html")));
}

void                                    ConfigUtil::_loadDefaultErrorFiles(void)
{
    std::stringstream           file_name_ss;
    std::string                 file_name;
    std::string                 buffer;
    std::string                 file_contents;
    std::ifstream               file_stream;

    status_code_map_t::iterator it = m_status_codes.begin();
    for ( status_code_map_t::value_type code ;  it != m_status_codes.end() ; ++it )
    {

        /* will translate to <location_of_executable>/conf/error_pages/<status_code>.html */
        file_name_ss << "../../" << DFL_ERROR_PAGES_PATH << "/" << code.first << ".html";
        file_name = file_name_ss.str();
        file_name_ss.str( std::string() );

        file_contents = ft::readFileContent(file_stream, file_name);
        if (!file_stream.good() || file_contents.empty())
            continue ;
        m_status_codes[code.first] = std::make_pair(code.second.first, file_contents);
    }
}

void                                    ConfigUtil::setSignalled(int sig)
{
    pthread_mutex_lock( &m_signal_lock );
    m_signal = sig;
    pthread_mutex_unlock( &m_signal_lock );
}

bool                                    ConfigUtil::isSignalled(void)
{
    bool    ret;

    pthread_mutex_lock( &m_signal_lock );
    ret = (m_signal != 0);
    pthread_mutex_unlock( &m_signal_lock );
    return (ret);
}
