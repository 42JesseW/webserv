#include <Response.hpp>

std::string	Response::_buildFilePath(const std::string &filename, int file_flag)
{
	std::string path;

	if (file_flag < HTTP_STATUS_BAD_REQUEST)
		path = m_request.m_filesearch + filename;
	else
		path = DFL_ERROR_PAGES_PATH + filename + FILE_EXTENSION;

	return (path);
}

int			Response::_readFileIntoString(const std::string &path)
{
	std::ifstream input_file(path.c_str());
	if (!input_file.is_open())
		return (0);

	m_body = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
	return (1);
}

void		Response::_buildBodyGet()
{
	std::string path;
	std::vector<std::string>			path_vector;
	std::vector<std::string>::iterator	iter;

	if (m_request.getFilename().empty())
	{
		path_vector = m_route.getIndexFiles();
		for (iter = path_vector.begin(); iter != path_vector.end(); ++iter)
		{
			path = _buildFilePath(path_vector.at(iter - path_vector.begin()), m_status_code);
			if (_readFileIntoString(path))
				break;	
		}
	}
	else
	{
		path = _buildFilePath(m_request.getFilename(), m_status_code);
		_readFileIntoString(path);	
	}
}

void			Response::_buildBodyPost(void)
{
	std::string path;

	path = _buildFilePath(POST_FILENAME, m_status_code);
	_readFileIntoString(path);
}

void			Response::_buildBodyDelete(void)
{
	std::string path;

	path = _buildFilePath(DELETE_FILENAME, m_status_code);
	_readFileIntoString(path);
}

void			Response::_buildBodyError(void)
{
	std::string path;

	path = _buildFilePath(ft::intToString(m_status_code), m_status_code);
	_readFileIntoString(path);
}

void			Response::buildBody()
{
	if (m_status_code < HTTP_STATUS_BAD_REQUEST)
	{
		if (m_request.getMethod() == "GET")
			_buildBodyGet();
		else if (m_request.getMethod() == "POST")
			_buildBodyPost();
		else if (m_request.getMethod() == "DELETE")
			_buildBodyDelete();
	}
	else
		_buildBodyError();
}
