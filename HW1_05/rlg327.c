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
        create_paths(&dungeon);
        //monsterMovement(&dungeon);
        //do_moves(&dungeon);

        print_dungeon(&dungeon, 0);     
    }

    free_dungeon(&dungeon);
    return 0;
}