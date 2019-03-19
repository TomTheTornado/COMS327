#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdint.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 21
#define COLOR 1 //1 means draw in color

#define mapxy(x, y) (d->map[y][x])
#define hardnessxy(x, y) (d->hardness[y][x])
#define costxy(x, y) (d->hardness[y][x] / 85 + 1)

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

/**
 * Position struct
 * */
typedef struct pos
{
    uint8_t x;
    uint8_t y;
} pos_t;

/**
 * Dimension struct
 * */
typedef struct dim
{
    uint8_t h;
    uint8_t w;
} dim_t;

/**
 * Room Structure
 * */
typedef struct room
{
    pos_t pos;
    dim_t dim;
} room_t;

/**
 * Player
 * */
typedef struct player //refrac
{
    pos_t pos;
} player_t;

typedef struct dungeon
{

    uint32_t seed;
    terrain_t map[MAP_HEIGHT][MAP_WIDTH];
    uint8_t hardness[MAP_HEIGHT][MAP_WIDTH];

    uint16_t num_rooms;
    room_t *rooms;

    uint16_t num_upstairs;
    pos_t *upstairs;

    uint16_t num_downstairs;
    pos_t *downstairs;

    player_t player;
    int32_t non_tunneling_map[MAP_HEIGHT][MAP_WIDTH];
    int32_t tunneling_map[MAP_HEIGHT][MAP_WIDTH];

} dungeon_t;

#endif