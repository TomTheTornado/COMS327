#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

#include "sudoku.h"

//Game board
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @   |   |   @   |   |   @   |   |   @
// @---+---+---@---+---+---@---+---+---@
// @   |   |   @   |   |   @   |   |   @
// @---+---+---@---+---+---@---+---+---@
// @   |   |   @   |   |   @   |   |   @
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @   |   |   @   |   |   @   |   |   @
// @---+---+---@---+---+---@---+---+---@
// @   |   |   @   |   |   @   |   |   @
// @---+---+---@---+---+---@---+---+---@
// @   |   |   @   |   |   @   |   |   @
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// @   |   |   @   |   |   @   |   |   @
// @---+---+---@---+---+---@---+---+---@
// @   |   |   @   |   |   @   |   |   @
// @---+---+---@---+---+---@---+---+---@
// @   |   |   @   |   |   @   |   |   @
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//             1   1   2   2   2   3   3
// 0   4   8   2   6   0   4   8   2   6

//           1   1   1   2   2   3   3       
//   2   6   0   4   8   2   6   0   4   



const char gameboard[19][37] = {
	{'@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@', '-', '-', '-', '+', '-', '-', '-', '+', '-', '-', '-', '@'},
	{'@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@', ' ', ' ', ' ', '|', ' ', ' ', ' ', '|', ' ', ' ', ' ', '@'},
	{'@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@', '@'},
};

const char *triumph =
  "\n"
  "                ,:/+/-                              ##   ###   ##       \n"
  "                /M/              .,-=;@/;-          ##   ###   ##       \n"
  "           .:/= ;MH/,    ,=/+&$XH@MM#@:             ##   ###   ##       \n"
  "          -$##@+$###@H@MMM#######H:.    -/H#        ##   ###   ##       \n"
  "     .,H@H@ X######@ -H#####@+-     -+H###@X        '## ##'## ##'       \n"
  "      .,@##H;      +XM##M/,     =&@###@X;-           '###' '###'        \n"
  "    X&-  :M##########$.    .:&M###@&:                                   \n"
  "    M##H,   +H@@@$/-.  ,;$M###@&,          -        #############       \n"
  "    M####M=,,---,.-%%H####M$:          ,+@##             ###            \n"
  "    @##################@/.         :&H##@$-              ###            \n"
  "    M###############H,         ;HM##M$=                  ###            \n"
  "    #################.    .=$M##M$=                      ###            \n"
  "    ################H..;XM##M$=          .:+        #############       \n"
  "    M###################@&=           =+@MH&                            \n"
  "    @#################M/.         =+H#X&=              ###   ##         \n"
  "    =+M###############M,      ,/X#H+:,                 ####  ##         \n"
  "      .;XM###########H=   ,/X#H+:;                     ##### ##         \n"
  "        .=+HM#######M+/+HM@+=.                         ## #####         \n"
  "            ,:/%XM####H/.                              ##  ####         \n"
  "                  ,.:=-.                               ##   ###         \n"
  "                                   You win!\n";




int main(int argc, char *argv[]){

    sudoku s;
    
    time_t seed;
    struct timeval tv;
    memset(&s, 0, sizeof (s));


    //Random Seed For Puzzle
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;

    srand(seed);

    //Initiates the Game Here
    init_game(&s);

    clear();
    refresh();
    print_board();
    print_hints(&s);
    print_specific_num(&s);
    mvaddstr(13, 42, "Use the numpad or vim keys used in ");
    mvaddstr(14, 42, "RLG327 game to move around the board.");
    mvaddstr(16, 42, "Press 'e' to enter in a number 1-9.");
    mvaddstr(18, 42, "Press 'w' for more options.");
    mvaddstr(20, 42, "Press 'q' to quit.");

    s.highlight = 0;
    s.quit = 0;
    s.win = 0;
    s.cursor.x = 0;
    s.cursor.y = 0;
    while(s.quit == 0){
        game_input(&s);
        if(check_win(&s)){
            s.win = 1;
            attron(COLOR_PAIR(COLOR_CYAN));
            attron(A_BOLD);
            mvaddstr(6, 51, "You Win!");
            mvaddstr(8, 42, "Press any key to continue.");
            getch();
            attron(COLOR_PAIR(COLOR_CYAN));
            attron(A_BOLD);
            break;
        }
        else if(s.quit == 1 || s.win == 1){
            break;
        }
    }
    
    endwin();

    if(s.win == 1){
        cout << triumph;
    }

    /**
     * Section Below Used for Debugging
     */
    // cout << "Seed:" << seed << '\n' << "Solution" << '\n';
    // //Prints afterwards
    // uint32_t x, y;
    // for(y = 0; y < 9; y++){
    //     for(x = 0; x < 9; x++){
    //         cout << s.solution[y][x] << ' ';
    //     }
    //     cout << '\n';
    // }
    // cout << "Hints Locations Shown" << '\n';
    // for(y = 0; y < 9; y++){
    //     for(x = 0; x < 9; x++){
    //         cout << s.hints[y][x] << ' ';
    //     }
    //     cout << '\n';
    // }
    // cout << "User Input on Board" << '\n';
    // for(y = 0; y < 9; y++){
    //     for(x = 0; x < 9; x++){
    //         cout << s.userIn[y][x] << ' ';
    //     }
    //     cout << '\n';
    // }
}

