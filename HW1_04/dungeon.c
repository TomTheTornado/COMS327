#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <endian.h>
#include <limits.h>
#include <unistd.h>
#include <curses.h>

#include "headers/dungeon.h"
#include "headers/load_save.h"
#include "headers/heap.h"
#include "headers/movement.h"

#define mappair(pair) (d->map[pair[dim_y]][pair[dim_x]])
#define mapxy(x, y) (d->map[y][x])
#define hardnesspair(pair) (d->hardness[pair[dim_y]][pair[dim_x]])
#define hardnessxy(x, y) (d->hardness[y][x])

typedef struct corridor_path
{
    heap_node_t *hn;
    uint8_t pos[2];
    uint8_t from[2];
    int32_t cost;
} corridor_path_t;

typedef enum dim
{
    dim_x,
    dim_y,
    num_dims
} dim_t;

typedef int8_t pair_t[num_dims];

typedef enum __attribute__((__packed__)) terrain_type
{
    ter_debug,
    ter_wall,
    ter_wall_immutable,
    ter_floor,
    ter_floor_room,
    ter_floor_hall,
    ter_stairs,
    ter_stairs_up,
    ter_stairs_down
} terrain_type_t;

void generate_monsters(dungeon_t *d)
{
    uint8_t x, y, i, speed;
    uint8_t intelligence, telepathy, tunneling, erratic;
    for(y = 0; y < max_dimY; y++){
        for(x = 0; x < max_dimX; x++){
            d->monsterMap[y][x] = 99;
        }
    }
    d->monsters = malloc(sizeof(monster_t) * d->numMonsters);

    printf("Num monsters: %d\n", d->numMonsters);
    for (i = 0; i < d->numMonsters; i++)
    {
        d->monsters[i].intelligence = false;
        d->monsters[i].telepathy = false;
        d->monsters[i].tunneling = false;
        d->monsters[i].erratic = false;

        x = rand() % (max_dimX - 1) + 1;
        y = rand() % (max_dimY - 1) + 1;
        speed = (rand() % (16)) + 5;
       

        if (d->map[y][x] == '.' && d->monsterMap[y][x] == 99 && (x != d->pc.x && y != d->pc.y)){
            d->monsters[i].location.x = x;
            d->monsters[i].location.y = y;
            d->monsters[i].speed = speed;

            //printf("Monster %d Speed: %d\n", i, d->monsters[i].speed);

            intelligence = rand() % 2;
            telepathy = rand() % 2;
            tunneling = rand() % 2;
            erratic = rand() % 2;

            if(intelligence == 1){
                d->monsters[i].intelligence = true;
                d->monsters[i].type += 1;
            }
            if(telepathy == 1){
                d->monsters[i].telepathy = true;
                d->monsters[i].type += 2;
            }
            if(tunneling == 1){
                d->monsters[i].tunneling = true;
                d->monsters[i].type += 4;
            }

            if(erratic == 1){
                d->monsters[i].erratic = true;
                d->monsters[i].type += 8;
            }

            d->monsterMap[y][x] = d->monsters[i].type;
        }
        else{
            i--;
        }
    }
    return;
}


/**
 * Main function for dungeon.c.
 */
