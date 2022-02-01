NAME = webserv

SRC =	main.cpp\
		classes/Config.cpp\
		classes/Route.cpp\
		classes/Server.cpp\
		classes/Socket.cpp

OBJS = $(SRC:.cpp=.o)
FT_OBJS = $(FT_SRC:.cpp=.o)
# CFLAGS = -Wall -Werror -Wextra -pedantic -std=c++98 
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c++98 -g -fsanitize=address 

CC = clang++

all: $(NAME) $(FT_NAME)

$(NAME):	$(OBJS)
			$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(FT_NAME):	$(FT_OBJS)
			$(CC) $(CFLAGS) -o $(FT_NAME) $(FT_OBJS)

%.o:	%.cpp
			$(CC) $(CFLAGS) -c $< -o $@

clean:
			rm -f $(OBJS) $(FT_OBJS)

fclean: 	clean
			rm -f $(NAME) $(FT_NAME)

re: fclean all 
    