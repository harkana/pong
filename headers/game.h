#ifndef __GAME_H__
#define __GAME_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../lib/headers/list.h"
#include "../lib/headers/node.h"

#define     SCREEN_WIDTH    640
#define     SCREEN_HEIGHT   480
#define     PLAYER_HEIGHT   100
#define     PLAYER_WIDTH    10
#define     BALL_WIDTH      10
#define     BALL_HEIGHT     10

#define     MAX_PLAYERS     2
#define     BUFFER_MAX      4096

typedef struct s_args {
    int         ac;
    char        **av;
}              t_args;

typedef struct s_player {
    int         fd;
    char        uid[256];
    int         score;
    int         x;
    int         y;
    int         w;
    int         h;
}              t_player;

typedef struct s_ball {
    int         x;
    int         y;
    int         w;
    int         h;
}              t_ball;

typedef struct s_game_info {
    t_player        player1;
    t_player        player2;
    t_ball          ball;
    int             count_player;
    int             identity;
    int             isRun;
}              t_game_info;

typedef struct s_cmd {
    char            name[256];
}              t_cmd;

typedef enum    e_dir {
    LEFT = 0,
    TOP  = 1,
    RIGHT = 2,
    BOTTOM = 3
}               t_dir;

t_ball      *init_ball();
t_player    *init_player(int, int);
t_game_info *init_game_info();

void        delete_game_info(t_game_info *);
void        delete_player(t_player *);

void        *client(void *);
void        *server(void *);

void        handle_client(t_game_info *);

t_args      *init_args(int, char **);

char        *serialize_int(char *buffer, int value);
char        *serialize_char(char *buffer, char value);
char        *serialize_char_ptr(char *buffer, char *value);

char        *deserialize_char_ptr(char *buffer, char *value);
char        *deserialize_char(char *buffer, char *value);
char        *deserialize_int(char *buffer, int *val);

char        *serialize_ball(char *, t_ball *);
char        *serialize_player(char *, t_player *);
char        *serialize_game_info(char *, t_game_info *);

char        *deserialize_ball(char *, t_ball *);
char        *deserialize_player(char *, t_player *);
char        *deserialize_game_info(char *, t_game_info *);

char        *serialize_command(char *, t_cmd *);
char        *deserialize_command(char *, t_cmd *);

void        clear_buffer(char *, int);

void        debug_display_players(t_game_info *);

int         checkPlayer(t_player *one, t_player *two);
int         checkGameInfo(t_game_info *a, t_game_info *b);

void        moveBall(t_ball *, int, int);
void        movePlayer(t_player *, t_cmd *, int);

void        restoreBall(t_ball *);

void        broadcast_to_client(t_game_info *);

#endif
