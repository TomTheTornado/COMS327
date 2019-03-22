#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#include "headers/dungeon.h"
#include "headers/moveIn.h"
#include "headers/move.h"
#include "headers/pc.h"
#include "headers/npc.h"

const char *quitInMove =
  "\n"
  "                                                                                \n"
  "                                                                                \n"
  "                                                                                \n"
  " ,ggg,         gg                                                               \n"  
  "dP''Y8a        88                                                           I8  \n"
  "Yb, `88        88                                                           I8  \n" 
  " `'  88        88                                                    gg  8888888\n"
  "     88        88                                                    ''     I8  \n"
  "     88        88    ,ggggg, gg      gg       ,gggg,gg  gg      gg   gg     I8  \n" 
  "     88       ,88   dP'  'Y8gI8      8I      dP'  'Y8I  I8      8I   88     I8  \n" 
  "     Y8b,   ,d888  i8'    ,8II8,    ,8I     i8'    ,8I  I8,    ,8I   88    ,I8, \n"
  "      'Y88888P'88,,d8,   ,d8'd8b,  ,d8b,   ,d8,   ,d8b ,d8b,  ,d8b, ,88,  ,d88b,\n"
  "           ,ad8888P'Y8888P' 8P''Y88P'`Y8   P'Y8888P'88d8P''Y88P'`Y88P''Y888P''Y8\n"
  "          d8P' 88                                   I8P                         \n"
  "        ,d8'   88                                   I8'                         \n"
  "        d8'    88                                   I8                          \n"
  "        88     88                                   I8                          \n"
  "        Y8,   ,88                                   I8                          \n"
  "         'Y888P'                                    I8                          \n"
  "                                                                                \n"
  "                                                                                \n";