//Initates terminal
void init_terminal(){
    //Creates screen
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);

    init_color(COLOR_DARKGRAY, 99, 99, 99);
    init_pair(COLOR_DARKGRAY, COLOR_WHITE, COLOR_DARKGRAY);

    init_pair(COLOR_TITANIUMWHITE, COLOR_WHITE, COLOR_WHITE);
    init_pair(COLOR_GREENGRAY, COLOR_GREEN, COLOR_DARKGRAY);
    init_pair(COLOR_HINT_LIGHT, COLOR_GREEN, COLOR_MAGENTA);
    init_pair(COLOR_NUM_LIGHT, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(COLOR_RED_LIGHT, COLOR_WHITE, COLOR_RED);
    init_pair(COLOR_BLUE_LIGHT, COLOR_WHITE, COLOR_BLUE);
}

//Checks if space is a hint
int is_hint(sudoku *s){
    return s->hints[s->cursor.y][s->cursor.x];
}

//Inserts num into userIn matrix
void insert_num(sudoku *s){
    attron(COLOR_PAIR(COLOR_CYAN));
    attron(A_BOLD);
    mvaddstr(6, 42, "Enter a number 1-9.");
    mvaddstr(8, 42, "Press '0' to empty space.");
    mvaddstr(10, 42, "Press ESC key to return");
    mvaddstr(11, 42, "to selection screen.");
    attroff(COLOR_PAIR(COLOR_CYAN));
    attroff(A_BOLD);
    uint32_t key, picked;
    picked = 1;
    while(picked){
        key = getch();
        switch(key){
            case '1':
                s->userIn[s->cursor.y][s->cursor.x] = 1;
                picked = 0;
                break;
            case '2':
                s->userIn[s->cursor.y][s->cursor.x] = 2;
                picked = 0;
                break;
            case '3':
                s->userIn[s->cursor.y][s->cursor.x] = 3;
                picked = 0;
                break;
            case '4':
                s->userIn[s->cursor.y][s->cursor.x] = 4;
                picked = 0;
                break;
            case '5':
                s->userIn[s->cursor.y][s->cursor.x] = 5;
                picked = 0;
                break;
            case '6':
                s->userIn[s->cursor.y][s->cursor.x] = 6;
                picked = 0;
                break;
            case '7':
                s->userIn[s->cursor.y][s->cursor.x] = 7;
                picked = 0;
                break;
            case '8':
                s->userIn[s->cursor.y][s->cursor.x] = 8;
                picked = 0;
                break;
            case '9':
                s->userIn[s->cursor.y][s->cursor.x] = 9;
                picked = 0;
                break;
            case '0':
                s->userIn[s->cursor.y][s->cursor.x] = 0;
                picked = 0;
                break;
            case 27:
                picked = 0;
                break;
            default:
                break;
        }
    }

}

