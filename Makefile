NAME		= webserv

INCLUDE_DIR	= includes

SRC_DIR		= srcs
SRC			= $(SRC_DIR)/main.cpp \
		  	  $(SRC_DIR)/Config.cpp \
		  	  $(SRC_DIR)/Option.cpp \
		  	  $(SRC_DIR)/Utils.cpp \
          	  $(SRC_DIR)/Route.cpp \
		  	  $(SRC_DIR)/Server.cpp \
			  $(SRC_DIR)/Socket.cpp \
			  $(SRC_DIR)/Client.cpp \
			  $(SRC_DIR)/ConfigUtil.cpp \
			  $(SRC_DIR)/request/ParseRequest.cpp \
			  $(SRC_DIR)/request/ErrorHandling.cpp \
			  $(SRC_DIR)/request/Request.cpp \
			  $(SRC_DIR)/response/Response.cpp \
			  $(SRC_DIR)/response/GetResponse.cpp
        

TEST =	tests/unittests/main.cpp\
		tests/unittests/test_request.cpp\
		srcs/request/Request.cpp\
		srcs/request/HandleRequest.cpp\
		srcs/request/ErrorHandling.cpp\
    
TEST_OBJS = $(TEST:.cpp=.o)

OBJS		= $(SRC:.cpp=.o)
CFLAGS		= -Wall -Werror -Wextra -std=c++98 -pedantic -g -fsanitize=address
TFLAGS    = -Wall -Werror -Wextra -std=c++11 -pedantic -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS)

%.o:	%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

test: $(TEST_OBJS)
	$(CXX) $(TFLAGS) -o $(NAME) $(TEST_OBJS)

clean:
	rm -f $(OBJS) $(TEST_OBJS) a.out

fclean: 	clean
	rm -f $(NAME)

re: fclean all
