#ifndef SUDOKU_H
# define SUDOKU_H


#define BOARD_X 37
#define BOARD_Y 19

enum colors{ //OTHER COLORS BEFORE 8 ARE ALREADY DEFINED
    COLOR_TITANIUMWHITE = 8,
    COLOR_DARKGRAY,
    COLOR_GREENGRAY,
    COLOR_HINT_LIGHT,
    COLOR_NUM_LIGHT,
    COLOR_RED_LIGHT,
    COLOR_BLUE_LIGHT
};

typedef struct location{
    uint32_t x;
    uint32_t y;
}location_t;

//class 
class sudoku {
    public:
    uint32_t solution[9][9];
    uint32_t hints[9][9];
    uint32_t userIn[9][9];
    uint32_t numHints;
    uint32_t quit;
    uint32_t win;
    location_t cursor;
    uint32_t highlight;
    uint32_t skip;
};
//void print_board();
int check_win(sudoku *s);
void game_input(sudoku *s);
void print_solution(sudoku *s);
void print_hints(sudoku *s);
void print_board();
void init_terminal();
void init_game(sudoku *s);
bool isValid(uint32_t x, uint32_t y, sudoku *s, uint32_t rNum);
void create_solution(sudoku *s);
void create_hints(sudoku *s);
void game_difficulty(sudoku *s);
void move_cursor(sudoku *s, uint32_t x, uint32_t y);
void print_userIn(sudoku *s, uint32_t incorrect);
void print_specific_num(sudoku *s);
void add_hint(sudoku *s);

#endif