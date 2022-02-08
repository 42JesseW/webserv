NAME = webserv

SRC =	main.cpp\
		srcs/Route.cpp\
		srcs/Server.cpp\
		srcs/Socket.cpp\
		srcs/Client.cpp\
		srcs/Request.cpp\
		srcs/ParseRequest.cpp\
		srcs/Response.cpp\
		srcs/GetResponse.cpp\
		srcs/StatusCodes.cpp

OBJS = $(SRC:.cpp=.o)
# CFLAGS = -Wall -Werror -Wextra -pedantic -std=c++98 
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c++98 -g -fsanitize=address 

CC = clang++

all: $(NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o:	%.cpp
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			rm -f $(OBJS)

fclean: 	clean
			rm -f $(NAME)

re: fclean all
