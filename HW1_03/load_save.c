#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dungeon.h"
#include "load_save.h"

void fill_map(dungeon_t *dungeon)
{
    uint8_t i, j;

    dungeon->numRooms = 0;

    for (i = 0; i < max_dimY; i++) //height
    {
        for (j = 0; j < max_dimX; j++) //width
        {
            if (i == 0 || i == max_dimY - 1) //y borders
            {
                dungeon->map[i][j] = '-';
                dungeon->hardness[i][j] = 255;
            }
            else if (j == 0 || j == max_dimX - 1) //x borders
            {
                dungeon->map[i][j] = '|';
                dungeon->hardness[i][j] = 255;
            }
            else
            {
                dungeon->map[i][j] = ' ';
                dungeon->hardness[i][j] = (rand() % 254) + 1;
            }
        }
    }
}

int load_dungeon(dungeon_t *dungeon)
{
    FILE *f = NULL;

    fill_map(dungeon);
    open_file(&f, read);
    char marker[13];
    uint32_t version_marker;
    uint32_t file_size;
    uint16_t i, j, k;

    fread(&marker, sizeof(char), 12, f);
    //printf("%s\n", marker);
    fread(&version_marker, sizeof(uint32_t), 1, f);
    //printf("%d\n", be16toh(version_marker));
    marker[12] = '\0';
    if (strcmp(marker, MARK)) //check file marker
        return -1;

    if (be32toh(version_marker) != 0)
        return -1;

    fread(&file_size, sizeof(uint32_t), 1, f);
    //printf("%d\n", be32toh(file_size));
    fread(&dungeon->pc.x, sizeof(uint8_t), 1, f);
    fread(&dungeon->pc.y, sizeof(uint8_t), 1, f);
    //printf("Player: %d, %d\n", dungeon->pc.y, dungeon->pc.x);

   // printf("X: %d, Y: %d\n", dungeon->pc.x, dungeon->pc.y);
    fread(&dungeon->hardness, sizeof(uint8_t), 1680, f);
    fread(&dungeon->numRooms, sizeof(uint16_t), 1, f);
    dungeon->numRooms = be16toh(dungeon->numRooms);
    //printf("%d\n", dungeon->numRooms);
    dungeon->rooms = malloc(sizeof(room_t) * dungeon->numRooms);

    //rooms
    for (i = 0; i < dungeon->numRooms; i++)
    {
        fread(&dungeon->rooms[i].x, sizeof(uint8_t), 1, f);
        fread(&dungeon->rooms[i].y, sizeof(uint8_t), 1, f);
        fread(&dungeon->rooms[i].length, sizeof(uint8_t), 1, f);
        fread(&dungeon->rooms[i].height, sizeof(uint8_t), 1, f);

        //printf("%d. X: %d,Y: %d,Width: %d,Height: %d\n",i, dungeon->rooms[i].x, dungeon->rooms[i].y, dungeon->rooms[i].length, dungeon->rooms[i].height);

        for (j = dungeon->rooms[i].y; j < dungeon->rooms[i].height + dungeon->rooms[i].y; j++) //add to map
        {
            for (k = dungeon->rooms[i].x; k < dungeon->rooms[i].length + dungeon->rooms[i].x; k++)
            {
                dungeon->map[j][k] = '.';
            }
        }
    }
    //printf("# of rooms: %d\n", dungeon->numRooms);


    
    //Stairs
    uint16_t num_stairs;
    uint8_t x, y;
    //upstairs
    fread(&num_stairs, 2, 1, f);
    num_stairs = be16toh(num_stairs);
    for (; num_stairs; num_stairs--) {
        fread(&x, 1, 1, f);
        fread(&y, 1, 1, f);
        dungeon->map[y][x] = '<';
    }
    //downstairs
    fread(&num_stairs, 2, 1, f);
    num_stairs = be16toh(num_stairs);
    for (; num_stairs; num_stairs--) {
        fread(&x, 1, 1, f);
        fread(&y, 1, 1, f);
        dungeon->map[y][x] = '>';
    }


    //Corridors
    for (i = 0; i < max_dimY; i++)
    {
        for (j = 0; j < max_dimX; j++)
        {
            if (dungeon->hardness[i][j] == 0 && dungeon->map[i][j] == ' ')
            {
                dungeon->map[i][j] = '#';
            }
        }
    }

    fclose(f);
    printf("Finished Reading.\n");

    return 0;
}

int save_dungeon(dungeon_t *dungeon)
{
    FILE *f = NULL;

    //init values and convert endiness
    uint32_t mark_v = 0;
    mark_v = htobe32(mark_v);
    uint32_t file_size = 1708 + dungeon->numRooms * 4 + dungeon->numUpStrs * 2 + dungeon->numDownStrs * 2;
    file_size = htobe32(file_size);
    uint16_t numRooms = dungeon->numRooms;
    numRooms = htobe16(numRooms);
    uint16_t numUpStrs = dungeon->numUpStrs;
    numUpStrs = htobe16(numUpStrs);
    uint16_t numDownStrs = dungeon->numDownStrs;
    numDownStrs = htobe16(numDownStrs);
    //printf("Up:%d, Down:%d\n", numUpStrs, numDownStrs);


    open_file(&f, write);                                   //open file

    fwrite(&MARK, 12, 1, f);                                //file marker
    fwrite(&mark_v, sizeof(uint32_t), 1, f);                //file version
    fwrite(&file_size, sizeof(uint32_t), 1, f);             //file size
    fwrite(&dungeon->pc.x, sizeof(uint8_t), 1, f);            //player x
    fwrite(&dungeon->pc.y, sizeof(uint8_t), 1, f);            //player y
    fwrite(&dungeon->hardness, sizeof(uint8_t), 1680, f);         //hardness array

    //rooms
    int i;
    fwrite(&numRooms, sizeof(uint16_t), 1, f);             //number of room
    for (i = 0; i < dungeon->numRooms; i++)
    {
        fwrite(&dungeon->rooms[i].x, sizeof(uint8_t), 1, f);      //room x
        fwrite(&dungeon->rooms[i].y, sizeof(uint8_t), 1, f);      //room y
        fwrite(&dungeon->rooms[i].length, sizeof(uint8_t), 1, f);  //room width
        fwrite(&dungeon->rooms[i].height, sizeof(uint8_t), 1, f); //room height
    }

    //upstairs
    fwrite(&numUpStrs, sizeof(uint16_t), 1, f);          //number of up stairs
    //printf("Save Up: %d\n", dungeon->numUpStrs);
    for (i = 0; i < dungeon->numUpStrs; i++)
    {
        fwrite(&dungeon->upstrs[i].x, sizeof(uint8_t), 1, f);   //upstair x
        fwrite(&dungeon->upstrs[i].y, sizeof(uint8_t), 1, f);   //upstair y
        //printf("%d. %d, %d\n", i, dungeon->upstrs[i].y, dungeon->upstrs[i].x);
    }

    //downstairs
    fwrite(&numDownStrs, sizeof(uint16_t), 1, f);        //num of down stairs
    //printf("Save Down: %d\n", dungeon->numDownStrs);
    for (i = 0; i < dungeon->numDownStrs; i++)
    {
        fwrite(&dungeon->downstrs[i].x, sizeof(uint8_t), 1, f); //downstair x
        fwrite(&dungeon->downstrs[i].y, sizeof(uint8_t), 1, f); //downstair y
        //printf("%d. %d, %d\n", i, dungeon->downstrs[i].y, dungeon->downstrs[i].x);
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