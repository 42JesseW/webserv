NAME        = webserv

SHELL		= /bin/bash

SOURCE_DIR  = srcs
HEADER_DIR  = includes
TEST_DIR    = tests
LIB_BASE	= libs
CMAKE_DIR   = cmake-build

CFLAGS      = -Wall -Werror -Wextra -pedantic -g -fsanitize=address
CLINKS      = -lpthread

CURL_VERSION		= 7.82.0
CURL_DOWNLOAD		= https://curl.se/download/curl-7.82.0.tar.gz

OPENSSL_VERSION		= 3.0.2
OPENSSL_DOWNLOAD	= https://www.openssl.org/source/openssl-3.0.2.tar.gz

GTEST_VERSION		= 1.11.0
GTEST_DOWNLOAD		= https://github.com/google/googletest/archive/refs/tags/release-1.11.0.tar.gz

TST_HEADER_DIR		= $(HEADER_DIR) tests/unittests
TST_HEADER_DIR		+= $(PWD)/$(LIB_BASE)/openssl/includes
TST_HEADER_DIR		+= $(PWD)/$(LIB_BASE)/curl/includes
TST_HEADER_DIR		+= $(PWD)/$(LIB_BASE)/googletest/googletest/include

LIB_DIR		= $(PWD)/$(LIB_BASE)/openssl/lib
LIB_DIR		+= $(PWD)/$(LIB_BASE)/curl/lib
LIB_DIR		+= $(PWD)/$(LIB_BASE)/googletest/lib

SRC         = $(SOURCE_DIR)/ConfigUtil.cpp \
              $(SOURCE_DIR)/Socket.cpp \
              $(SOURCE_DIR)/ClientSocket.cpp \
              $(SOURCE_DIR)/ServerSocket.cpp \
              $(SOURCE_DIR)/FileParser.cpp \
              $(SOURCE_DIR)/Option.cpp \
              $(SOURCE_DIR)/PortConfig.cpp \
              $(SOURCE_DIR)/Poller.cpp \
              $(SOURCE_DIR)/ServerConfig.cpp \
              $(SOURCE_DIR)/Route.cpp \
              $(SOURCE_DIR)/Connection.cpp \
              $(SOURCE_DIR)/Signals.cpp \
              $(SOURCE_DIR)/CGI.cpp \
              $(SOURCE_DIR)/Utils.cpp \
              $(SOURCE_DIR)/handler/Handler.cpp\
              $(SOURCE_DIR)/handler/DeleteHandler.cpp\
              $(SOURCE_DIR)/handler/PostHandler.cpp\
              $(SOURCE_DIR)/request/HandleRequest.cpp \
              $(SOURCE_DIR)/request/ErrorHandling.cpp \
              $(SOURCE_DIR)/request/Request.cpp \
              $(SOURCE_DIR)/request/Decoder.cpp \
              $(SOURCE_DIR)/response/Response.cpp

TST_SRC		= $(TEST_DIR)/unittests/test_cgi.cpp \
			  $(TEST_DIR)/unittests/test_connection.cpp \
			  $(TEST_DIR)/unittests/test_parser.cpp \
			  $(TEST_DIR)/unittests/test_request.cpp \
			  $(TEST_DIR)/unittests/test_socket.cpp \
			  $(TEST_DIR)/unittests/utils.cpp

DFL_SRC		= $(SRC) $(SOURCE_DIR)/main.cpp

OBJECTS		= $(patsubst %.cpp, %.o, $(SRC))
DFL_OBJECTS	= $(patsubst %.cpp, %.o, $(DFL_SRC))
DEPENDS		= $(patsubst %.cpp, %.d, $(SRC))
HEADERS     = $(addprefix -I, $(HEADER_DIR))
LIBS		= $(addprefix -L, $(LIB_DIR))

TST_OBJECTS	= $(patsubst %.cpp, %.o, $(TST_SRC))
TST_HEADERS	= $(addprefix -I, $(TST_HEADER_DIR))

all: $(NAME)

$(NAME): $(DFL_OBJECTS)
	$(CXX) $(DFL_OBJECTS) -o $@ $(CLINKS) $(HEADERS) $(CFLAGS)

-include $(DEPENDS)

$(SOURCE_DIR)/%.o: $(SOURCE_DIR)/%.cpp Makefile
	$(CXX) -std=c++11 $(CFLAGS) -MMD -MP -c $< -o $@ $(HEADERS)

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) -std=c++11 -fsanitize=address -c $< -o $@ $(TST_HEADERS)

