NAME = server

SRC =	./Networking/Sockets/SimpleSocket.cpp\
		./Networking/Sockets/BindingSocket.cpp\
		./Networking/Sockets/ListeningSocket.cpp\
		./Networking/Sockets/ConnectingSocket.cpp\
		./Networking/Servers/SimpleServer.cpp\
		./Networking/Servers/TestServer.cpp\
		./Networking/Servers/test.cpp

OBJS = $(SRC:.cpp=.o)
CFLAGS = -pedantic 

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
    