int main(int argc, char const *argv[])
{
    //Creates a random unique seed for the dungeon.
    time_t t;
    t = time(NULL);
    int seed = t;
    

    //Creates the dungeon struct.
    dungeon_t dungeon;
    dungeon.numMonsters = DEFAULT_MONSTERS; // Default number
    uint8_t numMonst = DEFAULT_MONSTERS;

    action_t action;

    int i;
    for (i = 1; i < argc; i++) //check for flags
    {
        if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "-s"))
            action = (action != load) ? save : load_save;
        else if (!strcmp(argv[i], "--load") || !strcmp(argv[i], "-l"))
            action = (action != save) ? load : load_save;
        else if (!strcmp(argv[i], "--seed") && (i + 1 < argc))
            seed = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-sl") || !strcmp(argv[i], "-ls"))
        {
            action = load_save;
        }
        else if (!strcmp(argv[i], "--nummon") || !strcmp(argv[i], "-n")){
            action = monsters;
            dungeon.numMonsters = atoi(argv[i + 1]);
        }
        else if (!strcmp(argv[i], "-lm") || !strcmp(argv[i], "-ml"))
        {
            action = load_monsters;
            numMonst = atoi(argv[i + 1]);
        }


    }
    
    //Starts the seed
    srand(seed);
    printf("Seed: %d\n", seed);

    switch (action)
    {
    case load:
        printf("loading...\n");
        load_dungeon(&dungeon);
        break;
    case save:
        printf("saving...\n");
        generate_dungeon(&dungeon);
        save_dungeon(&dungeon);
        break;
    case load_save:
        printf("loading and saving...\n");
        load_dungeon(&dungeon);
        save_dungeon(&dungeon);
        break;
    case monsters:
        printf("summoning %d monsters...\n", dungeon.numMonsters);
        generate_dungeon(&dungeon);
        break;
    case load_monsters:
        printf("loading and summoning %d monsters...\n", dungeon.numMonsters);
        load_dungeon(&dungeon);
        dungeon.numMonsters = numMonst;
        break;
    default:
        generate_dungeon(&dungeon);
        break;
    }

    //printf("Seed: %d\n", seed);
    generate_monsters(&dungeon);
    print_dungeon(&dungeon, 0);
    create_paths(&dungeon);


    bool alive;
    alive = true;
    while(alive)
    {
        usleep(250000);
        //printf ("%ld\n", time(NULL));
        erraticPC(&dungeon);
        monsterMovement(&dungeon);
        create_paths(&dungeon);
        print_dungeon(&dungeon, 0);     
    }

    free_dungeon(&dungeon);
    return 0;
}
void generate_dungeon(dungeon_t *dungeon)
{
    assign_border(dungeon);
    create_rooms(dungeon);
    create_corridors(dungeon);
    assign_hardness(dungeon);
    create_stairs(dungeon);
    place_player(dungeon);
}
void print_dungeon(dungeon_t *dungeon, uint8_t type)
{
    int y, x;
    for (y = 0; y < max_dimY; y++)
    {
        for (x = 0; x < max_dimX; x++)
        {
            if(dungeon->monsterMap[y][x] != 99){
                printf("%x", dungeon->monsterMap[y][x]);
            }
            else if (x == dungeon->pc.x && y == dungeon->pc.y && (type == 4 || type == 3))
            { //Special case when have to print 2 wide
                printf(" %c", '@');
            }
            else if (x == dungeon->pc.x && y == dungeon->pc.y)
            { //Prints pc
                printf("%c", '@');
            }
            else if (type == 1 && dungeon->hardness[y][x] == 0)
            { //Prints non tunnel
                if(dungeon->nonTunnelingMap[y][x] == 65535){
                    printf("X");
                }
                else{
                    printf("%d", dungeon->nonTunnelingMap[y][x] % 10);
                }
            }
            else if (type == 2)
            { //Prints tunnel
                printf("%d", dungeon->tunnelingMap[y][x] % 10);
            }
            else if (type == 3)
            { //Prints hardness
                printf("%2x", dungeon->hardness[y][x]);
            }
            else
            { //Regular
                printf("%c", dungeon->map[y][x]);
            }
        }
        printf("\n");
    }
    printf("\n");
}
void assign_border(dungeon_t *dungeon)
{
    int y, x;

    for (int y = 0; y < max_dimY; y++)
    {
        for (int x = 0; x < max_dimX; x++)
        {
            if (y == 0 || y == max_dimY - 1)
            {
                dungeon->map[y][x] = '-';
            }
            else if (x == 0 || x == max_dimX - 1)
            {
                dungeon->map[y][x] = '|';
            }
            else
            {
                dungeon->map[y][x] = ' ';
            }
        }
    }
}
bool origin(dungeon_t *dungeon, int x, int y)
{
    //Checks if x-y position is already occupied.
    if (dungeon->map[y][x] == '|' || dungeon->map[y][x] == '-' || dungeon->map[y][x] == '.')
    {
        return false;
    }
    //Checks for a clear one space margin so the origin doesn't overlap in another room.
    else if (dungeon->map[y + 1][x] == '.' || dungeon->map[y - 1][x] == '.' || dungeon->map[y][x + 1] == '.' || dungeon->map[y][x - 1] == '.')
    {
        return false;
    }
    else
    {
        return true;
    }
    return true;
}
bool proximity(dungeon_t *dungeon, int startX, int startY, int length, int height)
{
    if ((startX + length) >= max_dimX || (startY + height) >= max_dimY)
    {
        return false;
    }

    int x, y;
    for (y = startY; y < (startY + height); y++)
    {
        for (x = startX; x < (startX + length); x++)
        {
            if (dungeon->map[y][x] == '.' || dungeon->map[y + 1][x] == '.' || dungeon->map[y][x + 1] == '.' || dungeon->map[y - 1][x] == '.' || dungeon->map[y][x - 1] == '.')
            {
                return false;
            }
        }
    }
    return true;
}
void assign_rooms(dungeon_t *dungeon, int startX, int startY, int length, int height)
{
    int y, x;
    for (y = startY; y < startY + height; y++)
    {
        for (x = startX; x < startX + length; x++)
        {
            dungeon->map[y][x] = '.';
        }
    }
}
int numOfRooms()
{

    bool decay = true;
    double chance = 50;
    int rooms = min_rms;
    while (decay == true)
    {
        //Random out of 100.
        double probability = (rand() % 101);
        if (probability <= chance)
        {
            //Keeps decreasing the chance for additional rooms.
            chance = chance / 2.0;
            rooms += 1;
            continue;
        }
        if (rooms == max_rms)
        {
            decay = false;
        }
        else
        {
            decay = false;
        }
    }
    return rooms;
}
void create_rooms(dungeon_t *dungeon)
{

    //Creates the maximum number of rooms
    dungeon->numRooms = numOfRooms();

    //Gets 6-10 rooms for the dungeon

    int totalRooms;
    totalRooms = dungeon->numRooms;

    //Initializes random origin points.
    int randX, randY;
    //Initializes random length and height.
    int rLength, rHeight;
    int roomDetails[dungeon->numRooms][4];

    int attempts, r;

    for (r = 0; r < totalRooms; r++)
    {
        attempts++;

        randX = (rand() % (max_dimX + 1));
        randY = (rand() % (max_dimY + 1));

        rLength = (rand() % (max_rmX - min_rmX + 1) + min_rmX);
        rHeight = (rand() % (max_rmY - min_rmY + 1) + min_rmY);
        //Checks to see if the origin point is not open.
        if (!origin(dungeon, randX, randY))
        {
            //Breaks out if it is unable to place rooms after 1000 attempts and 6 rooms are placed.
            if (attempts > 1000 && (r >= 6))
            {
                break;
            }
            else
            {
                r -= 1;
                continue;
            }
        }
        else
        {
            //Checks to see if length and height of room will fit in space.
            if (!proximity(dungeon, randX, randY, rLength, rHeight))
            {

                rLength = (rand() % (8 - min_rmX + 1) + min_rmX);
                rHeight = (rand() % (6 - min_rmY + 1) + min_rmY);
                //Checks to see if a smaller room will fit in the space.
                if (!proximity(dungeon, randX, randY, rLength, rHeight))
                {
                    r -= 1;
                    continue;
                }
                //If it fits, assigns locations to '.' and saves origin location.
                else
                {
                    assign_rooms(dungeon, randX, randY, rLength, rHeight);
                    roomDetails[r][1] = randY;
                    roomDetails[r][2] = randX;
                    roomDetails[r][3] = rHeight;
                    roomDetails[r][4] = rLength;
                }
            }
            //If it fits, assigns locations to '.' and saves origin location.
            else
            {
                assign_rooms(dungeon, randX, randY, rLength, rHeight);
                roomDetails[r][1] = randY;
                roomDetails[r][2] = randX;
                roomDetails[r][3] = rHeight;
                roomDetails[r][4] = rLength;
            }
        }
    }

    dungeon->rooms = malloc(sizeof(room_t) * dungeon->numRooms);
    int i;
    for (i = 0; i < r; i++)
    {
        dungeon->rooms[i].y = roomDetails[i][1];
        dungeon->rooms[i].x = roomDetails[i][2];
        dungeon->rooms[i].height = roomDetails[i][3];
        dungeon->rooms[i].length = roomDetails[i][4];
    }
}
void assign_corridor(dungeon_t *dungeon, uint8_t x, uint8_t y)
{

    if (dungeon->map[y][x] == ' ')
    {
        dungeon->map[y][x] = '#';
    }
    return;
}
void create_corridors(dungeon_t *dungeon)
{

    int i;
    for (i = 0; i < dungeon->numRooms - 1; i++)
    {
        uint8_t startX, startY, endX, endY;

        //start location
        startX = dungeon->rooms[i].x + (rand() % dungeon->rooms[i].length);
        startY = dungeon->rooms[i].y + (rand() % dungeon->rooms[i].height);

        //end location
        endX = dungeon->rooms[i + 1].x + (rand() % dungeon->rooms[i + 1].length);
        endY = dungeon->rooms[i + 1].y + (rand() % dungeon->rooms[i + 1].height);

        while ((startX != endX) || (startY != endY))
        {
            int8_t moveX = (startX < endX) ? 1 : -1;
            int8_t moveY = (startY < endY) ? 1 : -1;

            uint8_t pos_x = startX + moveX;
            uint8_t pos_y = startY + moveY;

            float distance_x = sqrt(pow((endX - pos_x), 2) + pow(endY - startY, 2));
            float distance_y = sqrt(pow((endX - startX), 2) + pow(endY - pos_y, 2));
            float previous_d = sqrt(pow((endX - startX), 2) + pow(endY - startY, 2));

            if (distance_x < previous_d && distance_y < previous_d)
            {
                if (dungeon->hardness[startY][pos_x] < dungeon->hardness[pos_y][startY])
                {
                    startX += moveX;
                }
                else
                {
                    startY += moveY;
                }
            }

            else
            {
                if (distance_x < distance_y)
                {
                    startX += moveX;
                }
                else
                {
                    startY += moveY;
                }
            }

            assign_corridor(dungeon, startX, startY);
        }
    }
    return;
}
void create_stairs(dungeon_t *dungeon)
{

    dungeon->numUpStrs = ((rand() % max_upstrs) + 1);
    dungeon->numDownStrs = ((rand() % max_downstrs) + 1);
    dungeon->upstrs = malloc(sizeof(location_t) * dungeon->numUpStrs);
    dungeon->downstrs = malloc(sizeof(location_t) * dungeon->numDownStrs);

    int up = dungeon->numUpStrs;
    int down = dungeon->numDownStrs;
    int i = 0;
    while (i != up)
    { //Upstairs
        int x = (rand() % (max_dimX + 1));
        int y = (rand() % (max_dimY + 1));
        if (dungeon->map[y][x] == '.')
        {
            dungeon->map[y][x] = '<';
            dungeon->upstrs[i].y = y;
            dungeon->upstrs[i].x = x;
            i++;
        }
    }
    i = 0;
    while (i != down)
    { //Downstairs
        int x = (rand() % (max_dimX + 1));
        int y = (rand() % (max_dimY + 1));
        if (dungeon->map[y][x] == '.')
        {
            dungeon->map[y][x] = '>';
            dungeon->downstrs[i].y = y;
            dungeon->downstrs[i].x = x;
            i++;
        }
    }
}
void assign_hardness(dungeon_t *dungeon)
{
    int y, x;
    for (y = 0; y < max_dimY; y++)
    {
        for (x = 0; x < max_dimX; x++)
        {
            if (dungeon->map[y][x] == '-' || dungeon->map[y][x] == '|')
            {
                dungeon->hardness[y][x] = 255;
            } //Immutable Border
            else if (dungeon->map[y][x] == '#')
            {
                dungeon->hardness[y][x] = 0;
            } //Corridor
            else if (dungeon->map[y][x] == '.')
            {
                dungeon->hardness[y][x] = 0;
            } //Downstairs
            else
            {
                dungeon->hardness[y][x] = (rand() % 254) + 1;
            } //Rock
        }
    }
}