clean:
	@rm -f $(OBJECTS) $(TST_OBJECTS) $(DEPENDS)

fclean: clean
	@rm -f $(NAME) tester

uninstall: fclean
	@if [ -d $(LIB_BASE)/googletest ]; then cd $(LIB_BASE)/googletest && cmake --build . --target clean; fi
	@rm -rf $(LIB_BASE)/curl
	@make -C $(LIB_BASE)/openssl-$(OPENSSL_VERSION) clean
	@rm -rf $(LIB_BASE)/openssl
	@make -C $(LIB_BASE)/curl-$(CURL_VERSION) clean

re: fclean all

$(LIB_BASE)/googletest/lib/libgtest.a:
	# download and unpack googletest if not retreived yet
	@if [ ! -d $(LIB_BASE)/googletest ]; then \
  		curl -L $(GTEST_DOWNLOAD) --output $(LIB_BASE)/googletest-release-$(GTEST_VERSION).tar.gz; \
  		cd $(LIB_BASE) && tar -xzf googletest-release-$(GTEST_VERSION).tar.gz; \
  		mv $(LIB_BASE)/googletest-release-$(GTEST_VERSION) $(LIB_BASE)/googletest; \
  	fi
	@cd $(LIB_BASE)/googletest && cmake .
	@cd $(LIB_BASE)/googletest && cmake --build .

$(LIB_BASE)/curl/lib/libcurl.a:
	# download and unpack curl if not retrieved yet
	@if [ ! -d $(LIB_BASE)/curl-$(CURL_VERSION) ]; then \
  		curl -L $(CURL_DOWNLOAD) --output $(LIB_BASE)/curl-$(CURL_VERSION).tar.gz; \
  		cd $(LIB_BASE) && tar -xzf curl-$(CURL_VERSION).tar.gz; \
  	fi
	@mkdir -p $(LIB_BASE)/curl
	@cd $(LIB_BASE)/curl-$(CURL_VERSION) && \
		./configure \
			--prefix $(PWD)/$(LIB_BASE)/curl \
			--with-openssl=$(PWD)/$(LIB_BASE)/openssl
	@make -C $(LIB_BASE)/curl-$(CURL_VERSION)
	@make -C $(LIB_BASE)/curl-$(CURL_VERSION) install

$(LIB_BASE)/openssl/lib/libssl.a:
	# download and unpack openssl if not retrieved yet
	@if [ ! -d $(LIB_BASE)/openssl-$(OPENSSL_VERSION) ]; then \
  		curl -L $(OPENSSL_DOWNLOAD) --output $(LIB_BASE)/openssl-$(OPENSSL_VERSION).tar.gz; \
  		cd $(LIB_BASE) && tar -xzf openssl-$(OPENSSL_VERSION).tar.gz; \
  	fi
	@mkdir -p $(LIB_BASE)/openssl
	@cd $(LIB_BASE)/openssl-$(OPENSSL_VERSION) && ./Configure --prefix=$(PWD)/$(LIB_BASE)/openssl
	@make -C $(LIB_BASE)/openssl-$(OPENSSL_VERSION)
	@make -C $(LIB_BASE)/openssl-$(OPENSSL_VERSION) install_sw

# TODO add this to the github actions flow
tester: $(OBJECTS) $(LIB_BASE)/googletest/lib/libgtest.a $(LIB_BASE)/openssl/lib/libssl.a $(LIB_BASE)/curl/lib/libcurl.a $(TST_OBJECTS)
	$(CXX) $(OBJECTS) $(TST_OBJECTS) -o $@ $(CFLAGS) $(TST_HEADERS) $(LIBS) $(CLINKS) -lcurl -lgtest -lgtest_main

# mkdir libs/openssl
# libs/openssl-{version}/Configure --prefix=$PWD/libs/openssl
# make -C libs/openssl-{version}
# make -C libs/openssl-{version} install
# mkdir curl
# libs/curl-{version}/configure --prefix=$PWD/libs/curl --with-openssl=$PWD/libs/openssl
# make -C libs/curl-{version}
# make -C libs/curl-{version} install
# mv libs/googletest-release-{version} libs/googletest
# cmake libs/googletest-release-{version}
# cmake --build libs/googletest-release-{version}