void nextMoveKey(int n, dungeon_t *d){
    int pc_x, pc_y;
    pc_x = d->pc.position[dim_x];
    pc_y = d->pc.position[dim_y];
    char *clrline = "                                                                               ";
    mvaddstr(0, 0, clrline);
    
    if(n == '9' || n == 'u' || n == 'U'){
        if(d->map[pc_y - 1][pc_x + 1] == ter_wall || d->map[pc_y - 1][pc_x + 1] == ter_wall_immutable){
            mvaddstr(0, 0, "You can't move into the wall.");
        }
        else{
            do_moves(d, 9);
            mvaddstr(0, 0, "Last Move: Up 1 and Right 1");
        }
    }
    else if(n == '8' || n == 'k' || n == 'K'){// also check if valid
        if(d->map[pc_y - 1][pc_x] == ter_wall || d->map[pc_y - 1][pc_x] == ter_wall_immutable){
            mvaddstr(0, 0, "You can't move into the wall.");
        }
        else{
            do_moves(d, 8);
            mvaddstr(0, 0, "Last Move: Up 1");
        }
    }
    else if(n == '7' || n == 'y' || n == 'Y'){//make if and have move character determine x and y uppand down
        if(d->map[pc_y - 1][pc_x - 1] == ter_wall || d->map[pc_y - 1][pc_x - 1] == ter_wall_immutable){
            mvaddstr(0, 0, "You can't move into the wall.");
        }
        else{
            do_moves(d, 7);
            mvaddstr(0, 0, "Last Move: Up 1 and Left 1");
        }
    }
    else if(n == '6' || n == 'l' || n == 'L'){
        if(d->map[pc_y][pc_x + 1] == ter_wall || d->map[pc_y][pc_x + 1] == ter_wall_immutable){
            mvaddstr(0, 0, "You can't move into the wall.");
        }
        else{
            do_moves(d, 6);
            mvaddstr(0, 0, "Last Move: Right 1");
        }
    }
    else if(n == '5' || n == ' ' || n == '.'){
        do_moves(d, 5);
        mvaddstr(0, 0, "Last Move: Rested for a turn.");
    }
    else if(n == '4' || n == 'h' || n == 'H'){
        if(d->map[pc_y][pc_x - 1] == ter_wall || d->map[pc_y][pc_x - 1] == ter_wall_immutable){
            mvaddstr(0, 0, "You can't move into the wall.");
        }
        else{
            do_moves(d, 4);
            mvaddstr(0, 0, "Last Move: Left 1");
        }
    }
    else if(n == '3' || n == 'n' || n == 'N'){
       if(d->map[pc_y + 1][pc_x + 1] == ter_wall || d->map[pc_y + 1][pc_x + 1] == ter_wall_immutable){
            mvaddstr(0, 0, "You can't move into the wall.");
        }
        else{
            do_moves(d, 3);
            mvaddstr(0, 0, "Last Move: Down 1 and Right 1");
        }
    }
    else if(n == '2' || n == 'j' || n == 'J'){
        if(d->map[pc_y + 1][pc_x] == ter_wall || d->map[pc_y + 1][pc_x] == ter_wall_immutable){
            mvaddstr(0, 0, "You can't move into the wall.");
        }
        else{
            do_moves(d, 2);
            mvaddstr(0, 0, "Last Move: Down 1");
        }
    }
    else if(n == '1' || n == 'b' || n == 'B'){
        if(d->map[pc_y + 1][pc_x - 1] == ter_wall || d->map[pc_y + 1][pc_x - 1] == ter_wall_immutable){
            mvaddstr(0, 0, "You can't move into the wall.");
        }
        else{
            do_moves(d, 1);
            mvaddstr(0, 0, "Last Move: Up 1 and Left 1");
        }
    }
    
    else if(n == '>'){
        //int x, y;
        if(d->map[pc_y][pc_x] == ter_stairs_down){
            mvaddstr(0, 0, "Last Move: Traveled Downstairs");

            delete_dungeon(d);
            srand(time(NULL));
            init_dungeon(d);
            gen_dungeon(d);
            config_pc(d);
            gen_monsters(d);

        }
        else{
            mvaddstr(0, 0, "You need to be on a downstairs to go downstairs.");
        }
    }
    else if(n == '<'){
        if(d->map[pc_y][pc_x] == ter_stairs_up){
            mvaddstr(0, 0, "Last Move: Traveled Upstairs");
            
            delete_dungeon(d);
            srand(time(NULL));
            init_dungeon(d);
            gen_dungeon(d);
            config_pc(d);
            gen_monsters(d);
        }
        else{
            mvaddstr(0, 0, "You need to be on a upstairs to go upstairs.");
        }
    }
    else if(n == 'm' || n == 'M'){ //put all of these into the display monsters
        int display, numMonster;
        display = 1;
        numMonster = 0;
        int x, y;
        char monChar[d->num_monsters];
        int monLocation[d->num_monsters][2];//y then x
        for (y = 0; y < DUNGEON_Y; y++){
            for (x = 0; x < DUNGEON_X; x++){
                if(d->character[y][x] != NULL){
                    if(d->pc.position[dim_y] == y && d->pc.position[dim_x] == x){

                    }
                    else{
                        monChar[numMonster] = d->character[y][x]->symbol;
                        monLocation[numMonster][0] = y;
                        monLocation[numMonster][1] = x;
                        numMonster++;
                    }
                }
            }
        }
        numMonster = 0;
        while(display){
            if(monChar[numMonster]){

            }
            
            char intStr[3];
            mvaddstr(0, 0, "Number of Monsters:");
            sprintf(intStr, "%d", numMonster + 1);
            mvaddstr(0, 20, intStr);
            mvaddstr(0, 23, "/");
            sprintf(intStr, "%d", d->num_monsters);
            mvaddstr(0, 24, intStr);

            mvaddstr(0, 28, "Monster: ");
            mvaddch(0, 37, monChar[numMonster]);

            mvaddstr(0, 40, "Location: ");
            //Looks at Y
            if(monLocation[numMonster][0] == d->pc.position[dim_y]){
                mvaddstr(0, 50, "Same Latitude");
            }
            else if(monLocation[numMonster][0] > d->pc.position[dim_y]){
                mvaddstr(0, 50, "South");
                sprintf(intStr, "%d", monLocation[numMonster][0] - d->pc.position[dim_y]);
                mvaddstr(0, 56, intStr);
                if(monLocation[numMonster][1] != d->pc.position[dim_x]){
                    mvaddstr(0, 59, "and");
                }
            }
            else{
                mvaddstr(0, 50, "North");
                sprintf(intStr, "%d", d->pc.position[dim_y] - monLocation[numMonster][0]);
                mvaddstr(0, 56, intStr);
                if(monLocation[numMonster][1] != d->pc.position[dim_x]){
                    mvaddstr(0, 59, "and");
                }
            }
            //Looks at X
            if(monLocation[numMonster][1] == d->pc.position[dim_x]){
                mvaddstr(0, 65, "Same Longitude");
            }
            else if(monLocation[numMonster][1] > d->pc.position[dim_x]){
                mvaddstr(0, 65, "East");
                sprintf(intStr, "%d", monLocation[numMonster][1] - d->pc.position[dim_x]);
                mvaddstr(0, 70, intStr);
            }
            else{
                mvaddstr(0, 65, "West");
                sprintf(intStr, "%d", d->pc.position[dim_x] - monLocation[numMonster][1]);
                mvaddstr(0, 70, intStr);
            }

            render_dungeon(d);

            int keyIn;
            keyIn = 0;
            keyIn = getch();
            if(keyIn == KEY_UP && numMonster < d->num_monsters - 1){
                numMonster++;
            }
            else if(keyIn == KEY_DOWN && numMonster > 0){
                numMonster--;
            }
            else if(keyIn == 27){//esc equivalent
                mvaddstr(0, 0, clrline);
                display = 0;
            }
            mvaddstr(0, 0, clrline);
        }
    }
    else{
        //printf("Non Key Function ");
        mvaddstr(0, 0, clrline);
    }
    //printf(" Previous Move Input: %c\n\n", n);
    return;
}