//Recieves the user input for the game
void game_input(sudoku *s){
    uint32_t quit2, key, viewSol, hintLoc, incorrect;
    viewSol = 0;
    hintLoc = 0;
    incorrect = 0;
    key = getch();
    mvaddstr(10, 42, "                                     ");
    mvaddstr(11, 42, "                                ");
    mvaddstr(13, 42, "                                   ");
    mvaddstr(14, 42, "                                     ");
    mvaddstr(16, 42, "                                   ");
    mvaddstr(18, 42, "                           ");
    mvaddstr(20, 42, "                  ");
    switch(key){
        case 'q':
            attron(COLOR_PAIR(COLOR_CYAN));
            attron(A_BOLD);
            mvaddstr(6, 42, "Are you sure you want to quit?");
            mvaddstr(8, 55, "y/n");
            attron(COLOR_PAIR(COLOR_CYAN));
            attron(A_BOLD);
            refresh();
            quit2 = 0;
            while(quit2 == 0){
                key = getch();
                switch(key){
                    case 'y':
                    case 'Y':
                        s->quit = 1;
                        quit2 = 1;
                        break;
                    case 'n':
                    case 'N':
                        quit2 = 1;
                        break;
                    default:
                        break;
                }
            }
            break;
        case 'w':
            attron(COLOR_PAIR(COLOR_CYAN));
            attron(A_BOLD);
            mvaddstr(6, 42, "Press 's' to view sudoku solution.");
            mvaddstr(8, 42, "Press 'a' to view current");
            mvaddstr(9, 42, "incorrect answers.");
            mvaddstr(11, 42, "Press ESC key to return");
            mvaddstr(12, 42, "to selection screen.");
            attroff(COLOR_PAIR(COLOR_CYAN));
            attroff(A_BOLD);
            uint32_t wBreak;
            wBreak = 0;
            while(TRUE){
                key = getch();
                switch (key){
                    case 's':
                        viewSol = 1;
                        wBreak = 1;
                        break;
                    case 'a':
                        incorrect = 1;
                        wBreak = 1;
                        break;
                    case 27:
                        wBreak = 1;
                        break;
                    default:
                        break;
                }
                if(wBreak == 1){
                    break;
                }
            }
            break;
        case '1':
        case 'b':
        case KEY_END:
            move_cursor(s, -1, 1);
            break;
        case '2':
        case 'j':
        case KEY_DOWN:
            move_cursor(s, 0, 1);
            break;
        case '3':
        case 'n':
        case KEY_NPAGE:
            move_cursor(s, 1, 1);
            break;
        case '4':
        case 'h':
        case KEY_LEFT:
            move_cursor(s, -1, 0);
            break;
        case '6':
        case 'l':
        case KEY_RIGHT:
            move_cursor(s, 1, 0);
            break;
        case '7':
        case 'y':
        case KEY_HOME:
            move_cursor(s, -1, -1);
            break;
        case '8':
        case 'k':
        case KEY_UP:
            move_cursor(s, 0, -1);
            break;
        case '9':
        case 'u':
        case KEY_PPAGE:
            move_cursor(s, 1, -1);
            break;
        case 'e':
            if(is_hint(s)){
                hintLoc = 1;
            }
            else{
                insert_num(s);
            }
            break;
        case 'a':
            if(s->highlight == 0){
                s->highlight = 9;
            }
            else{
                s->highlight -= 1;
            }
            break;
        case 'd':
            s->highlight += 1;
            if(s->highlight > 9){
                s->highlight = 0;
            }
            break;
        default:
            break;
    }
    if(viewSol){
        clear();
        print_board();
        print_solution(s);

        attron(COLOR_PAIR(COLOR_CYAN));
        attron(A_BOLD);
        mvaddstr(6, 42, "Sudoku Solution");
        mvaddstr(8, 42, "Press any key to continue.");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(COLOR_CYAN));
        refresh();
        viewSol = 0;
        getch();
    }
    else if(hintLoc){
        attron(COLOR_PAIR(COLOR_CYAN));
        attron(A_BOLD);
        mvaddstr(6, 42, "A hint spot cannot be changed.");
        mvaddstr(8, 42, "Press any key to continue.");
        attroff(COLOR_PAIR(COLOR_CYAN));
        attroff(A_BOLD);
        refresh();
        hintLoc = 0;
        getch();
    }
    else if(incorrect){
        clear();
        print_board();
        print_hints(s);
        print_userIn(s, incorrect);

        attron(COLOR_PAIR(COLOR_CYAN));
        attron(A_BOLD);
        mvaddstr(6, 42, "Incorrect Answers Shown in Red");
        mvaddstr(8, 42, "Press any key to continue.");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(COLOR_CYAN));
        refresh();
        incorrect = 0;
        getch();
    }
    clear();
    print_board();
    print_hints(s);
    print_userIn(s, 0);
    print_specific_num(s);
    mvaddstr(13, 42, "Use the numpad or vim keys used in ");
    mvaddstr(14, 42, "RLG327 game to move around the board.");
    mvaddstr(16, 42, "Press 'e' to enter in a number 1-9.");
    mvaddstr(18, 42, "Press 'w' for more options.");
    mvaddstr(20, 42, "Press 'q' to quit.");
}

