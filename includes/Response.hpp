/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:04:57 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/01 13:44:09 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Common.hpp"

# define stringVector std::vector<std::string>
# define intStringMap std::map<int, std::string>

class Response
{
	private:
		e_Method 			m_method;
		intStringMap*		m_status_codes;
		std::string			m_start_line;
		stringVector		m_header_lines;
		std::string			m_body_lines;
		std::string 		m_response;

	public:
		Response();
		Response(const Response &copy);
		~Response();

		Response& operator = (const Response &copy);

		e_Method		getMethod();
		std::string		getResponse();
};

#endif