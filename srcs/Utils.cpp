#include <Utils.hpp>

#include <iostream>

namespace ft
{
    /*
     * // TODO testcase
     * Split the string based on the delimiter(s) and push the
     * individual tokens into the specified container.
     */
    std::deque<std::string>&    tokenizeString(std::deque<std::string>& queue, std::string& str, const std::string& delims)
    {
        std::string             sub;
        std::string::size_type  pos, prev;

        prev = 0;
        while ((pos = str.find_first_of(delims, prev)) != std::string::npos)
        {
            if (pos > prev)
            {
                sub = str.substr(prev, pos - prev);
                if (queue.empty() || !(sub == "\n" && queue.back() == "\n"))
                    queue.push_back(str.substr(prev, (pos - prev)));
            }
            prev = (pos + 1);
        }
        if (prev < str.length())
            queue.push_back(str.substr(prev, std::string::npos));
        return (queue);
    }

    /*
     * // TODO testcase
     * Replace a substring in a string with another substring using
     * built-in string methods find() and replace().
     */
    std::string&    stringReplaceMultiple(std::string& str, const std::string& from, const std::string& to)
    {
        std::string::size_type  index;

        index = 0;
        while (true) {
            index = str.find(from, index);
            if (index == std::string::npos)
                break;
            str.replace(index, from.size(), to);
            index += to.size();
        }
        return (str);
    }
}
