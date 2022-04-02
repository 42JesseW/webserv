#pragma once

/*
 * You can convert numbers to strings using Stringizing:
 * https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
 */
#define STR(x)          #x

#define Webserv_VERSION_MAJOR STR(1)
#define Webserv_VERSION_MINOR STR(1)

#define PROG_NAME       "Websurf/" Webserv_VERSION_MAJOR "." Webserv_VERSION_MINOR " (Unix)"

#define SYS_ERROR       (-1)
#define HTTP_VERSION    "HTTP/1.1"
#define CR              "\r"
#define LF              "\n"

#define DFL_SERVER_HOST "*"     /* translates to INADDR_ANY 0.0.0.0 */

#include <exception>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <pthread.h>

typedef void            *(*THREAD_FUNC_PTR)(void *);
