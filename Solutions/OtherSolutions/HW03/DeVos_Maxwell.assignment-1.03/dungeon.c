#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "headers/dungeon.h"
#include "headers/generator.h"
#include "headers/load_save.h"
#include "headers/render.h"
#include "headers/path_finding.h"

void usage(char *name)
{
    fprintf(stderr, "Usage: %s [--seed <seed>] [-l|--load ]\n"
                    "          [-s|--save ]\n",
            name);
    exit(-1);
}

/**
 * Dungeon Crawler Game
 * --save, -s
 * --load, -l
 * --seed <SEED>
 * 
 * Tested on MacOs and Linux
 * */
int main(int argc, char *argv[])
{
    action_t action;
    dungeon_t d;
    d.seed = 0;
    int i;

    // if (argc > 1)
    // {
    //     for (i = 1; i < argc; i++)
    //     {
    //         if (argv[i][0] == '-')
    //         {
    //             if (argv[i][1] == '-')
    //                 argv[i]++;

    //             switch (argv[i][1])
    //             {
    //             case 's':
    //                 if (strcmp(argv[i], "-save") && strcmp(argv[i], "-s"))
    //                     usage(argv[0]);
    //                 action = (action != load) ? save : load_save;
    //                 break;
    //             case 'l':
    //                 if (strcmp(argv[i], "-load") && strcmp(argv[i], "-l"))
    //                     usage(argv[0]);
    //                 action = (action != save) ? load : load_save;
    //                 break;
    //             case 'r':
    //                 // if (strcmp(argv[i], "--rand="))
    //                 //     usage(argv[0]);
    //                 // action = (action != save) ? load : load_save;
    //                 break;
    //             default:
    //                 usage(argv[0]);
    //             }
    //         }
    //         else
    //         {
    //             usage(argv[0]);
    //         }
    //     }
    for (i = 1; i < argc; i++) //check for flags
    {
        if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "-s"))
            action = (action != load) ? save : load_save;
        else if (!strcmp(argv[i], "--load") || !strcmp(argv[i], "-l"))
            action = (action != save) ? load : load_save;
        else if (!strcmp(argv[i], "--seed") && (i + 1 < argc))
            d.seed = atoi(argv[i + 1]);
        else if (!strcmp(argv[i], "-sl") || !strcmp(argv[i], "-ls"))
        {
            action = load_save;
        }
    }

    switch (action)
    {
    case load:
        printf("loading...\n");
        load_dungeon(&d);
        break;
    case save:
        printf("saving...\n");
        generate(&d);
        save_dungeon(&d);
        break;
    case load_save:
        printf("Loading and saving...\n");
        load_dungeon(&d);
        save_dungeon(&d);
        break;
    default:
        generate(&d);
        break;
    }

    get_distance_maps(&d);

    //player position
    printf("\nPC is at (X: %d, Y: %d)\n", d.player.pos.x, d.player.pos.y);
    printf("Terrain Map\n");
    draw_map(&d);

    draw_distance_map(&d, 0);

    draw_distance_map(&d, 1);

    draw_hardness(&d);

    draw_movement_cost(&d);

    //free anything because program is ending
    printf("\n");
    free_dungeon(&d);

    return 0;
}
