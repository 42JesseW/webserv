#ifndef UTILS_HPP

# define UTILS_HPP

# include <deque>
# include <string>

namespace ft
{
    std::deque<std::string>&    tokenizeString(std::deque<std::string>& queue, std::string& str, const std::string& delims);
    std::string&                stringReplaceMultiple(std::string& str, const std::string& from, const std::string& to);
}

#endif