//Moves the cursor
void move_cursor(sudoku *s, uint32_t x, uint32_t y){
    if(s->cursor.x + x >= 0 && s->cursor.x + x <= 8 && s->cursor.y + y >= 0 && s->cursor.y + y <= 8){
        s->cursor.x += x;
        s->cursor.y += y;
    }
}

//Prints cursor on userIn square
void print_cursor_userIn(sudoku *s, uint32_t y, uint32_t x){
    attron(A_BOLD);
    attron(COLOR_PAIR(COLOR_NUM_LIGHT));
    mvaddch(y + s->cursor.y * 2, x - 1, ' ');
    mvaddch(y + s->cursor.y * 2, x + 1, ' ');
    if(s->userIn[s->cursor.y][s->cursor.x] != 0){
        mvprintw(y + s->cursor.y * 2, x, "%d", s->userIn[s->cursor.y][s->cursor.x]);
    }
    attroff(COLOR_PAIR(COLOR_NUM_LIGHT));
    attroff(A_BOLD);
}

//Prints cursor on hints square
void print_cursor_hints(sudoku *s, uint32_t y, uint32_t x, uint32_t num){
    attron(A_BOLD);
    attron(COLOR_PAIR(COLOR_HINT_LIGHT));
    mvaddch(y + s->cursor.y * 2, x - 1, ' ');
    mvaddch(y + s->cursor.y * 2, x + 1, ' ');
    if(s->hints[s->cursor.y][s->cursor.x] != 1){

        mvaddch(y + s->cursor.y * 2, x, ' ');
    }
    else{
        mvprintw(y + s->cursor.y * 2, x, "%d", num);
    }
    attroff(COLOR_PAIR(COLOR_HINT_LIGHT));
    attroff(A_BOLD);
}

//Checks for a win
int check_win(sudoku *s){
    uint32_t x, y;
    for(y = 0; y < 9; y++){
        for(x = 0; x < 9; x++){
            if(s->hints[y][x] == 1){
                continue;
            }
            else if(s->userIn[y][x] == 0){
                return 0;
            }
            else if(s->userIn[y][x] == s->solution[y][x]){
                continue;
            }
            else{
                return 0;
            }
        }
    }
    return 1;

}

//Prints board
void print_board(){ // For n curses
    //pair t for later
    //cout << board;
    //char symbol;

    clear();
    attron(COLOR_PAIR(COLOR_GREENGRAY));
    attron(A_BOLD);
    mvaddstr(1, 13, " SUDOKU FOR 327 ");
    attroff(A_BOLD);
    attroff(COLOR_PAIR(COLOR_GREENGRAY));
    uint32_t x, y, xshift, yshift;
    xshift = 2;
    yshift = 3;
    for(y = 0; y < 19; y++){
        for(x = 0; x < 37; x++){
            //symbol = gameboard[y][x];
            switch (gameboard[y][x])
            {
                case '@':
                    attron(COLOR_PAIR(COLOR_TITANIUMWHITE));
                    mvaddch(y + yshift, x + xshift, gameboard[y][x]);
                    attroff(COLOR_PAIR(COLOR_TITANIUMWHITE));
                    break;

                case '|':
                case '-':
                case '+':
                    attron(A_BOLD);
                    attron(COLOR_PAIR(COLOR_DARKGRAY));
                    mvaddch(y + yshift, x + xshift, gameboard[y][x]);
                    attroff(A_BOLD);
                    attroff(COLOR_PAIR(COLOR_DARKGRAY));
                    break;

                case ' ':
                    attron(COLOR_PAIR(COLOR_DARKGRAY));
                    mvaddch(y + yshift, x + xshift, gameboard[y][x]);
                    attroff(COLOR_PAIR(COLOR_DARKGRAY));
                    break;
                    
                default:
                    break;
            }
        }
    }
    refresh();
}

//Prints solution
void print_solution(sudoku *s){
    
    uint32_t x, y, xLoc, yLoc, xshift, yshift;//, key;
    xshift = 2;
    yshift = 3;
    yLoc = 0;
    xLoc = 0;
    attron(COLOR_PAIR(COLOR_DARKGRAY));
    for(y = 1; y < 19; y+=2){
        for(x = 2; x < 37; x+=4){
            switch (s->solution[yLoc][xLoc])
            {
                case 1:
                    mvaddch(y + yshift, x + xshift, '1');
                    break;
                case 2:
                    mvaddch(y + yshift, x + xshift, '2');
                    break;
                case 3:
                    mvaddch(y + yshift, x + xshift, '3');
                    break;
                case 4:
                    mvaddch(y + yshift, x + xshift, '4');
                    break;
                case 5:
                    mvaddch(y + yshift, x + xshift, '5');
                    break;
                case 6:
                    mvaddch(y + yshift, x + xshift, '6');
                    break;
                case 7:
                    mvaddch(y + yshift, x + xshift, '7');
                    break;
                case 8:
                    mvaddch(y + yshift, x + xshift, '8');
                    break;
                case 9:
                    mvaddch(y + yshift, x + xshift, '9');
                    break;
                default: //Prints 0 if something prints wrong
                    mvaddch(y + yshift, x + xshift, '0');
                    break;
            }
            xLoc++;
        }
        //yLoc++;
    }
    attroff(COLOR_PAIR(COLOR_DARKGRAY));
    refresh();
}

