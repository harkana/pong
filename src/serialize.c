#include "../headers/game.h"

char    *serialize_int(char *buffer, int value)
{
    int div;
    int i;

    i = 0;
    div = value / 128;
    while (i < div)
    {
        buffer[i] = 127;
        i++;
    }
    buffer[i] = value % 128;
    i++;
    while (i < 16)
    {
        buffer[i] = 0;
        i++;
    }
    return (buffer + 16);
}

char    *serialize_char(char *buffer, char value)
{
    buffer[0] = value;
    return buffer + 1;
}

char    *serialize_char_ptr(char *buffer, char *value)
{
    int i;

    i = 0;
    while (value[i])
    {
        buffer[i] = value[i];
        i++;
    }
    while (i < 256)
    {
        buffer[i] = 0;
        i++;
    }
    return (buffer + 256);
}

char    *deserialize_char_ptr(char *buffer, char *value)
{
    int i;

    i = 0;
    buffer -= 256;
    while (buffer[i])
    {
        value[i] = buffer[i];
        i++;
    }
    value[i] = 0;
    return (buffer);
}

char    *deserialize_char(char *buffer, char *value)
{
    buffer -= 1;
    *value = buffer[0];
    return (buffer);
}

char    *deserialize_int(char *buffer, int *val)
{
    int i;

    i = 0;
    *val = 0;
    buffer -= 16;
    while (i < 16)
    {
        if (buffer[i] == 127)
            *val += 128;
        else
            *val += buffer[i];
        i++;
    }
    return (buffer);
}

char    *serialize_ball(char *buffer, t_ball *rect)
{
    buffer = serialize_int(buffer, rect->x);
    buffer = serialize_int(buffer, rect->y);
    buffer = serialize_int(buffer, rect->w);
    buffer = serialize_int(buffer, rect->h);
    return (buffer);
}

char    *deserialize_ball(char *buffer, t_ball *rect)
{
    buffer = deserialize_int(buffer, &rect->h);
    buffer = deserialize_int(buffer, &rect->w);
    buffer = deserialize_int(buffer, &rect->y);
    buffer = deserialize_int(buffer, &rect->x);
    return (buffer);
}

char    *serialize_player(char *buffer, t_player *player)
{
    buffer = serialize_int(buffer, player->x);
    buffer = serialize_int(buffer, player->y);
    buffer = serialize_int(buffer, player->w);
    buffer = serialize_int(buffer, player->h);
    buffer = serialize_int(buffer, player->fd);
    buffer = serialize_int(buffer, player->score);
    buffer = serialize_char_ptr(buffer, player->uid);
    return (buffer);
}

char    *deserialize_player(char *buffer, t_player *player)
{
    buffer = deserialize_char_ptr(buffer, player->uid);
    buffer = deserialize_int(buffer, &player->score);
    buffer = deserialize_int(buffer, &player->fd);
    buffer = deserialize_int(buffer, &player->h);
    buffer = deserialize_int(buffer, &player->w);
    buffer = deserialize_int(buffer, &player->y);
    buffer = deserialize_int(buffer, &player->x);  
    return (buffer);
}

char    *serialize_game_info(char *buffer, t_game_info *gameInfo)
{
    buffer = serialize_player(buffer, &gameInfo->player1);
    buffer = serialize_player(buffer, &gameInfo->player2);
    buffer = serialize_ball(buffer, &gameInfo->ball);
    buffer = serialize_int(buffer, gameInfo->count_player);
    return (buffer);
}

char    *deserialize_game_info(char *buffer, t_game_info *gameInfo)
{
    buffer = deserialize_int(buffer, &gameInfo->count_player);
    buffer = deserialize_ball(buffer, &gameInfo->ball);
    buffer = deserialize_player(buffer, &gameInfo->player2);
    buffer = deserialize_player(buffer, &gameInfo->player1);
    return (buffer);
}

char    *serialize_command(char *buffer, t_cmd *command)
{
    buffer = serialize_char_ptr(buffer, command->name);
    return (buffer);
}

char    *deserialize_command(char *buffer, t_cmd *command)
{
    buffer = deserialize_char_ptr(buffer, command->name);
    return (buffer);
}
