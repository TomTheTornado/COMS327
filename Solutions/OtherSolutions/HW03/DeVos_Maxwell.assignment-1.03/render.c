#include <stdio.h>

#include "headers/dungeon.h"
#include "headers/color.h"

void draw_map(dungeon_t *d)
{
    uint8_t i, j;

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            terrain_t terrain;

            if (i == d->player.pos.y && j == d->player.pos.x)
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

void draw_hardness(dungeon_t *d)
{
    printf("\nHardness Map\n");
    uint8_t i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
            printf("%02x", d->hardness[i][j]);
        printf("\n");
    }
    return;
}

void draw_movement_cost(dungeon_t *d)
{
    printf("\nMovement Cost Map\n");

    uint8_t i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
            if (d->hardness[i][j] < 255)
                printf("%d", (d->hardness[i][j] != 0) ? costxy(j,i) : 1);
            else
                putchar('X');

        printf("\n");
    }
    return;
}

void draw_distance_map(dungeon_t *d, uint8_t mode)
{
    if(mode)
        printf("\nNon Tunneling Map\n");
    else
        printf("\nTunneling Distance Map\n");
    
    uint8_t i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            if (d->player.pos.y != i || d->player.pos.x != j)
            {
                int value;

                if (mode)
                    value = d->tunneling_map[i][j];
                else
                    value = d->non_tunneling_map[i][j];

                if (value > 0)
                    printf("%1d", value % 10);
                else if (value || mode)
                    putchar('X');
                else
                    putchar(' ');
            }
            else
            {
                printf("@");
            }
        }
        printf("\n");
    }
    return;
}