/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Response.hpp                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:04:57 by katherine     #+#    #+#                 */
/*   Updated: 2022/01/31 12:29:40 by katherine     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include <iostream>
# include <fstream>
# include <map>
# include <vector>
# include "main.hpp"

class Response
{
	private:
		e_Method 					m_method;
		std::map<int, std::string>	m_status_codes;
		std::string					m_start_line;
		std::vector<std::string>	m_header_lines;
		std::vector<std::string>	m_body_lines;
		std::ofstream 				m_response_file;

	public:
		Response();
		Response(const Response &copy);
		~Response();

		Response& operator = (const Response &copy);

		e_Method		getMethod();
		std::ofstream	getResponseFile();
		void			writeToResponseFile(std::string content);
		void			writeToResponseFile(std::vector<std::string> content);
};

#endif