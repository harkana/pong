#include "../headers/game.h"

t_player *init_player(int n, int score)
{
    t_player *player;

    player = (t_player *)malloc(sizeof(*player));
    if (player == NULL)
    {
        perror("player:");
        return (NULL);
    }
    if (n == 1)
    {
        player->x = 10;
        player->y = 10;
        player->w = PLAYER_WIDTH;
        player->h = PLAYER_HEIGHT;
    }
    else if (n == 2)
    {
        player->x = SCREEN_WIDTH - 10 - PLAYER_WIDTH;
        player->y = 10;
        player->w = PLAYER_WIDTH;
        player->h = PLAYER_HEIGHT;
    }
    player->score = score;
    return (player);
}

t_ball *init_ball()
{
    t_ball *ball;

    ball = (t_ball *)malloc(sizeof(*ball));
    if (ball == NULL)
    {
        perror("ball:");
        return (NULL);
    }
    ball->x = SCREEN_WIDTH / 2;
    ball->y = SCREEN_HEIGHT / 2;
    ball->w = 10;
    ball->h = 10;
    return (ball);
}

void delete_player(t_player *player)
{
    free(player->uid);
    free(player);
}

t_game_info *init_game_info()
{
    t_game_info *gameInfo;

    gameInfo = (t_game_info *)malloc(sizeof(*gameInfo));
    if (gameInfo == NULL)
    {
        perror("game info:");
        return (NULL);
    }
    gameInfo->count_player = 0;
    gameInfo->ball.x = 0;
    gameInfo->ball.y = 0;
    gameInfo->ball.w = 0;
    gameInfo->ball.h = 0;
    return (gameInfo);
}

void del_node_player(t_node *node)
{
    delete_player((t_player *)node->data);
    free(node);
}

void delete_game_info(t_game_info *gameInfo)
{
    free(gameInfo);
}

t_args *init_args(int ac, char **av)
{
    t_args *args;

    args = NULL;
    if ((args = (t_args *)malloc(sizeof(*args))) == NULL)
    {
        perror("t_args:");
        return (NULL);
    }
    args->ac = ac;
    args->av = av;
    return (args);
}

void clear_buffer(char *buffer, int size)
{
    int i;

    i = 0;
    while (i < size)
    {
        buffer[i] = 0;
        i++;
    }
}

void debug_display_players(t_game_info *gameInfo)
{
    printf("player1(x, y, w, h) = (%d, %d, %d, %d)", gameInfo->player1.x, gameInfo->player1.y, gameInfo->player1.w, gameInfo->player1.h);
    printf("player2(x, y, w, h) = (%d, %d, %d, %d)", gameInfo->player2.x, gameInfo->player2.y, gameInfo->player2.w, gameInfo->player2.h);
}

int checkPlayer(t_player *one, t_player *two)
{
    int flag;

    flag = 0;
    flag = one->x == two->x;
    if (flag == 0)
        return (0);
    flag = one->y == two->y;
    if (flag == 0)
        return (0);
    flag = one->h == two->h;
    if (flag == 0)
        return (0);
    flag = one->w == two->w;
    if (!flag)
        return (0);
    flag = one->fd == two->fd;
    if (!flag)
        return (0);
    flag = one->score == two->score;
    if (!flag)
        return (0);
    flag = strcmp(one->uid, two->uid) == 0;
    return (flag);
}

int checkBall(t_ball *a, t_ball *b)
{
    int flag;

    flag = 0;
    flag = a->x == b->x;
    if (!flag)
        return (0);
    flag = a->y == b->y;
    if (!flag)
        return (0);
    flag = a->w == b->w;
    if (!flag)
        return (0);
    flag = a->h == b->h;
    return (flag);
}

int checkGameInfo(t_game_info *a, t_game_info *b)
{
    int flag;

    flag = 0;
    flag = checkPlayer(&a->player1, &b->player1);
    if (!flag)
        return (0);
    flag = checkPlayer(&a->player2, &b->player2);
    if (!flag)
        return (0);
    flag = a->count_player == b->count_player;
    if (!flag)
        return (0);
    flag = checkBall(&a->ball, &b->ball);
    return (flag);
}

void moveBall(t_ball *ball, int dx, int dy)
{
    ball->y += dy;
    ball->x += dx;
}

void broadcast_to_client(t_game_info *gameInfo)
{
    char *buf;

    buf = (char *)malloc(sizeof(char) * (BUFFER_MAX + 1));
    if (buf == NULL)
    {
        perror("buf:");
        return;
    }
    buf = serialize_game_info(buf, gameInfo);
    send(gameInfo->player1.fd, buf - 784, BUFFER_MAX, 0);
    send(gameInfo->player2.fd, buf - 784, BUFFER_MAX, 0);
    free(buf - 784);
}

void movePlayer(t_player *player, t_cmd *cmd, int step)
{
    if (strcmp(cmd->name, "up") == 0)
    {
        player->y -= step;
    }
    else if (strcmp(cmd->name, "down") == 0)
    {
        player->y += step;
    }
}

void    restoreBall(t_ball *ball)
{
    ball->x = SCREEN_WIDTH / 2;
    ball->y = SCREEN_HEIGHT / 2;
    ball->w = 10;
    ball->h = 10;
}
