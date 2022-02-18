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

	/* simple function to convert num into string TODO testcase */
	std::string		intToString(int num)
	{
	    std::ostringstream ss;

	    ss << num;
		return (ss.str());
	}

	int             stringToInt(const std::string& str)
    {
	    std::stringstream   num_stream;
	    int                 num_out;

	    num_stream.exceptions( std::ios::badbit | std::ios::failbit );
	    num_stream << str;
	    try
        {
            num_stream >> num_out;
        }
        catch (std::ios::failure& e)
        {
            num_out = FAIL_CONVERSION;
        }
	    return (num_out);
    }

    /* generalised function that completely reads out a file. file_stream.good() can be check for error */
    std::string     readFileContent(std::ifstream& file_stream, const std::string& file_name)
    {
	    std::string         file_contents;
        std::stringstream   file_content_stream;

        try
        {
            file_stream.open(file_name);
            file_content_stream << file_stream.rdbuf();
            file_contents = file_content_stream.str();
        }
        catch (const std::ios::failure& e)
        {
            std::cout << "[WARN] Could not find error_page file: " << file_name << '\n';
        }
        catch (...)
        {
            std::cerr << "[ERROR] Error opening file: " << file_name << '\n';
        }
        return (file_contents);
    }
}
