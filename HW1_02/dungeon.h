#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <endian.h>

#define fileMarker "RLG327-S2019"
//Sets max dimensions of output as constants.
#define max_dimX 80
#define max_dimY 21
//Sets min dimensions for the rooms as constants.
#define min_rmX 4
#define min_rmY 3
//Sets max dimensions for the rooms as constants.
#define max_rmX 15
#define max_rmY 12
//Sets the maximum and minimum amount of rooms.
#define min_rms 6
#define max_rms 10
//Sets max stairs
#define max_upstrs 2
#define max_downstrs 2

typedef struct room{
    uint8_t x;
    uint8_t y;
    uint8_t height;
    uint8_t length;
} room_t;

typedef struct stairs{
    uint8_t x;
    uint8_t y;
} stairs_t;

typedef struct pc{
    uint8_t x;
    uint8_t y;
} pc_t;

typedef struct dungeon{
    char map[max_dimY][max_dimX];
    uint8_t hardness[max_dimY][max_dimX];
    room_t *rooms;
    uint16_t numRooms;
    stairs_t *upstrs;
    stairs_t *downstrs;
    uint8_t numUpStrs;
    uint8_t numDownStrs;
    pc_t pc;
} dungeon_t;

/**
 * Assigns a border for the 80(x-axis) x 21(y-axis) output.
 * X-axis uses '-' and Y-axis uses '|'
 */
void assign_border(dungeon_t *dungeon);

/**
 * Checks to see if a room can be originated in its random x-y location.
 */
bool origin(dungeon_t *dungeon, int x, int y);

/**
 * Checks to see if a room is able to be fit in its random location.
 */
bool proximity(dungeon_t *dungeon, int startX, int startY, int length, int height);

/**
 * Assigns the '.' character to the positions of the rooms.
 */
void assign_rooms(dungeon_t *dungeon, int startX, int startY, int length, int height);

/**
 * Generates the number of rooms with a decay method. 50% chance for only 6 rooms.
 * 25% chance for 7 rooms, 12.5% chance for 8 rooms, etc. No more than 10 rooms.
 */
int numOfRooms(int seed);

/**
 * Creates the rooms for the dungeon.
 */
void create_rooms(dungeon_t *dungeon, int seed);

/**
 * Assigns the char '#' to the corridors.
 */
void assign_corridors(dungeon_t *dungeon, int destX, int destY, int cornerX, int cornerY);

/**
 * Creates corridors to connect the rooms.
 */
void create_corridors(dungeon_t *dungeon, int seed);

/**
 * Creates stairs.
 */
void create_stairs(dungeon_t *dungeon, int seed);

/**
 * Assigns hardness to the dungeon.
 */
void assign_hardness(dungeon_t *dungeon);