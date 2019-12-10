#include "../headers/game.h"

void *server(void *data)
{
    int fd;
    struct sockaddr_in addr;
    t_game_info *gameInfo;
    char *buffer;

    if ((buffer = (char *)malloc(sizeof(*buffer) * (BUFFER_MAX + 1))) == 0)
    {
        perror("buffer:");
        return (NULL);
    }
    fd = -1;
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");
    addr.sin_port = htons(981);
    addr.sin_family = PF_INET;
    gameInfo = init_game_info();
    if (gameInfo == NULL)
    {
        perror("game info:");
        return (NULL);
    }
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket:");
        return (NULL);
    }
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind:");
        return (NULL);
    }
    if (listen(fd, 10) == -1)
    {
        perror("listen:");
        return (NULL);
    }
    while (gameInfo->count_player < 2)
    {
        int fd_client;
        t_player *player;

        player = NULL;
        fd_client = accept(fd, NULL, NULL);
        printf("accept %d\n", fd_client);
        if (gameInfo->count_player == 0)
        {
            player = init_player(1, 0);
            strcpy(player->uid, "one");
        }
        else if (gameInfo->count_player == 1)
        {
            player = init_player(2, 0);
            strcpy(player->uid, "two");
        }
        if (player == NULL)
        {
            close(fd);
            delete_game_info(gameInfo);
            return (NULL);
        }
        player->fd = fd_client;
        if (gameInfo->count_player == 0)
            gameInfo->player1 = *player;
        else if (gameInfo->count_player == 1)
            gameInfo->player2 = *player;
        gameInfo->count_player++;
    }
    gameInfo->ball = *init_ball();
    buffer = serialize_game_info(buffer, gameInfo);
    send(gameInfo->player1.fd, buffer - 800, BUFFER_MAX, 0);
    send(gameInfo->player2.fd, buffer - 800, BUFFER_MAX, 0);
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500;

    int currentTime = 0;
    int lastTime = 0;
    int step = 1;
    int vx = step;
    int vy = step;
    char *tmp;
    t_cmd command;
    while (gameInfo->isRun)
    {
        fd_set read_fs;

        currentTime = SDL_GetTicks();
        FD_ZERO(&read_fs);
        FD_SET(gameInfo->player1.fd, &read_fs);
        FD_SET(gameInfo->player2.fd, &read_fs);
        if (select(FD_SETSIZE, &read_fs, NULL, NULL, &timeout) > -1)
        {
            if (FD_ISSET(gameInfo->player1.fd, &read_fs) || FD_ISSET(gameInfo->player2.fd, &read_fs))
            {
                tmp = (char *)malloc(sizeof(*tmp) * (BUFFER_MAX));
            }
            if (FD_ISSET(gameInfo->player1.fd, &read_fs))
            {
                recv(gameInfo->player1.fd, tmp, BUFFER_MAX, 0);
                deserialize_command(tmp + 256, &command);
                movePlayer(&gameInfo->player1, &command, 2);
            }
            else if (FD_ISSET(gameInfo->player2.fd, &read_fs))
            {
                recv(gameInfo->player2.fd, tmp, BUFFER_MAX, 0);
                deserialize_command(tmp + 256, &command);
                movePlayer(&gameInfo->player2, &command, 2);
            }
            if (FD_ISSET(gameInfo->player1.fd, &read_fs) || FD_ISSET(gameInfo->player2.fd, &read_fs))
            {
                free(tmp);
            }
        }
        if ((currentTime - lastTime) > 5)
        {
            if ((gameInfo->ball.x + gameInfo->ball.w) >= SCREEN_WIDTH)
            {
                gameInfo->player1.score++;
                restoreBall(&gameInfo->ball);
            }
            if (gameInfo->ball.x <= 0)
            {
                gameInfo->player2.score++;
                restoreBall(&gameInfo->ball);
            }
            if (((gameInfo->player1.x + gameInfo->player1.w) >= gameInfo->ball.x) &&
                (gameInfo->ball.y >= gameInfo->player1.y) && (gameInfo->ball.y <= (gameInfo->player1.y + gameInfo->player1.h)))
            {
                vx = -vx;
            }
            if (((gameInfo->player2.x) <= gameInfo->ball.x) &&
                (gameInfo->ball.y >= gameInfo->player2.y) && (gameInfo->ball.y <= (gameInfo->player2.y + gameInfo->player2.h)))
            {
                vx = -vx;
            }
            if ((gameInfo->ball.y + gameInfo->ball.h) >= SCREEN_HEIGHT)
            {
                vy = -step;
            }
            if (gameInfo->ball.y <= 0)
            {
                vy = step;
            }
            moveBall(&gameInfo->ball, vx, vy);
            lastTime = currentTime;
            if (gameInfo->player1.score == 10 || gameInfo->player2.score == 10)
            {
                gameInfo->isRun = 0;
            }
            broadcast_to_client(gameInfo);
        }
    }
    free(buffer - 800);
    close(fd);
    return (data);
}
