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
#include "headers/movement.h"
#include "headers/heap.h"

void erraticPC(dungeon_t *d){
    uint8_t direction;
    direction = (rand() % 8) + 1;

    if(direction == 1 && d->hardness[d->pc.y - 1][d->pc.x] == 0){
        d->pc.y = d->pc.y - 1;
        d->pc.x = d->pc.x;
    }
    else if(direction == 2 && d->hardness[d->pc.y - 1][d->pc.x - 1] == 0){
        d->pc.y = d->pc.y - 1;
        d->pc.x = d->pc.x - 1;
    }
    else if(direction == 3 && d->hardness[d->pc.y - 1][d->pc.x + 1] == 0){
        d->pc.y = d->pc.y - 1;
        d->pc.x = d->pc.x + 1;
    }
    else if(direction == 4 && d->hardness[d->pc.y][d->pc.x - 1] == 0){
        d->pc.y = d->pc.y;
        d->pc.x = d->pc.x - 1;
    }
    else if(direction == 5 && d->hardness[d->pc.y][d->pc.x + 1] == 0){
        d->pc.y = d->pc.y;
        d->pc.x = d->pc.x + 1;
    }
    else if(direction == 6 && d->hardness[d->pc.y + 1][d->pc.x] == 0){
        d->pc.y = d->pc.y + 1;
        d->pc.x = d->pc.x;
    }
    else if(direction == 7 && d->hardness[d->pc.y + 1][d->pc.x - 1] == 0){
        d->pc.y = d->pc.y + 1;
        d->pc.x = d->pc.x - 1;
    }
    else if(direction == 8 && d->hardness[d->pc.y + 1][d->pc.x + 1] == 0){
        d->pc.y = d->pc.y + 1;
        d->pc.x = d->pc.x + 1;
    }
    else{
        d->pc.y = d->pc.y;
        d->pc.x = d->pc.x;
    }
}

//up
void monsterN(dungeon_t *d, monster_t *m){
    m->location.y -= 1;
    d->monsterMap[m->location.y][m->location.x];
}

//up right
void monsterNW(dungeon_t *d, monster_t *m){
    m->location.x -= 1;
    m->location.y -= 1;
    d->monsterMap[m->location.y][m->location.x];
}

//up left
void monsterNE(dungeon_t *d, monster_t *m){
    m->location.x += 1;
    m->location.y -= 1;
    d->monsterMap[m->location.y][m->location.x];
}

//right
void monsterWest(dungeon_t *d, monster_t *m){
    m->location.x -= 1;
    d->monsterMap[m->location.y][m->location.x];
}

//left
void monsterEast(dungeon_t *d, monster_t *m){
    m->location.x += 1;
    d->monsterMap[m->location.y][m->location.x];
}

//down
void monsterSouth(dungeon_t *d, monster_t *m){
    m->location.y += 1;
    d->monsterMap[m->location.y][m->location.x];
}

//down right
void monsterSW(dungeon_t *d, monster_t *m){
    m->location.x -= 1;
    m->location.y += 1;
    d->monsterMap[m->location.y][m->location.x];
}

//down left
void monsterSE(dungeon_t *d, monster_t *m){
    m->location.x += 1;
    m->location.y += 1;
    d->monsterMap[m->location.y][m->location.x];
}

void intelligenceMove(dungeon_t *d){

}

void telepathyMove(dungeon_t *d){

}

void tunnelingMove(dungeon_t *d){
    
}

void erraticMove(dungeon_t *d, monster_t *m){
    uint8_t direction;
    direction = (rand() % 8) + 1;

    if(direction = 1){monsterN(d, m);}
    if(direction = 2){monsterN(d, m);}
    if(direction = 3){monsterN(d, m);}
    if(direction = 4){monsterN(d, m);}
    if(direction = 5){monsterN(d, m);}
    if(direction = 6){monsterN(d, m);}
    if(direction = 7){monsterN(d, m);}
    if(direction = 8){monsterN(d, m);}
}


void monsterMovement(dungeon_t *d){
    uint8_t i;
    for(i = 0; i < d->numMonsters; i++){
        if(d->monsters[i].erratic == true){
            erraticMove(d, &d->monsters[i]);
        }
    }
}