/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Request.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/31 10:04:57 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/02 17:55:02 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "Common.hpp"

# define BUFF_SIZE 1024
# define ENDLINE '\n'

class Request
{
	private:
		e_Method 								m_method;
		std::string								m_request;
		std::string								m_start_line;
		std::map<std::string, std::string>		m_header;
		std::string								m_body;

	public:
		Request();
		Request(const Request &Copy);
		~Request();

		Request& operator = (const Request &Copy);

		// Getters
		std::string							getRequest();
		e_Method							getMethod();
		std::string							getStartLine();
		std::map<std::string, std::string>	getHeader();
		std::string							getBody();

		// Parsing
		void								copyRequest(int socket);
		void								divideRequest();
		void 								parseAndSetStartLine();
		void 								parseAndSetHeader();
		void								printRequest();
};

#endif