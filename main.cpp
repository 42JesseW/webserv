/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: kfu <kfu@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/29 12:29:41 by kfu           #+#    #+#                 */
/*   Updated: 2022/02/02 17:55:50 by kfu           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"
#include "Response.hpp"

int     main(void)
{
    Request NewRequest;
    int fd = open("Request/RequestFile.txt", O_RDONLY);

    NewRequest.copyRequest(fd);
    NewRequest.printRequest();
    return (0);
}
