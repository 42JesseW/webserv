NAME		= 	webserve
CC			= 	clang++
RM			=	rm -f

SRC			= 	main.cpp\
				Request.cpp\
				Response.cpp\
				Request/ParseRequest.cpp\
				Tests/RequestTests.cpp
OBJ			= 	$(SRC:%.cpp=%.o)
FLAGS		=	-std=c++11
LIBS		=	-o

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(LIBS) $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $(LIBS) $@ $< -I includes

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all