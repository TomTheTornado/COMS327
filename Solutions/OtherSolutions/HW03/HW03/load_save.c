#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/dungeon.h"
#include "headers/generator.h"
#include "headers/load_save.h"

#ifdef __APPLE__
#include <libkern/OSByteOrder.h>
#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)
#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)
#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)
#else
#include <endian.h>
#endif

#define MARK "RLG327-S2019"

typedef enum open
{
    read,
    write
} open_t;

void open_file(FILE **f, open_t open);
void open_file_working_directory(FILE ***f, open_t open);

int load_dungeon(dungeon_t *d)
{
    FILE *f = NULL;

    fill_map(d);
    open_file(&f, read);

    char marker[13];
    uint32_t version_marker, file_size;
    uint16_t i, j, k;

    fread(&marker, sizeof(char), 12, f);            //read marker
    fread(&version_marker, sizeof(uint32_t), 1, f); //read file version

    marker[12] = '\0';
    if (strcmp(marker, MARK))
        return -1;

    if (be32toh(version_marker) != 0)
        return -1;

    fread(&file_size, sizeof(uint32_t), 1, f);     //File Size
    fread(&d->player.pos.x, sizeof(uint8_t), 1, f);    //Player x
    fread(&d->player.pos.y, sizeof(uint8_t), 1, f);    //Player y
    fread(&d->hardness, sizeof(uint8_t), 1680, f); //Hardness Map
    fread(&d->num_rooms, sizeof(uint16_t), 1, f);  //Room Count

    d->num_rooms = be16toh(d->num_rooms);
    d->rooms = malloc(sizeof(room_t) * d->num_rooms);

    //rooms
    for (i = 0; i < d->num_rooms; i++)
    {
        fread(&d->rooms[i].pos.x, sizeof(uint8_t), 1, f);      //Room x
        fread(&d->rooms[i].pos.y, sizeof(uint8_t), 1, f);      //Room y
        fread(&d->rooms[i].dim.w, sizeof(uint8_t), 1, f);  //Room Width
        fread(&d->rooms[i].dim.h, sizeof(uint8_t), 1, f); //Room Height

        for (j = d->rooms[i].pos.y; j < d->rooms[i].dim.h + d->rooms[i].pos.y; j++)
            for (k = d->rooms[i].pos.x; k < d->rooms[i].dim.w + d->rooms[i].pos.x; k++)
                d->map[j][k] = c_room;
    }

    fread(&d->num_upstairs, sizeof(uint16_t), 1, f); //Up stairs count

    d->num_upstairs = be16toh(d->num_upstairs);
    d->upstairs = malloc(sizeof(pos_t) * d->num_upstairs);

    for (i = 0; i < d->num_upstairs; i++)
    {
        fread(&d->upstairs[i].x, sizeof(uint8_t), 1, f); //stair x
        fread(&d->upstairs[i].y, sizeof(uint8_t), 1, f); //stair y
        d->map[d->upstairs[i].y][d->upstairs[i].x] = c_u_stairs;
    }

    fread(&d->num_downstairs, sizeof(uint16_t), 1, f); //Down stairs count

    d->num_downstairs = be16toh(d->num_downstairs);
    d->downstairs = malloc(sizeof(pos_t) * d->num_downstairs);

    for (i = 0; i < d->num_downstairs; i++)
    {
        fread(&d->downstairs[i].x, sizeof(uint8_t), 1, f); //stair x
        fread(&d->downstairs[i].y, sizeof(uint8_t), 1, f); //stair y
        d->map[d->downstairs[i].y][d->downstairs[i].x] = c_d_stairs;
    }

    for (i = 0; i < MAP_HEIGHT; i++)
        for (j = 0; j < MAP_WIDTH; j++)
            if (d->hardness[i][j] == 0 && d->map[i][j] == c_rock)
                d->map[i][j] = c_corridor;

    fclose(f);

    return 0;
}

