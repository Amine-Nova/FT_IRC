SERVER = Server

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS_S = Server.cpp Client.cpp

OBJ_S = ${SRCS_S:.cpp=.o}

all : $(SERVER)

%.o : %.cpp Server.hpp Client.hpp
	$(CC) $(CFLAGS) -c $< -o $@


$(SERVER) : $(OBJ_S)
	$(CC) $(CFLAGS) $^ -o $@

clean :
	rm -rf $(OBJ_S)

fclean : clean
	rm -rf $(SERVER)

re : fclean all

.PHONY : clean
