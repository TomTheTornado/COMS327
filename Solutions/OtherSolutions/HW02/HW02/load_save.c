#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/generator.h"
#include "headers/load_save.h"

int load_dungeon(dungeon_t *d)
{
    FILE *f = NULL;

    fill_map(d);
    open_file(&f, read);
    char marker[13];
    uint32_t version_marker;
    uint32_t file_size;
    uint16_t i, j, k;

    fread(&marker, sizeof(char), 12, f);
    // printf("%s\n", marker);
    fread(&version_marker, sizeof(uint32_t), 1, f);
    // printf("%d\n", be16toh(version_marker));
    marker[12] = '\0';
    if (strcmp(marker, MARK)) //check file marker
        return -1;

    if (be32toh(version_marker) != 0)
        return -1;

    fread(&file_size, sizeof(uint32_t), 1, f);
    // printf("%d\n", be32toh(file_size));
    fread(&d->player.x, sizeof(uint8_t), 1, f);
    fread(&d->player.y, sizeof(uint8_t), 1, f);

    // printf("X: %d, Y: %d\n", d->player.x, d->player.y);
    fread(&d->hardness, sizeof(uint8_t), 1680, f);
    fread(&d->num_rooms, sizeof(uint16_t), 1, f);
    d->num_rooms = be16toh(d->num_rooms);
    // printf("%d\n", d->num_rooms);
    d->rooms = malloc(sizeof(room_t) * d->num_rooms);

    //rooms
    for (i = 0; i < d->num_rooms; i++)
    {
        fread(&d->rooms[i].x, sizeof(uint8_t), 1, f);
        fread(&d->rooms[i].y, sizeof(uint8_t), 1, f);
        fread(&d->rooms[i].width, sizeof(uint8_t), 1, f);
        fread(&d->rooms[i].height, sizeof(uint8_t), 1, f);

        // printf("%d. X: %d,Y: %d,Width: %d,Height: %d\n",i, d->rooms[i].x, d->rooms[i].y, d->rooms[i].width, d->rooms[i].height);

        for (j = d->rooms[i].y; j < d->rooms[i].height + d->rooms[i].y; j++) //add to map
        {
            for (k = d->rooms[i].x; k < d->rooms[i].width + d->rooms[i].x; k++)
            {
                d->map[j][k] = c_room;
            }
        }
    }

    //upstairs
    fread(&d->num_upstairs, sizeof(uint16_t), 1, f);
    d->num_upstairs = be16toh(d->num_upstairs);
    // printf("%d\n", d->num_upstairs);
    d->upstairs = malloc(sizeof(location_t) * d->num_upstairs);
    for (i = 0; i < d->num_upstairs; i++)
    {
        fread(&d->upstairs[i].x, sizeof(uint8_t), 1, f);
        fread(&d->upstairs[i].y, sizeof(uint8_t), 1, f);
        // printf("X:%d, Y:%d\n", d->upstairs[i].x, d->upstairs[i].y);
        //add to map
        d->map[d->upstairs[i].y][d->upstairs[i].x] = c_u_stairs;
    }

    // //downstairs
    fread(&d->num_downstairs, sizeof(uint16_t), 1, f);
    d->num_downstairs = be16toh(d->num_downstairs);
    // printf("%d\n", d->num_downstairs);
    d->downstairs = malloc(sizeof(location_t) * d->num_downstairs);
    for (i = 0; i < d->num_downstairs; i++)
    {
        fread(&d->downstairs[i].x, sizeof(uint8_t), 1, f);
        fread(&d->downstairs[i].y, sizeof(uint8_t), 1, f);
        //add to map
        d->map[d->downstairs[i].y][d->downstairs[i].x] = c_d_stairs;
    }

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            if (d->hardness[i][j] == 0 && d->map[i][j] == c_rock)
            {
                d->map[i][j] = c_corridor;
            }
        }
    }

    fclose(f);
    // printf("Finished Reading.\n");

    return 0;
}

int save_dungeon(dungeon_t *d)
{
    FILE *f = NULL;

    //init values and convert endiness
    uint32_t mark_v = 0;
    mark_v = htobe32(mark_v);
    uint32_t file_size = 1708 + d->num_rooms * 4 + d->num_upstairs * 2 + d->num_downstairs * 2;
    file_size = htobe32(file_size);
    uint16_t num_rooms = d->num_rooms;
    num_rooms = htobe16(num_rooms);
    uint16_t num_upstairs = d->num_upstairs;
    num_upstairs = htobe16(num_upstairs);
    uint16_t num_downstairs = d->num_downstairs;
    num_downstairs = htobe16(num_downstairs);


    open_file(&f, write);                                   //open file

    fwrite(&MARK, 12, 1, f);                                //file marker
    fwrite(&mark_v, sizeof(uint32_t), 1, f);                //file version
    fwrite(&file_size, sizeof(uint32_t), 1, f);             //file size
    fwrite(&d->player.x, sizeof(uint8_t), 1, f);            //player x
    fwrite(&d->player.y, sizeof(uint8_t), 1, f);            //player y
    fwrite(&d->hardness, sizeof(uint8_t), 1680, f);         //hardness array

    //rooms
    int i;
    fwrite(&num_rooms, sizeof(uint16_t), 1, f);             //number of room
    for (i = 0; i < d->num_rooms; i++)
    {
        fwrite(&d->rooms[i].x, sizeof(uint8_t), 1, f);      //room x
        fwrite(&d->rooms[i].y, sizeof(uint8_t), 1, f);      //room y
        fwrite(&d->rooms[i].width, sizeof(uint8_t), 1, f);  //room width
        fwrite(&d->rooms[i].height, sizeof(uint8_t), 1, f); //room height
    }

    //upstairs
    fwrite(&num_upstairs, sizeof(uint16_t), 1, f);          //number of up stairs
    for (i = 0; i < d->num_upstairs; i++)
    {
        fwrite(&d->upstairs[i].x, sizeof(uint8_t), 1, f);   //upstair x
        fwrite(&d->upstairs[i].y, sizeof(uint8_t), 1, f);   //upstair y
    }

    //downstairs
    fwrite(&num_downstairs, sizeof(uint16_t), 1, f);        //num of down stairs
    for (i = 0; i < d->num_downstairs; i++)
    {
        fwrite(&d->downstairs[i].x, sizeof(uint8_t), 1, f); //downstair x
        fwrite(&d->downstairs[i].y, sizeof(uint8_t), 1, f); //downstair y
    }

    fclose(f);                                              //close file
    printf("Finished Writing.\n");                  
    return 0;
}

int open_file(FILE **f, open_t open)
{
    char *path;

    char *home = getenv("HOME");

    if (!(path = malloc(strlen(home) + strlen("/.rlg327/dungeon") + 1)))
        return 1;

    strcpy(path, home);
    strcat(path, "/.rlg327/dungeon");

    switch (open)
    {
    case read:
        if (!(*f = fopen(path, "r")))
            return 1;
        break;
    case write:
        if (!(*f = fopen(path, "w")))
            return 1;
        break;
    }

    free(path);
    return 0;
}