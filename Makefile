NAME = webserv

SRC =   main.cpp\
		srcs/Config.cpp \
		srcs/Option.cpp \
		srcs/Utils.cpp \
        srcs/Route.cpp \
		srcs/Server.cpp \
		srcs/Socket.cpp \
		srcs/Client.cpp \
		srcs/request/ParseRequest.cpp \
		srcs/request/ErrorHandling.cpp \
		srcs/request/Request.cpp \
		srcs/ConfigUtil.cpp

OBJS = $(SRC:.cpp=.o)
FT_OBJS = $(FT_SRC:.cpp=.o)
CFLAGS = -Wall -Wextra -pedantic -g -fsanitize=address

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
