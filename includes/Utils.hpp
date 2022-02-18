#ifndef UTILS_HPP

# define UTILS_HPP
# define FAIL_CONVERSION (-1)

# include <deque>
# include <string>
# include <sstream>
# include <fstream>

namespace ft
{
    std::deque<std::string>&    tokenizeString(std::deque<std::string>& queue, std::string& str, const std::string& delims);
    std::string&                stringReplaceMultiple(std::string& str, const std::string& from, const std::string& to);

	std::string					intToString(int num);
	int                         stringToInt(const std::string& str);
	std::string                 readFileContent(std::ifstream& file_stream, const std::string& file_name);
}

#endif
