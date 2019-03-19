#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include <stdint.h>

#define MAX_ROOM_ATTEMPTS 1000
#define MIN_ROOMS 6
#define MAX_ROOMS 10
#define MIN_ROOM_WIDTH 4
#define MIN_ROOM_HEIGHT 3
#define MAX_ROOM_SIZE 10

/**
 * Generates a dungeon
 * */
int generate(dungeon_t *d);

/**
 * Inits the terrain map and hardness
 * */
void fill_map(dungeon_t *d);

/**
 * Free's the arrays that are part of the dungeon_t struct
 * */
void free_dungeon(dungeon_t *d);

#endif