//Prints hints
void print_hints(sudoku *s){
    
    uint32_t x, y, xLoc, yLoc, xshift, yshift, yCur, xCur;
    xshift = 2;
    yshift = 3;
    yLoc = 0;
    xLoc = 0;
    attron(COLOR_PAIR(COLOR_GREENGRAY));
    attron(A_BOLD);
    for(y = 1; y < 19; y+=2){
        for(x = 2; x < 37; x+=4){
            if(s->hints[0][xLoc] == 1){
                mvaddch(y + yshift, x + xshift - 1, ' ');
                mvaddch(y + yshift, x + xshift, ' ');
                mvaddch(y + yshift, x + xshift + 1, ' ');

                switch (s->solution[yLoc][xLoc])
                {
                    case 1:
                        mvaddch(y + yshift, x + xshift, '1');
                        break;
                    case 2:
                        mvaddch(y + yshift, x + xshift, '2');
                        break;
                    case 3:
                        mvaddch(y + yshift, x + xshift, '3');
                        break;
                    case 4:
                        mvaddch(y + yshift, x + xshift, '4');
                        break;
                    case 5:
                        mvaddch(y + yshift, x + xshift, '5');
                        break;
                    case 6:
                        mvaddch(y + yshift, x + xshift, '6');
                        break;
                    case 7:
                        mvaddch(y + yshift, x + xshift, '7');
                        break;
                    case 8:
                        mvaddch(y + yshift, x + xshift, '8');
                        break;
                    case 9:
                        mvaddch(y + yshift, x + xshift, '9');
                        break;
                    default: //Prints 0 if something prints wrong
                        mvaddch(y + yshift, x + xshift, '0');
                        break;
                } 
            }
            if(xLoc == s->cursor.x){
                    yCur = y + yshift;
                    xCur = x + xshift;

            }
            xLoc++;
        }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(COLOR_GREENGRAY));
    
    print_cursor_hints(s, yCur, xCur, s->solution[s->cursor.y][s->cursor.x]);
    
    refresh();
}

//Prints user input
void print_userIn(sudoku *s, uint32_t incorrect){
    uint32_t x, y, xLoc, yLoc, xshift, yshift, yCur, xCur;
    xshift = 2;
    yshift = 3;
    yLoc = 0;
    xLoc = 0;
    
    for(y = 1; y < 19; y+=2){
        for(x = 2; x < 37; x+=4){
            attron(COLOR_PAIR(COLOR_DARKGRAY));
            if(s->userIn[0][xLoc] != 0){
                if(s->userIn[0][xLoc] != s->solution[0][xLoc] && incorrect == 1){
                    attron(COLOR_PAIR(COLOR_RED_LIGHT));
                } 
                mvaddch(y + yshift, x + xshift - 1, ' ');
                mvaddch(y + yshift, x + xshift, ' ');
                mvaddch(y + yshift, x + xshift + 1, ' ');

                switch (s->userIn[yLoc][xLoc])
                {
                    case 1:
                        mvaddch(y + yshift, x + xshift, '1');
                        break;
                    case 2:
                        mvaddch(y + yshift, x + xshift, '2');
                        break;
                    case 3:
                        mvaddch(y + yshift, x + xshift, '3');
                        break;
                    case 4:
                        mvaddch(y + yshift, x + xshift, '4');
                        break;
                    case 5:
                        mvaddch(y + yshift, x + xshift, '5');
                        break;
                    case 6:
                        mvaddch(y + yshift, x + xshift, '6');
                        break;
                    case 7:
                        mvaddch(y + yshift, x + xshift, '7');
                        break;
                    case 8:
                        mvaddch(y + yshift, x + xshift, '8');
                        break;
                    case 9:
                        mvaddch(y + yshift, x + xshift, '9');
                        break;
                    default: //Prints 0 if something prints wrong
                        mvaddch(y + yshift, x + xshift, '0');
                        break;
                }

                if(s->userIn[0][xLoc] != s->solution[0][xLoc] && incorrect == 1){
                    attroff(COLOR_PAIR(COLOR_RED_LIGHT));
                } 
                attroff(COLOR_PAIR(COLOR_DARKGRAY));
            }
            if(xLoc == s->cursor.x){
                    yCur = y + yshift;
                    xCur = x + xshift;

            }
            xLoc++;
        }
    }
    
    if(incorrect != 1){
        print_cursor_userIn(s, yCur, xCur);
    }
    
    refresh();
}

