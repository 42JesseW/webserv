NAME        = webserv

SOURCE_DIR  = srcs
HEADER_DIR  = includes
TEST_DIR    = tests
CMAKE_DIR   = cmake-build

CFLAGS      = -Wall -Werror -Wextra -pedantic -g -fsanitize=address -std=c++98
CLINKS      = -lpthread -lcurl

SRC         = $(SOURCE_DIR)/ConfigUtil.cpp \
              $(SOURCE_DIR)/Socket.cpp \
              $(SOURCE_DIR)/ClientSocket.cpp \
              $(SOURCE_DIR)/ServerSocket.cpp \
              $(SOURCE_DIR)/FileParser.cpp \
              $(SOURCE_DIR)/Option.cpp \
              $(SOURCE_DIR)/PortConfig.cpp \
              $(SOURCE_DIR)/ServerConfig.cpp \
              $(SOURCE_DIR)/Route.cpp \
              $(SOURCE_DIR)/Connection.cpp \
              $(SOURCE_DIR)/Signals.cpp \
              $(SOURCE_DIR)/CGI.cpp \
              $(SOURCE_DIR)/Utils.cpp \
              $(SOURCE_DIR)/request/HandleRequest.cpp \
              $(SOURCE_DIR)/request/ErrorHandling.cpp \
              $(SOURCE_DIR)/request/Request.cpp \
              $(SOURCE_DIR)/request/Decoder.cpp \
              $(SOURCE_DIR)/response/Response.cpp \
			  $(SOURCE_DIR)/main.cpp

OBJECTS     = $(SRC:.cpp=.o)
HEADERS     = $(addprefix -I, $(HEADER_DIR))

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(CLINKS) $(CFLAGS) -o $(NAME) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@ $(HEADERS)

clean:
	@rm -f $(OBJECTS)

fclean: 	clean
	@rm -f $(NAME)

re: fclean all

# TODO add this to the github actions flow
test:
	@mkdir -p $(CMAKE_DIR)
	@cd $(CMAKE_DIR) && cmake ../ && cmake --build .
	@cd $(CMAKE_DIR)/tests && ./tests
