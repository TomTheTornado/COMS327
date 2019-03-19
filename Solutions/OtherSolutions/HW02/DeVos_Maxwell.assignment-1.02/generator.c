#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "headers/generator.h"
#include "headers/color.h"

int generate(dungeon_t *d)
{
    if (!d->seed)
        d->seed = time(NULL);

    srand(d->seed);
    fill_map(d);
    smooth_hardness(d);
    place_rooms(d);
    place_corridors(d);
    place_stairs(d);
    place_player(d);

    return 0;
}

void fill_map(dungeon_t *d)
{
    uint8_t i, j;

    d->num_rooms = 0;

    for (i = 0; i < MAP_HEIGHT; i++) //height
    {
        for (j = 0; j < MAP_WIDTH; j++) //width
        {
            if (i == 0 || i == MAP_HEIGHT - 1) //y borders
            {
                d->map[i][j] = c_h_wall;
                d->hardness[i][j] = 255;
            }
            else if (j == 0 || j == MAP_WIDTH - 1) //x borders
            {
                d->map[i][j] = c_v_wall;
                d->hardness[i][j] = 255;
            }
            else
            {
                d->map[i][j] = c_rock;
                d->hardness[i][j] = (rand() % 254) + 1;
            }
        }
    }
}

void smooth_hardness(dungeon_t *d)
{
    uint8_t i, j;
    //smooth hardness with gausian filter
    int matrix[3][3] = {
        {0, 2, 0},
        {2, 4, 2},
        {0, 2, 0}};

    uint8_t reference[MAP_HEIGHT][MAP_WIDTH];
    for (i = 1; i < MAP_HEIGHT - 1; i++)
    {
        for (j = 1; j < MAP_WIDTH - 1; j++)
        {
            reference[i][j] = d->hardness[i][j];
        }
    }

    for (i = 1; i < MAP_HEIGHT - 1; i++)
    {
        for (j = 1; j < MAP_WIDTH - 1; j++)
        {
            d->hardness[i][j] = apply_square_matrix(3, matrix, j, i, reference);
        }
    }
    
    return;
}

void place_corridors(dungeon_t *d)
{

    int i;
    for (i = 0; i < d->num_rooms - 1; i++)
    {
        uint8_t x_cur, y_cur, tx, ty;

        //start location
        x_cur = d->rooms[i].x + (rand() % d->rooms[i].width);
        y_cur = d->rooms[i].y + (rand() % d->rooms[i].height);

        //target location
        tx = d->rooms[i + 1].x + (rand() % d->rooms[i + 1].width);
        ty = d->rooms[i + 1].y + (rand() % d->rooms[i + 1].height);

        while ((x_cur != tx) || (y_cur != ty))
        {
            int8_t horizontal_movement = (x_cur < tx) ? 1 : -1;
            int8_t vertical_movement = (y_cur < ty) ? 1 : -1;

            uint8_t pot_x = x_cur + horizontal_movement;
            uint8_t pot_y = y_cur + vertical_movement;

            float distance_x = sqrt(pow((tx - pot_x), 2) + pow(ty - y_cur, 2));
            float distance_y = sqrt(pow((tx - x_cur), 2) + pow(ty - pot_y, 2));
            float previous_d = sqrt(pow((tx - x_cur), 2) + pow(ty - y_cur, 2));

            if (distance_x < previous_d && distance_y < previous_d)
            {
                if (d->hardness[y_cur][pot_x] < d->hardness[pot_y][y_cur])
                    x_cur += horizontal_movement;
                else
                    y_cur += vertical_movement;
            }
            else
            {
                if (distance_x < distance_y)
                    x_cur += horizontal_movement;
                else
                    y_cur += vertical_movement;
            }

            add_corridor(d, x_cur, y_cur);
        }
    }
    return;
}

void add_corridor(dungeon_t *d, uint8_t x, uint8_t y)
{

    if (d->map[y][x] == c_rock)
    {
        d->map[y][x] = c_corridor;
        d->hardness[y][x] = 0;
    }
    return;
}

void place_stairs(dungeon_t *d)
{
    uint8_t x, y;
    while (1)
    {
        x = rand() % (MAP_WIDTH - 1) + 1;
        y = rand() % (MAP_HEIGHT - 1) + 1;

        if (d->hardness[y][x] == 0 && d->map[y][x] != c_u_stairs && d->map[y][x] != c_player)
        {
            d->num_downstairs = 1;
            d->downstairs = malloc(sizeof(location_t));
            d->downstairs[0].x = x;
            d->downstairs[0].y = y;

            d->map[y][x] = c_d_stairs;
            d->hardness[y][x] = 0;
            break;
        }
    }

    while (1)
    {
        x = rand() % (MAP_WIDTH - 1) + 1;
        y = rand() % (MAP_HEIGHT - 1) + 1;

        if (d->hardness[y][x] == 0 && d->map[y][x] != c_d_stairs && d->map[y][x] != c_player)
        {
            d->num_upstairs = 1;
            d->upstairs = malloc(sizeof(location_t));
            d->upstairs[0].x = x;
            d->upstairs[0].y = y;

            d->map[y][x] = c_u_stairs;
            d->hardness[y][x] = 0;
            break;
        }
    }
    return;
}

void place_player(dungeon_t *d)
{
    uint8_t x, y;

    while (1)
    {
        x = rand() % (MAP_WIDTH - 1) + 1;
        y = rand() % (MAP_HEIGHT - 1) + 1;

        if (d->hardness[y][x] == 0)
        {
            d->player.x = x;
            d->player.y = y;
            break;
        }
    }
    return;
}

int apply_square_matrix(int matrix_size, int matrix[matrix_size][matrix_size], uint8_t x, uint8_t y, uint8_t map[MAP_HEIGHT][MAP_WIDTH])
{
    uint32_t sum = 0;

    uint8_t i, j;
    for (i = 0; i < matrix_size; i++)
        for (j = 0; j < matrix_size; j++)
            sum += matrix[i][j] * map[y + (i - 1)][x + (j - 1)];

    return (sum / (matrix_size * matrix_size));
}

void free_dungeon(dungeon_t *d)
{
    free(d->upstairs);
    free(d->downstairs);
    free(d->rooms);
}

void draw_map(dungeon_t *d)
{
    uint8_t i, j;

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            terrain_t terrain;

            if (i == d->player.y && j == d->player.x)
                terrain = c_player;
            else
                terrain = d->map[i][j];

            if (COLOR)
            {
                uint8_t hardness = d->hardness[i][j];

                if (hardness == 0)
                    printf(BG_GROUND);
                else if (hardness == 255)
                    printf(BG_BLACK C_BLACK);
                else if (hardness < 85)
                    printf(BG_GREY);
                else if (hardness < 170)
                    printf(BG_DARK_GREY);
                else
                    printf(BG_DARK);

                if (terrain == c_player)
                    printf(C_BLUE);
                else if (terrain == c_d_stairs || terrain == c_u_stairs)
                    printf(C_YELLOW);
            }

            switch (terrain)
            {
            case c_rock:
                putchar(' ');
                break;
            case c_h_wall:
                putchar('-');
                break;
            case c_v_wall:
                putchar('|');
                break;
            case c_room:
                putchar('.');
                break;
            case c_corridor:
                putchar('#');
                break;
            case c_player:
                putchar('@');
                break;
            case c_u_stairs:
                putchar('>');
                break;
            case c_d_stairs:
                putchar('<');
                break;
            default:
                putchar('*');
                break;
            }

            printf(A_C_R);
        }
        printf("\n");
    }
    return;
}