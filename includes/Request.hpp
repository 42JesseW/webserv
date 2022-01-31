/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:04:57 by katherine     #+#    #+#                 */
/*   Updated: 2022/01/31 10:48:38 by katherine     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include <vector>
# include "Response.hpp"
# include "Request.hpp"

class Request
{
	private:
		e_Method 					m_method;
		std::string					m_start_line;
		std::vector<std::string>	m_header_lines;
		std::vector<std::string>	m_body_lines;

	public:
		Request();
		Request(const Request &Copy);
		~Request();

		Request& 					operator = (const Request &Copy);

		e_Method					getMethod();
		std::string					getStartLine();
		std::vector<std::string>	getHeader();
		std::vector<std::string>	getBody();
};

#endif