//Prints num to be highlighted
void print_specific_num(sudoku *s){
    uint32_t x, y, xLoc, yLoc, xshift, yshift, xCur, yCur;
    xshift = 2;
    yshift = 3;
    yLoc = 0;
    xLoc = 0;
    
    mvaddstr(10, 42, "Use 'a' and 'd' to highlight numbers.");
    mvaddstr(11, 42, "NONE  1  2  3  4  5  6  7  8  9 ");
    attron(COLOR_PAIR(COLOR_BLUE_LIGHT));
    attron(A_BOLD);
    switch (s->highlight){
        case 1:
            mvaddstr(11, 47, " 1 ");
            break;
        case 2:
            mvaddstr(11, 50, " 2 ");
            break;
        case 3:
            mvaddstr(11, 53, " 3 ");
            break;
        case 4:
            mvaddstr(11, 56, " 4 ");
            break;
        case 5:
            mvaddstr(11, 59, " 5 ");
            break;
        case 6:
            mvaddstr(11, 62, " 6 ");
            break;
        case 7:
            mvaddstr(11, 65, " 7 ");
            break;
        case 8:
            mvaddstr(11, 68, " 8 ");
            break;
        case 9:
            mvaddstr(11, 71, " 9 ");
            break;
        default:
            mvaddstr(11, 42, "NONE");
            break;
    }
    for(y = 1; y < 19; y+=2){
        for(x = 2; x < 37; x+=4){
            if(s->userIn[0][xLoc] == s->highlight && s->userIn[0][xLoc] != 0){
                mvaddch(y + yshift, x + xshift - 1, ' ');
                mvaddch(y + yshift, x + xshift, ' ');
                mvaddch(y + yshift, x + xshift + 1, ' ');

                switch (s->userIn[yLoc][xLoc])
                {
                    case 1:
                        mvaddch(y + yshift, x + xshift, '1');
                        break;
                    case 2:
                        mvaddch(y + yshift, x + xshift, '2');
                        break;
                    case 3:
                        mvaddch(y + yshift, x + xshift, '3');
                        break;
                    case 4:
                        mvaddch(y + yshift, x + xshift, '4');
                        break;
                    case 5:
                        mvaddch(y + yshift, x + xshift, '5');
                        break;
                    case 6:
                        mvaddch(y + yshift, x + xshift, '6');
                        break;
                    case 7:
                        mvaddch(y + yshift, x + xshift, '7');
                        break;
                    case 8:
                        mvaddch(y + yshift, x + xshift, '8');
                        break;
                    case 9:
                        mvaddch(y + yshift, x + xshift, '9');
                        break;
                    default: //Prints 0 if something prints wrong
                        mvaddch(y + yshift, x + xshift, '0');
                        break;
                }
            }
            else if(s->hints[0][xLoc] == 1 && s->solution[0][xLoc] == s->highlight){
                mvaddch(y + yshift, x + xshift - 1, ' ');
                mvaddch(y + yshift, x + xshift, ' ');
                mvaddch(y + yshift, x + xshift + 1, ' ');

                switch (s->solution[yLoc][xLoc])
                {
                    case 1:
                        mvaddch(y + yshift, x + xshift, '1');
                        break;
                    case 2:
                        mvaddch(y + yshift, x + xshift, '2');
                        break;
                    case 3:
                        mvaddch(y + yshift, x + xshift, '3');
                        break;
                    case 4:
                        mvaddch(y + yshift, x + xshift, '4');
                        break;
                    case 5:
                        mvaddch(y + yshift, x + xshift, '5');
                        break;
                    case 6:
                        mvaddch(y + yshift, x + xshift, '6');
                        break;
                    case 7:
                        mvaddch(y + yshift, x + xshift, '7');
                        break;
                    case 8:
                        mvaddch(y + yshift, x + xshift, '8');
                        break;
                    case 9:
                        mvaddch(y + yshift, x + xshift, '9');
                        break;
                    default: //Prints 0 if something prints wrong
                        mvaddch(y + yshift, x + xshift, '0');
                        break;
                }
            }
            if(xLoc == s->cursor.x){
                yCur = y + yshift;
                xCur = x + xshift;
            }
            xLoc++;
        }
    }
    attroff(COLOR_PAIR(COLOR_BLUE_LIGHT));
    attroff(A_BOLD);
    
    if(s->hints[s->cursor.y][s->cursor.x] == 1){
        print_cursor_hints(s, yCur, xCur, s->solution[s->cursor.y][s->cursor.x]);
    }
    else{
        print_cursor_userIn(s, yCur, xCur);
    }
    
    
    refresh();
}

