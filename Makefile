NAME = webserv

SRC =	main.cpp\
		srcs/Route.cpp\
		srcs/Server.cpp\
		srcs/Socket.cpp\
		srcs/Client.cpp\
		Request/ParseRequest.cpp\
		Request/ErrorHandling.cpp\
		Request/Request.cpp

OBJS = $(SRC:.cpp=.o)
FT_OBJS = $(FT_SRC:.cpp=.o)
CFLAGS = -Wall -Wextra -pedantic -g -fsanitize=address 

CC = clang++

all: $(NAME)

$(NAME):	$(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o:	%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I includes

clean:
	rm -f $(OBJS)

fclean: 	clean
	rm -f $(NAME)

re: fclean all
