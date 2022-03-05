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

OBJS		= $(SRC:.cpp=.o)
CFLAGS		= -Wall -Werror -Wextra -std=c++98 -pedantic -g -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS)

%.o:	%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ -I $(INCLUDE_DIR)

clean:
	rm -f $(OBJS)

fclean: 	clean
	rm -f $(NAME)

re: fclean all
