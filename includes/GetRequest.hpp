/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   GetRequest.hpp                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: katherine <katherine@student.codam.nl>       +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/09 14:05:49 by katherine     #+#    #+#                 */
/*   Updated: 2022/02/11 17:31:41 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

class GetRequest : public Request
{
	private:
		std::string								m_route;
		std::string								m_port;
		std::string    							m_query;
		bool									m_connection;
		std::map<std::string, std::string>		m_headers;
		std::map<std::string, std::string>		m_matches;
}
