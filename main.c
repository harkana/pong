#include "./headers/game.h"

int main(int ac, char **av)
{
    pthread_t   tid;

    if (ac == 2 && strcmp(av[1], "host") == 0)
    {
        pthread_create(&tid, NULL, server, NULL);
    }
    client(NULL);
    return (EXIT_SUCCESS);
}
