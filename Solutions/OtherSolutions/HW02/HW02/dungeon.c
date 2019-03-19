#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "headers/generator.h"
#include "headers/load_save.h"

/**
 * Dungeon crawler game
 * --save, -s
 * --load, -l
 * --seed
 * */
int main(int argc, char const *argv[])
{
    action_t action;
    dungeon_t dungeon;
    dungeon.seed = 0;

    int i;
    for (i = 1; i < argc; i++) //check for flags
    {
        if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "-s"))
            action = (action != load) ? save : load_save;
        else if (!strcmp(argv[i], "--load") || !strcmp(argv[i], "-l"))
            action = (action != save) ? load : load_save;
        else if (!strcmp(argv[i], "--seed") && (i + 1 < argc))
            dungeon.seed = atoi(argv[i + 1]);
        else if(!strcmp(argv[i], "-sl") || !strcmp(argv[i], "-ls")){
            action = load_save;
        }
    }

    switch (action)
    {
    case load:
        printf("loading...\n");
        load_dungeon(&dungeon);
        break;
    case save:
        printf("saving...\n");
        generate(&dungeon);
        save_dungeon(&dungeon);
        break;
    case load_save:
        printf("Loading and saving...\n");
        load_dungeon(&dungeon);
        save_dungeon(&dungeon);
        break;
    default:
        generate(&dungeon);
        break;
    }

    printf("Seed: %d\n", dungeon.seed);
    draw_map(&dungeon);

    free_dungeon(&dungeon);

    return 0;
}
