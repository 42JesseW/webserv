/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: kfu <kfu@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/29 17:14:05 by kfu           #+#    #+#                 */
/*   Updated: 2022/01/31 12:10:55 by katherine     ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HPP
# define MAIN_HPP

# include <sys/socket.h>
# include <sys/types.h>
# include <signal.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <stdarg.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/time.h>
# include <sys/ioctl.h>
# include <netdb.h>

# define SERVER_PORT 18000
# define MAXLINE 4096
# define SA struct sockaddr
# define forever for(;;)

typedef enum e_Method
{
	GET,
	POST,
	DELETE,
	INVALID
} e_Method;

void err_n_die(const char *fmt, ...);

#endif
