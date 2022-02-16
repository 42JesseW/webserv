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
CFLAGS = -Wall -Werror -Wextra -std=c++98 -pedantic -g -fsanitize=address 

all: $(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS)

%.o:	%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ -I includes

clean:
	rm -f $(OBJS)

fclean: 	clean
	rm -f $(NAME)

re: fclean all
