#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "headers/dungeon.h"
#include "headers/moveIn.h"

void nextMoveKey(int n){
    if(n == '7' || n == 'y' || n == 'Y'){
        printf("Upper Left       ");
    }
    else if(n == '8' || n == 'k' || n == 'K'){
        printf("Up One           ");
    }
    else if(n == '9' || n == 'u' || n == 'U'){
        printf("Upper Right      ");
    }
    else if(n == '6' || n == 'l' || n == 'L'){
        printf("Right One        ");
    }
    else if(n == '3' || n == 'n' || n == 'N'){
        printf("Lower Right      ");
    }
    else if(n == '2' || n == 'j' || n == 'J'){
        printf("Down One         ");
    }
    else if(n == '1' || n == 'b' || n == 'B'){
        printf("Lower Left       ");
    }
    else if(n == '4' || n == 'h' || n == 'H'){
        printf("Left One         ");
    }
    else if(n == '>'){
        printf("Down Stairs      ");
    }
    else if(n == '<'){
        printf("Up Stairs        ");
    }
    else if(n == '5' || n == ' ' || n == '.'){
        printf("Rest for a turn  ");
    }
    else if(n == 'm' || n == 'M'){
        printf("Display Monsters ");
    }
    else if(n == KEY_UP){
        printf("Up Monster List  ");
    }
    else if(n == KEY_DOWN){
        printf("Down Monster List");
    }
    else if(n == 27){//esc equivalent
        printf("ESC  Monster List");
    }
    else if(n == 'q' || n == 'Q'){
        printf("Quit Game        ");
    }
    else{
        printf("Non Key Function ");
    }
    printf(" Previous Move Input: %c\n\n", n);
    return;
}