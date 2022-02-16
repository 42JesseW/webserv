#ifndef STATUS_CODES_HPP
# define STATUS_CODES_HPP

# define code_pair std::pair<int, std::string>

# include <string>
# include <map>

class StatusCodes
{
	private:
		std::map<int, std::string> m_status_codes;
	public:
		StatusCodes();
		StatusCodes(const StatusCodes &copy);
		StatusCodes & operator=(const StatusCodes &copy);

		~StatusCodes();

		std::map<int, std::string>      getStatusCodesMap();
		std::map<int, std::string>		initStatusCodes();
};

#endif