void free_dungeon(dungeon_t *dungeon)
{
    free(dungeon->upstrs);
    free(dungeon->downstrs);
    free(dungeon->rooms);
    free(dungeon->monsters);
}

static int32_t corridor_path_cmp(const void *key, const void *with)
{
    return ((corridor_path_t *)key)->cost - ((corridor_path_t *)with)->cost;
}

void dijkstra(dungeon_t *d, uint8_t type)
{
    static corridor_path_t path[max_dimY][max_dimX], *p;
    static uint32_t initialized = 0;
    heap_t h;
    uint32_t x, y;

    if (!initialized)
    {
        for (y = 0; y < max_dimY; y++)
        {
            for (x = 0; x < max_dimX; x++)
            {
                path[y][x].pos[dim_y] = y;
                path[y][x].pos[dim_x] = x;
            }
        }
        initialized = 1;
    }

    for (y = 0; y < max_dimY; y++)
    {
        for (x = 0; x < max_dimX; x++)
        {
            path[y][x].cost = INT_MAX;
        }
    }

    path[d->pc.y][d->pc.x].cost = 0;

    heap_init(&h, corridor_path_cmp, NULL);

    for (y = 0; y < max_dimY; y++)
    {
        for (x = 0; x < max_dimX; x++)
        {
            if(type == 1){//non tunnel
                if (d->hardness[y][x] == 0)
                { 
                    path[y][x].hn = heap_insert(&h, &path[y][x]);
                }
                else
                {
                path[y][x].hn = NULL;
                }
            }
            if(type == 2){//tunnel
                if (d->hardness[y][x] != 255)
                {
                    path[y][x].hn = heap_insert(&h, &path[y][x]);
                }
                else
                {
                    path[y][x].hn = NULL;
                }
            }
        }
    }

    while ((p = heap_remove_min(&h)))
    {
        p->hn = NULL;
        //TOP CHECK
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x]].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x]].cost > p->cost + hardnesspair(p->pos)))
        {
            path[p->pos[dim_y] - 1][p->pos[dim_x]].cost = p->cost + (d->weight[p->pos[1]][p->pos[0]] / 85 + 1);
            path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x]].hn);
        }
        //LEFT CHECK
        if ((path[p->pos[dim_y]][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y]][p->pos[dim_x] - 1].cost > p->cost + hardnesspair(p->pos)))
        {
            path[p->pos[dim_y]][p->pos[dim_x] - 1].cost = p->cost + (d->hardness[p->pos[1]][p->pos[0]] / 85 + 1);
            path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y]][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y]][p->pos[dim_x] - 1].hn);
        }
        //RIGHT CHECK
        if ((path[p->pos[dim_y]][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y]][p->pos[dim_x] + 1].cost > p->cost + hardnesspair(p->pos)))
        {
            path[p->pos[dim_y]][p->pos[dim_x] + 1].cost = p->cost + (d->hardness[p->pos[1]][p->pos[0]] / 85 + 1);
            path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y]][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y]] [p->pos[dim_x] + 1].hn);
        }
        //BOTTOM CHECK
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x]].hn) &&(path[p->pos[dim_y] + 1][p->pos[dim_x]].cost > p->cost + hardnesspair(p->pos)))
        {
            path[p->pos[dim_y] + 1][p->pos[dim_x]].cost = p->cost + (d->hardness[p->pos[1]][p->pos[0]] / 85 + 1);
            path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x]].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x]].hn);
        }
        //TOP LEFT
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost > p->cost + hardnesspair(p->pos)))
        {
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost = p->cost + (d->hardness[p->pos[1]][p->pos[0]] / 85 + 1);
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn);
        }
        //TOP RIGHT
        if ((path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost > p->cost + hardnesspair(p->pos)))
        {
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].cost = p->cost + (d->hardness[p->pos[1]][p->pos[0]] / 85 + 1);
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1][p->pos[dim_x] + 1].hn);
        }
        //BOTTOM LEFT
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost > p->cost + hardnesspair(p->pos)))
        {
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost = p->cost + (d->hardness[p->pos[1]][p->pos[0]] / 85 + 1);
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn);
        }
        //BOTTOM RIGHT
        if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) && (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost > p->cost + hardnesspair(p->pos)))
        {
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost = p->cost + (d->hardness[p->pos[1]][p->pos[0]] / 85 + 1);
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_y] = p->pos[dim_y];
            path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].from[dim_x] = p->pos[dim_x];
            heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn);
        }

        if(type == 2){//Tunnel
            d->tunnelingMap[p->pos[dim_y]][p->pos[dim_x]] = p->cost;
        }
        if(type == 1){//Non-Tunnel
            d->nonTunnelingMap[p->pos[dim_y]][p->pos[dim_x]] = p->cost;
        }
    }
    heap_delete(&h);
    return;
}


void print_nonTunnelingMap(dungeon_t *dungeon)
{
    uint8_t type = 1;
    dijkstra(dungeon, type);
    //print_dungeon(dungeon, type);
}

void print_tunnelingMap(dungeon_t *dungeon)
{
    uint8_t type = 2;
    dijkstra(dungeon,type);
    //print_dungeon(dungeon, type);
}

void create_paths(dungeon_t *dungeon)
{
    uint8_t x, y;
    for (y = 0; y < max_dimY; y++)
    {
        for (x = 0; x < max_dimX; x++)
        {
            dungeon->tunnelingMap[y][x] = 1000;
            dungeon->nonTunnelingMap[y][x] = 1000;
        }
    }
    print_nonTunnelingMap(dungeon);
    print_tunnelingMap(dungeon);
}

void place_player(dungeon_t *dungeon)
{
    uint8_t x, y;

    while (1)
    {
        x = rand() % (max_dimX - 1) + 1;
        y = rand() % (max_dimY - 1) + 1;

        if (dungeon->map[y][x] == '.')
        {
            dungeon->pc.x = x;
            dungeon->pc.y = y;
            break;
        }
    }
    return;
}