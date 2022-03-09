NAME    = webserv
CFLAGS  = -Wall -Werror -Wextra -pedantic -g -fsanitize=address

ifndef TEST
	SRC =   srcs/main.cpp\
	        srcs/Config.cpp\
	        srcs/Option.cpp\
	        srcs/Utils.cpp\
	        srcs/Route.cpp\
	        srcs/Server.cpp\
            srcs/Socket.cpp\
            srcs/Client.cpp\
            srcs/ConfigUtil.cpp\
            srcs/request/HandleRequest.cpp\
            srcs/request/ErrorHandling.cpp\
            srcs/request/Request.cpp\
            srcs/response/Response.cpp\
            srcs/response/GetResponse.cpp

    CFLAGS  += -std=c++98
else
    SRC =   tests/unittests/main.cpp\
	        tests/unittests/test_request.cpp\
            srcs/request/Request.cpp\
	        srcs/request/HandleRequest.cpp\
	        srcs/request/ErrorHandling.cpp

    CFLAGS  += -std=c++11
    NAME    = tester
endif

OBJS = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ -I includes

test: $(OBJS)
	$(MAKE) TEST=1

clean:
	rm -f $(OBJS) $(TEST_OBJS)

fclean: 	clean
	rm -f $(NAME)

re: fclean all
