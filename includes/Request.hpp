/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:04:57 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/01 11:42:13 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Common.hpp"

# define stringMap std::map<std::string, std::string>
# define stringVector std::vector<std::string>

class Request
{
	private:
		e_Method 		m_method;
		std::string		m_start_line;
		stringMap		m_header_lines;
		stringVector	m_body_lines;

	public:
		Request();
		Request(const Request &Copy);
		~Request();

		Request& operator = (const Request &Copy);

		e_Method					getMethod();
		std::string					getStartLine();
		stringMap					getHeader();
		std::vector<std::string>	getBody();
};

#endif