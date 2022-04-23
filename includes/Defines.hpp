#pragma once

#define CR "\r"
#define LF "\n"
#define HTTP_VERSION   "HTTP/1.1"
#define CGI_VERSION    "CGI/1.1"

#define DFL_CGI_DIR    "cgi-bin/"
#define RECV_SIZE      100

#define DFL_FILE_SEARCH_PATH   "html/"
#define DFL_UPLOAD_PATH        "html/"
#define DFL_INDEX_FILE         "index.html"

#include <poll.h>
#include <sys/poll.h>

#include <iostream>
#include <sstream>
#include <fstream>

#include <vector>
#include <stack>

#include <csignal>
#include <cstring>
#include <unistd.h>