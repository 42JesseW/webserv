#include <Response.hpp>

// void					route_printing(Route *m_route)
// {
// 	std::cout << "---------- ROUTE PRINTING -----------" << std::endl;
	// std::cout << "base url: " << m_route->getBaseUrl() << std::endl;
	// std::cout << "m_file_search_path: " << m_route->getFileSearchPath() << std::endl;
	// std::cout << "m_upload_path: " << m_route->getUploadPath() << std::endl;
	// std::cout << "m_has_autoindex: " << m_route->hasAutoIndex() << std::endl;

	// std::vector<std::string>::iterator iter;

	// std::vector<std::string> accepted_methods;
	// std::cout << "m_accepted_methods: " << std::endl;
	// accepted_methods = m_route->getAcceptedMethods();
	// for (iter = accepted_methods.begin(); iter < accepted_methods.end(); ++iter)
	// 	std::cout << *iter << std::endl;

	// std::vector<std::string> index_files;
	// std::cout << "m_index_files: " << std::endl;
	// index_files = m_route->getIndexFiles();
	// for (iter = index_files.begin(); iter < index_files.end(); ++iter)
	// 	std::cout << *iter << std::endl;

	// std::vector<std::string> cgi_file_extensions;
	// std::cout << "m_cgi_file_extensions: " << std::endl;
	// cgi_file_extensions = m_route->getCgiFileExtensions();
	// for (iter = cgi_file_extensions.begin(); iter < cgi_file_extensions.end(); ++iter)
	// 	std::cout << *iter << std::endl;

// 	REDIR *m_redir;

// 	m_redir = m_route->getRedir();
// 	std::string url = m_redir->url;
// 	int status_code = m_redir->status_code;
// 	std::cout << "redirect url: " << url << std::endl;
// 	std::cout << "redirect status_code: " << status_code << std::endl;
// }

void					Response::buildStartLine(ConfigUtil::status_code_map_t& m_error_files)
{
	std::string									str_status_code;
	std::string 								reason_phrase;
	std::string									white_space;
	ConfigUtil::status_code_map_t::iterator		it;

	// route_printing(&m_route);
	m_status_code = m_request.getStatus();
	str_status_code = ft::intToString(m_status_code);

	it = m_error_files.find(m_status_code);
	if (it != m_error_files.end())
		reason_phrase = it->second.first;
	else
		throw ResponseFailure();

	white_space = " ";
	m_start_line = HTTP_VERSION
				+ white_space
				+ str_status_code
				+ white_space
				+ reason_phrase
				+ (CR LF);
}
