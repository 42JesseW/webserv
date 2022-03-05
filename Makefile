NAME = webserv

SRC =   main.cpp\
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

TEST =	tests/unittests/main.cpp\
		tests/unittests/test_request.cpp\
		srcs/request/Request.cpp\
		srcs/request/HandleRequest.cpp\
		srcs/request/ErrorHandling.cpp\

OBJS = $(SRC:.cpp=.o)
TEST_OBJS = $(TEST:.cpp=.o)
CFLAGS = -Wall -Werror -Wextra -std=c++98 -pedantic -g -fsanitize=address
TFLAGS = -Wall -Werror -Wextra -std=c++11 -pedantic -g -fsanitize=address

all: $(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(TFLAGS) -c $< -o $@ -I includes

test: $(TEST_OBJS)
	$(CXX) $(TFLAGS) -o $(NAME) $(TEST_OBJS)

clean:
	rm -f $(OBJS) $(TEST_OBJS) a.out

fclean: 	clean
	rm -f $(NAME)

re: fclean all
