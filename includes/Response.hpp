#pragma once

#include <Webserv.hpp>
#include <ConfigUtil.hpp>
#include <Request.hpp>
#include <Utils.hpp>
#include <Route.hpp>

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

#define RETRY_AFTER_SEC "120"
/*
 * An unknown file type should use this type. Browsers pay a particular
 * care when manipulating these files, to protect users from software
 * vulnerabilities and possible dangerous behavior.
 */
#define DFL_MIME_TYPE   "application/octet-stream"
#define MIME_TEXT_HTML  "text/html"

typedef std::pair<std::string, std::string> string_pair_t;

class Response
{
private:
    Request 								m_request;
    Route									m_route;
    int										m_status_code;
    std::string								m_start_line;
    std::map<std::string, std::string>		m_headers_map;
    std::string								m_headers_str;
    std::string								m_body;
    std::string 							m_response;

public:
    Response(void);
    Response(Request &re, Route &ro);
    Response(const Response &copy);
    ~Response(void);

    Response& operator = (const Response &copy);

    // Getters
    Request const &									    getRequest(void) const;
    Route const &											getRoute(void) const;
    int const &												getStatusCode(void) const;
    std::string const &										getStartLine(void) const;
    std::map<std::string, std::string> const &				getHeadersMap(void) const;
    std::string	const &										getHeadersStr(void) const;
    std::string	const &										getBody(void) const;
    std::string const &										getResponse(void) const;

    void			buildStartLine(ConfigUtil::status_code_map_t& m_error_files);
    void			buildHeaders(void);
    void			buildBody(void);
    void			buildResponse(ConfigUtil::status_code_map_t& m_error_files);

private:
    string_pair_t	_buildDate(void);
    string_pair_t	_buildLocation(void);
    string_pair_t	_buildRetryAfter(void);
    string_pair_t	_buildAllow(void);
    string_pair_t	_buildServer(void);
    string_pair_t	_buildConnection(void);
    string_pair_t	_buildContentLength(void);
    string_pair_t	_buildContentType(void);
    string_pair_t	_buildTransferEncoding(void);
    int				_readFileIntoString(const std::string &path);
    
};
