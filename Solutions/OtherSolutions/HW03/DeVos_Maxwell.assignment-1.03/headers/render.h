#ifndef render_h
#define render_h

/**
 * Draws the map based on the given map array
 * */
void draw_map(dungeon_t *d);

/**
 * Draws a graphical representation of the hardness using hexidecimal
 * */
void draw_hardness(dungeon_t *d);

/**
 * Draws the movement cost
 * */
void draw_movement_cost(dungeon_t *d);

/**
 * Draws the generated distance maps
 * */
void draw_distance_map(dungeon_t *d, uint8_t mode);

#endif