//Initiates window
void init_game(sudoku *s){
    //Creates the Puzzle Right Here
    create_solution(s);
    //Creates the window
    init_terminal();
    //Chooses how many Hints to Give (17-80) Min 17, Max 80
    print_board();
    game_difficulty(s);
    create_hints(s);
}

//Checks if the number is valid
bool isValid(uint32_t x, uint32_t y, sudoku *s, uint32_t rNum){
  // Check whether rNum is valid at the x row
    uint32_t column, row;
    for (column = 0; column < 9; column++){
        if (column != y && s->solution[column][x] == rNum){
            return false;
        }
    }

    // Check whether rNum is valid at the y column
    for (row = 0; row < 9; row++){
        if (row != x && s->solution[y][row] == rNum){
            return false;
        }
    }

    // Check whether rNum is valid in the 3 by 3 box
    for (row = (x / 3) * 3; row < (x / 3) * 3 + 3; row++){
        for (column = (y / 3) * 3; column < (y / 3) * 3 + 3; column++){
            if (row != x && column != y && s->solution[column][row] == rNum){
                return false;
            }
        }
    }

  return true; //rNum is valid
}

//Creates the puzzle
void create_solution(sudoku *s){

    uint32_t x, y, rNum, i, col;
    for(y = 0; y < 9; y++){
        for(x = 0; x < 9; x++){
            s->solution[y][x] = 0;
            s->userIn[y][x] = 0;
        }
    }

    col = 0;
    for(y = 0; y < 9; y++){
        for(x = 0; x < 9; x++){
            rNum = rand() % 9 + 1;
            if(isValid(x, y, s, rNum)){
                s->solution[y][x] = rNum;
            }
            else{
                for(i = 1; i < 10; i++)
                {
                    if(isValid(x, y, s, i)){
                        s->solution[y][x] = i;
                        break; 
                    }
                }
            }
        }
        for(i = 0; i < 9; i++){
            if(s->solution[y][i] == 0){y--; col++; break;}
        }
        if(col > 1){break;}
    }
    if(col > 1){
        create_solution(s);
    }
}

//Creates the hints
void create_hints(sudoku *s){
    uint32_t x, y, i;
    for(y = 0; y < 9; y++){
        for(x = 0; x < 9; x++){
            s->hints[y][x] = 0;
        }
    }

    for(i = 0; i < s->numHints; i++){
        while(TRUE){
            y = rand() % 9;
            x = rand() % 9;
            if(s->hints[y][x] == 0){
                s->hints[y][x] = 1;
                break;
            }
        }
    }
}

