#ifndef COLOR_H
#define COLOR_H

/**
 * Ansii escape codes
 * */

//colors
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_BLACK   "\x1b[30m"

//backgrounds
#define BG_BLACK      "\033[48;5;m"
#define BG_DARK       "\033[48;2;15;15;15m"
#define BG_DARK_GREY  "\033[48;2;40;40;40m"
#define BG_GREY       "\033[48;2;60;60;60m"
#define BG_GROUND     "\033[48;2;75;75;75m"
#define BG_BROWN      "\033[48;2;75;75;75m"
#define BG_RED        "\033[48;2;255;0;0m"
#define BG_BLUE       "\033[48;2;0;0;255m"
#define BG_GREEN      "\033[48;2;0;255;0m"
#define BG_YELLOW     "\033[48;2;255;255;0m"

//reset console
#define A_C_R   "\x1b[0m"

#endif