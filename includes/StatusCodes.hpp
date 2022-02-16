#ifndef STATUS_CODES_HPP
# define STATUS_CODES_HPP

# define code_pair std::pair<int, std::string>

void	initStatusCodes()
{
	std::map<int, std::string> status_codes;

	//------ 1xx - Informational ------
	status_codes.insert(code_pair(100, "Continue"));
	status_codes.insert(code_pair(101, "Switching Protocols"));
	status_codes.insert(code_pair(102, "Processing"));
	status_codes.insert(code_pair(103, "Early Hints"));

	//------ 2xx - Successful ------
	status_codes.insert(code_pair(200, "OK"));
	status_codes.insert(code_pair(201, "Created"));
	status_codes.insert(code_pair(202, "Accepted"));
	status_codes.insert(code_pair(203, "Non-Authoritative Information"));
	status_codes.insert(code_pair(204, "No Content"));
	status_codes.insert(code_pair(205, "Reset Content"));
	status_codes.insert(code_pair(206, "Partial Content"));
	status_codes.insert(code_pair(207, "Multi-Status"));
	status_codes.insert(code_pair(208, "Already Reported"));
	status_codes.insert(code_pair(226, "IM Used"));

	//------ 3xx - Redirection ------
	status_codes.insert(code_pair(300, "Multiple Choices"));
	status_codes.insert(code_pair(301, "Moved Permanently"));
	status_codes.insert(code_pair(302, "Found"));
	status_codes.insert(code_pair(303, "See Other"));
	status_codes.insert(code_pair(304, "Not Modified"));
	status_codes.insert(code_pair(305, "Use Proxy"));
	status_codes.insert(code_pair(307, "Temporary Redirect"));
	status_codes.insert(code_pair(308, "Permanent Redirect"));

 //------ 4xx - Client Error ------
	status_codes.insert(code_pair(400, "Bad Request"));
	status_codes.insert(code_pair(401, "Unauthorized"));
	status_codes.insert(code_pair(402, "Payment Required"));
	status_codes.insert(code_pair(403, "Forbidden"));
	status_codes.insert(code_pair(404, "Not Found"));
	status_codes.insert(code_pair(405, "Method Not Allowed"));
	status_codes.insert(code_pair(406, "Not Acceptable"));
	status_codes.insert(code_pair(407, "Proxy Authentication Required"));
	status_codes.insert(code_pair(408, "Request Timeout"));
	status_codes.insert(code_pair(409, "Conflict"));
	status_codes.insert(code_pair(410, "Gone"));
	status_codes.insert(code_pair(411, "Length Required"));
	status_codes.insert(code_pair(412, "Precondition Failed"));
	status_codes.insert(code_pair(413, "Content Too Large"));
	status_codes.insert(code_pair(414, "URI Too Long"));
	status_codes.insert(code_pair(415, "Unsupported Media Type"));
	status_codes.insert(code_pair(416, "Range Not Satisfiable"));
	status_codes.insert(code_pair(417, "Expectation Failed"));
	status_codes.insert(code_pair(418, "I'm a teapot"));
	status_codes.insert(code_pair(421, "Misdirected Request"));
	status_codes.insert(code_pair(422, "Unprocessable Content"));
	status_codes.insert(code_pair(423, "Locked"));
	status_codes.insert(code_pair(424, "Failed Dependency"));
	status_codes.insert(code_pair(425, "Too Early"));
	status_codes.insert(code_pair(426, "Upgrade Required"));
	status_codes.insert(code_pair(428, "Precondition Required"));
	status_codes.insert(code_pair(429, "Too Many Requests"));
	status_codes.insert(code_pair(431, "Request Header Fields Too Large"));
	status_codes.insert(code_pair(451, "Unavailable For Legal Reasons"));

	//------ 5xx - Server Error ------
	status_codes.insert(code_pair(500, "Internal Server Error"));
	status_codes.insert(code_pair(501, "Not Implemented"));
	status_codes.insert(code_pair(502, "Bad Gateway"));
	status_codes.insert(code_pair(503, "Service Unavailable"));
	status_codes.insert(code_pair(504, "Gateway Timeout"));
	status_codes.insert(code_pair(505, "HTTP Version Not Supported"));
	status_codes.insert(code_pair(506, "Variant Also Negotiates"));
	status_codes.insert(code_pair(507, "Insufficient Storage"));
	status_codes.insert(code_pair(508, "Loop Detected"));
	status_codes.insert(code_pair(510, "Not Extended"));
	status_codes.insert(code_pair(511, "Network Authentication Required"));
}

#endif
