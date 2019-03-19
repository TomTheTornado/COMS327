#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include <stdint.h>


//defines the maps width in characters
#define MAP_WIDTH 80
//defines the maps height in characters
#define MAP_HEIGHT 21
//defines the minimum number of rooms
#define MIN_ROOMS 6
//defines the minimum number of rooms
#define MAX_ROOMS 10
//defines the minimum room width in characters
#define MIN_ROOM_WIDTH 4
//defines the minimum room height in characters
#define MIN_ROOM_HEIGHT 3
//defines the maximum room size in characters
#define MAX_ROOM_SIZE 10
///The amount of times a room will be atempted to be placed
#define MAX_ROOM_ATTEMPTS 1000
//defines whether color should be rendered
#define COLOR 1

/**
 * Room Structure
 * */
typedef struct room
{
    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
} room_t;

/**
 * Player
 * */
typedef struct player
{
    uint8_t x;
    uint8_t y;
} player_t;

typedef struct location
{
    uint8_t x;
    uint8_t y;
} location_t;

/**
 * Enum of character values for storing in map array
 * */
typedef enum terrain
{
    c_rock,
    c_h_wall,
    c_v_wall,
    c_room,
    c_corridor,
    c_player,
    c_u_stairs,
    c_d_stairs
} terrain_t;

typedef struct dungeon
{
    uint32_t seed;
    uint16_t num_rooms;
    player_t player;
    terrain_t map[MAP_HEIGHT][MAP_WIDTH];
    uint8_t hardness[MAP_HEIGHT][MAP_WIDTH];
    uint16_t num_upstairs;
    location_t *upstairs;
    uint16_t num_downstairs;
    location_t *downstairs;
    room_t *rooms;
} dungeon_t;

/**
 * Generates a dungeon
 * returns 0 if all went well
 * */
int generate(dungeon_t *d);

/**
 * Fills in the map with its initial values and adds borders.
 * */
void fill_map(dungeon_t *d);

/**
 * Connects all rooms with corridors
 * */
void place_rooms(dungeon_t *d);

/**
 * Adds a room to the map.
 * returns 0 if successful. 1 if it fails.
 * */
int add_room(dungeon_t *d);

/**
 * Creates a corridor between two rooms
 * */
void place_corridors(dungeon_t *d);

/**
 * Applys the position of the cursor drawing corridors to the map array
 * */
void add_corridor(dungeon_t *d, uint8_t x, uint8_t y);

/**
 * Adds stairs to the map
 * */
void place_stairs(dungeon_t *d);

/**
 * Adds the player to the map
 * */
void place_player(dungeon_t *d);

/**
 * Smooths hardness
 * */
void smooth_hardness(dungeon_t *d);

/**
 * Applies a square matrix over a larger matrix
 * */
int apply_square_matrix(int matrix_size, int matrix[matrix_size][matrix_size], uint8_t x, uint8_t y, uint8_t map[MAP_HEIGHT][MAP_WIDTH]);

/**
 * Free's the arrays that are part of the dungeon_t struct
 * */
void free_dungeon(dungeon_t *d);

/**
 * --------------------------Temporary in this location for testing--------------------------
 * Draws the map based on the given map array
 * */
void draw_map(dungeon_t *d);

#endif