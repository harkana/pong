NAME=pong
SRC= main.c \
	lib/list.c \
	lib/node.c \
	src/client.c \
	src/server.c \
	src/handle_client.c \
	src/serialize.c \
	src/game.c 
CC=gcc
CFLAGS=-W -Wall -Werror -Wextra -pedantic -std=c99
LINK=-lSDL2 -lSDL2main -lSDL2_image -lSDL2_ttf
OBJ=$(SRC:%.c=%.o)

all:	$(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LINK)

clean:
		rm -rf $(OBJ)

fclean:	clean
		rm -rf $(NAME)

re: 	fclean all

run\:dev:
		nodemon

.PHONY: all clean fclean re