int save_dungeon(dungeon_t *d)
{
    FILE *f = NULL;
    int i;

    uint32_t mark_v = 0;
    uint32_t file_size = 1708 + d->num_rooms * 4 + d->num_upstairs * 2 + d->num_downstairs * 2;
    uint16_t num_rooms = d->num_rooms;
    uint16_t num_upstairs = d->num_upstairs;
    uint16_t num_downstairs = d->num_downstairs;

    mark_v = htobe32(mark_v);
    file_size = htobe32(file_size);
    num_rooms = htobe16(num_rooms);
    num_upstairs = htobe16(num_upstairs);
    num_downstairs = htobe16(num_downstairs);

    open_file(&f, write); //open file

    fwrite(&MARK, 12, 1, f);                        //file marker
    fwrite(&mark_v, sizeof(uint32_t), 1, f);        //file version
    fwrite(&file_size, sizeof(uint32_t), 1, f);     //file size
    fwrite(&d->player.pos.x, sizeof(uint8_t), 1, f);    //player x
    fwrite(&d->player.pos.y, sizeof(uint8_t), 1, f);    //player y
    fwrite(&d->hardness, sizeof(uint8_t), 1680, f); //hardness array

    //rooms
    fwrite(&num_rooms, sizeof(uint16_t), 1, f); //number of room
    for (i = 0; i < d->num_rooms; i++)
    {
        fwrite(&d->rooms[i].pos.x, sizeof(uint8_t), 1, f);      //room x
        fwrite(&d->rooms[i].pos.y, sizeof(uint8_t), 1, f);      //room y
        fwrite(&d->rooms[i].dim.w, sizeof(uint8_t), 1, f);  //room width
        fwrite(&d->rooms[i].dim.h, sizeof(uint8_t), 1, f); //room height
    }

    //upstairs
    fwrite(&num_upstairs, sizeof(uint16_t), 1, f); //number of up stairs
    for (i = 0; i < d->num_upstairs; i++)
    {
        fwrite(&d->upstairs[i].x, sizeof(uint8_t), 1, f); //upstair x
        fwrite(&d->upstairs[i].y, sizeof(uint8_t), 1, f); //upstair y
    }

    //downstairs
    fwrite(&num_downstairs, sizeof(uint16_t), 1, f); //num of down stairs
    for (i = 0; i < d->num_downstairs; i++)
    {
        fwrite(&d->downstairs[i].x, sizeof(uint8_t), 1, f); //downstair x
        fwrite(&d->downstairs[i].y, sizeof(uint8_t), 1, f); //downstair y
    }

    fclose(f); //close file
    return 0;
}

void open_file(FILE **f, open_t open)
{
    char *path;
    char *home = NULL;

    if (!(home = getenv("HOME")))
    {
        printf("Home path could not be found. Checking working directory...\n");
        open_file_working_directory(&f, open);
        return;
    }

    if (!(path = malloc(strlen(home) + strlen("/.rlg327/dungeon") + 1)))
    {
        printf("Path malloc failed. Checking working directory...\n");
        open_file_working_directory(&f, open);
    }

    strcpy(path, home);
    strcat(path, "/.rlg327/dungeon");

    switch (open)
    {
    case read:
        if (!(*f = fopen(path, "r")))
        {
            printf("Dungeon not found in default location. Checking working directory...\n");
            open_file_working_directory(&f, open);
        }
        break;
    case write:
        if (!(*f = fopen(path, "w")))
        {
            printf("Dungeon not found in default location. Checking working directory...\n");
            open_file_working_directory(&f, open);
        }
        break;
    }

    free(path);
    return;
}

void open_file_working_directory(FILE ***f, open_t open)
{
    switch (open)
    {
    case read:
        if (!(**f = fopen("dungeon", "r")))
        {
            printf("No dungeon found in working directory. Exiting...\n");
            exit(-1);
        }
        break;
    case write:
        if (!(**f = fopen("dungeon", "w")))
        {
            printf("No dungeon found in working directory. Exiting...\n");
            exit(-1);
        }
        break;
    }
    return;
}