//Choose the game difficulty
void game_difficulty(sudoku *s){
    uint32_t x, y, c, cust, key, exit, hints;
    attron(A_BOLD);
    mvaddstr(5, 42, "CHOOSE YOUR DIFFICULTY");
    
    x = 45;
    y = 7;
    c = 0;
    hints = 17;
    exit = 0;
    cust = 0;
    attron(A_ITALIC);
    mvaddstr(7, 48, "EASY");
    attroff(A_ITALIC);
    mvaddch(y, x, '*');
    attroff(A_BOLD);
    mvaddstr(7, 61, "45 Hints");
    mvaddstr(9, 48, "MEDIUM");
    mvaddstr(9, 61, "35 Hints");
    mvaddstr(11, 48, "HARD");
    mvaddstr(11, 61, "25 Hints");
    mvaddstr(13, 48, "FORMIDABLE");
    mvaddstr(13, 61, "17 Hints");
    mvaddstr(15, 48, "CUSTOM");
    mvaddstr(15, 61, "17-80 Hints");

    while(TRUE){
        mvaddstr(20, 42, "Use 'w' and 's' to select,");
        mvaddstr(21, 42, "'e' to execute.");

        key = getch();
        mvaddch(y, x, ' ');
        switch (key){
            case 'w':
            case KEY_UP:
                c--;
                y-=2;
                break;
            case 's':
            case KEY_DOWN:
                c++;
                y+=2;
                break;
            case 'e':
            case KEY_ENTER:
                if(c == 4){
                    cust = 1;
                }
                else if(c == 3){
                    s->numHints = 17;
                    exit = 1;
                }
                else if(c == 2){
                    s->numHints = 25;
                    exit = 1;
                }
                else if(c == 1){
                    s->numHints = 35;
                    exit = 1;
                }
                else if(c == 0){
                    s->numHints = 40;
                    exit = 1;
                }
                break;
        }
        if(exit == 1){break;}
        else if(cust == 0){
            
            
            attron(A_ITALIC);
            attron(A_BOLD);
            if(y < 7){c = 4; y = 15;}
            else if(c > 4){c = 0; y = 7;}
            if (c == 0){
                mvaddstr(7, 48, "EASY");
                attroff(A_BOLD);
                attroff(A_ITALIC);
                mvaddstr(9, 48, "MEDIUM");
                mvaddstr(11, 48, "HARD");
                mvaddstr(13, 48, "FORMIDABLE");
                mvaddstr(15, 48, "CUSTOM");
            }
            else if(c == 1){
                mvaddstr(9, 48, "MEDIUM");
                attroff(A_BOLD);
                attroff(A_ITALIC);
                mvaddstr(7, 48, "EASY");
                mvaddstr(11, 48, "HARD");
                mvaddstr(13, 48, "FORMIDABLE");
                mvaddstr(15, 48, "CUSTOM");
            }
            else if(c == 2){
                mvaddstr(11, 48, "HARD");
                attroff(A_BOLD);
                attroff(A_ITALIC);
                mvaddstr(7, 48, "EASY");
                mvaddstr(9, 48, "MEDIUM");
                mvaddstr(13, 48, "FORMIDABLE");
                mvaddstr(15, 48, "CUSTOM");
            }
            else if(c == 3){
                mvaddstr(13, 48, "FORMIDABLE");
                attroff(A_BOLD);
                attroff(A_ITALIC);
                mvaddstr(7, 48, "EASY");
                mvaddstr(9, 48, "MEDIUM");
                mvaddstr(11, 48, "HARD");
                mvaddstr(15, 48, "CUSTOM");
            }
            else if(c == 4){
                mvaddstr(15, 48, "CUSTOM");
                attroff(A_BOLD);
                attroff(A_ITALIC);
                mvaddstr(7, 48, "EASY");
                mvaddstr(9, 48, "MEDIUM");
                mvaddstr(11, 48, "HARD");
                mvaddstr(13, 48, "FORMIDABLE");
            }
            attron(A_BOLD);
            mvaddch(y, x, '*');
            attroff(A_BOLD);
        }
        else{
            uint32_t done;
            done = 0;
            mvaddstr(16, 50, "|||");
            mvaddstr(17, 50, "VVV");
            mvprintw(18, 50, "%2d Hints", hints);
            mvaddstr(20, 42, "Use 'w' for more, 's' for less,");
            mvaddstr(21, 42, "'e' to execute, ESC key to ");
            mvaddstr(22, 42, "return to difficulty selection.");
            while(TRUE){
                key = getch();
                switch (key){
                    case 'w':
                    case KEY_UP:
                        hints++;
                        if(hints > 80){
                            hints = 17;
                        }
                        mvprintw(18, 50, "%2d Hints", hints);
                        break;
                    case 's':
                    case KEY_DOWN:
                        hints--;
                        if(hints < 17){
                            hints = 80;
                        }
                        mvprintw(18, 50, "%2d Hints", hints);
                        break;
                    case 'e':
                        s->numHints = hints;
                        done = 1;
                        break;
                    case 27:
                        attron(A_BOLD);
                        mvaddch(y, x, '*');
                        attroff(A_BOLD);
                        cust = 0;
                        mvaddstr(16, 50, "   ");
                        mvaddstr(17, 50, "   ");
                        mvaddstr(18, 50, "        ");
                        mvaddstr(20, 42, "                               ");
                        mvaddstr(21, 42, "                                ");
                        mvaddstr(22, 42, "                               ");
                        break;
                    default:
                        break;
                        
                }
                if(cust == 0){
                    break;
                }
                if(done == 1){
                clear();
                break;
            }
            }
            if(done == 1){
                break;
            }
        }
        
        
    }
    attroff(A_BOLD);
    attroff(A_ITALIC);

}