/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Common.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: kfu <kfu@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/01/29 17:14:05 by kfu           #+#    #+#                 */
/*   Updated: 2022/02/09 14:00:41 by katherine     ########   odam.nl         */
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
# include <iostream>
# include <fstream>
# include <map>
# include <vector>

# define SERVER_PORT 1800
# define MAXLINE 4096
# define SA struct sockaddr
# define forever for(;;)

void err_n_die(const char *fmt, ...);

#endif