#include <Response.hpp>

std::string				Response::_buildFilePath()
{
	std::string path;


	return (path);
}

int 					Response::_readFileIntoString(const std::string &path, int error_file)
{
	std::string new_path;

	// IF NOT NECESSARY IF REFACTORED?
	if (error_file == HTML_FILE_FLAG)
	{
		new_path = m_request.m_filesearch + path;
		if (new_path.at(0) == '/')
			new_path.erase(0, 1);
	}
	else
		new_path = path;

	std::ifstream input_file(new_path.c_str());
	if (!input_file.is_open())
		return (0);

	m_body = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
	return (1);
}

void					Response::_buildBodyGet()
{
	if (m_status_code == HTTP_STATUS_OK)
	{
		std::string							path;
		std::vector<std::string>			path_vector;
		std::vector<std::string>::iterator	iter;

		if (m_request.getFilename().empty())
		{
			path_vector = m_route.getIndexFiles();
			path = m_route.getFileSearchPath();
			for (iter = path_vector.begin(); iter != path_vector.end(); ++iter)
			{
				if (_readFileIntoString(path_vector.at(iter - path_vector.begin()), HTML_FILE_FLAG))
					break;	
			}
		}
		else
			_readFileIntoString(m_request.getFilename(), HTML_FILE_FLAG);	
	}

	if (m_status_code == HTTP_STATUS_NOT_FOUND)
	{
		std::string	path;

		path = DFL_ERROR_PAGES_PATH + ft::intToString(m_status_code) + ".html";
		_readFileIntoString(path, ERROR_FILE_FLAG);
	}
}

void					Response::buildBody(ConfigUtil::status_code_map_t& m_error_files)
{
	(void)m_error_files;
	if (m_request.getMethod() == "GET")
		_buildBodyGet();
	else if (m_request.getMethod() == "POST")
		_buildBodyPost();
	else if (m_request.getMethod() == "DELETE")
		_buildBodyDelete();
}
