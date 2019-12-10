#include "../headers/game.h"

void *client(void *data)
{
    int fd;
    struct sockaddr_in addr;
    fd_set read_fs;
    t_game_info gameInfo;
    char    *buffer;

    buffer = (char *)malloc(sizeof(*buffer) * (BUFFER_MAX + 1));
    if (buffer == NULL)
    {
        perror("buffer:");
        return (NULL);
    }
    addr.sin_addr.s_addr = inet_addr("192.168.0.27");
    addr.sin_port = htons(981);
    addr.sin_family = PF_INET;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        perror("socket:");
        return (data);
    }
    printf("file descriptor %d\n", fd);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("connect:");
        return (data);
    }
    FD_ZERO(&read_fs);
    FD_SET(fd, &read_fs);
    if (select(FD_SETSIZE, &read_fs, NULL, NULL, NULL) > -1)
    {
        recv(fd, buffer, BUFFER_MAX, 0);
        buffer = deserialize_game_info(buffer + 800, &gameInfo);
        gameInfo.identity = fd;
        printf("identity %d\n", gameInfo.identity);
        handle_client(&gameInfo);
    }
    close(fd);
    free(buffer);
    return (data);
}
