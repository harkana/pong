#include "../headers/game.h"

void renderPlayer(SDL_Renderer *renderer, t_player *player)
{
    SDL_Rect rect;

    rect.x = player->x;
    rect.y = player->y;
    rect.w = player->w;
    rect.h = player->h;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void renderBall(SDL_Renderer *renderer, t_ball *ball)
{
    SDL_Rect rect;

    rect.x = ball->x;
    rect.y = ball->y;
    rect.w = ball->w;
    rect.h = ball->h;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void getData(t_game_info *gameInfo)
{
    char *buf;

    buf = (char *)malloc(sizeof(char) * (BUFFER_MAX + 1));
    if (buf == NULL)
    {
        perror("get data:");
        return;
    }
    recv(gameInfo->identity, buf, BUFFER_MAX, 0);
    deserialize_game_info(buf + 784, gameInfo);
    free(buf);
}

void pollData(t_game_info *gameInfo)
{
    fd_set read_fs;
    struct timeval timeout;

    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    FD_ZERO(&read_fs);
    FD_SET(gameInfo->identity, &read_fs);
    if (select(FD_SETSIZE, &read_fs, NULL, NULL, &timeout) > -1)
    {
        if (FD_ISSET(gameInfo->identity, &read_fs))
        {
            getData(gameInfo);
        }
    }
}

void handle_client(t_game_info *gameInfo)
{
    SDL_Event event;
    int isRun;
    SDL_Window *window;
    SDL_Renderer *renderer;
    char *tmp;
    t_cmd cmd;

    isRun = 1;
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) == -1)
    {
        return;
    }
    window = SDL_CreateWindow("pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        return;
    }
    while (isRun)
    {
        SDL_PollEvent(&event);
        pollData(gameInfo);
        if (event.type == SDL_QUIT)
        {
            isRun = 0;
            send(gameInfo->identity, "quit", strlen("quit") + 1, 0);
        }
        if (event.type == SDL_KEYDOWN)
        {
            tmp = (char *)malloc(sizeof(*tmp) * (BUFFER_MAX));
            if (tmp == NULL)
            {
                perror("buffer malloc:");
                return;
            }
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_DOWN)
        {
            strcpy(cmd.name, "down");
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_UP)
        {
            strcpy(cmd.name, "up");
        }
        if (event.type == SDL_KEYDOWN)
        {
            serialize_command(tmp, &cmd);
            send(gameInfo->identity, tmp, BUFFER_MAX, 0);
            free(tmp);
        }
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(renderer, NULL);
        renderPlayer(renderer, &gameInfo->player1);
        renderPlayer(renderer, &gameInfo->player2);
        renderBall(renderer, &gameInfo->ball);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
