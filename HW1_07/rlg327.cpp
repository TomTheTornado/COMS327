#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>

#include "headers/dungeon.h"
#include "headers/pc.h"
#include "headers/npc.h"
#include "headers/move.h"
#include "headers/utils.h"
#include "headers/io.h"
#include "headers/parse.h"

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

const char *ganon =
  "\n"
  "                                GAME OVER\n"
  "                             RETURN OF GANON\n"
  "                      (&,                     ,/&)\n"
  "                       #@@@@*.             *&@@@)\n"
  "                        *@@@@@.,&&@@@@@%*.%@@@&\n"
  "                          .*&@@@@@@@@&(&&@@&#.\n"
  "                             .(%#   %&&   @@)*\n"
  "                  ,,**&&%%/%%@@@@%%@//\\&@@@@&&)/(&&/&,\n"   
  "               *&@@@@@#@&/@@@@@@@@//'@@'\\@@@@@@@)@#&@@@@&*\n"
  "           .,*#@@@@@@@@@(@@@@@@@@//'@@@@'\\@@@@@@@)@@@@@@@@\\.\n"
  "          *@@@@@@@@@@@@@(@@@@@@@@@@@@@@@@@@@@@@@/@@@@@@@@@@@\\,\n"
  "         (@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\,\n"
  "        /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\\n"
  "       /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@).\n"
  "      /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\,\n"
  "    *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\\,\n"
  "   (@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@),\n"
  "  /@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@),\n"
  " /@@@@@@@@@@@@/@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \\@@@@@@@@@@).\n"
  ",)@@@@@@@@@@/' @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ \\@@@@@@@@@@(\n"  
  "               You're dead.  Better luck in the next life.\n";

  const char *quitRLG =
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

  const char *batman = 
  "\n"
	"          .  .                 BATMAN SAYS:\n"
	"          |\\_|\\\n"
	"          | a_a\\    ~DONT PLAY THE RLG327 GAME WITH\n"
	"          | | \"]        NO MONSTERS, IT DEFEATS\n"
	"      ____| \'-\\___          THE PURPOSE!\n"
	"     /.----.___.-\'\\\n"
	"    //        _    \\\n"
	"   //   .-. (~v~) /|\n"
	"  |\'|  /\\:  .--  / \\\n"
	" // |-/  \\_/____/\\/~|\n"
	"|/  \\ |  []_|_|_] \\ |\n"
	"| \\  | \\ |___   _\\ ]_}\n"
	"| |  \'-\' /   \'.\'  |\n"
	"| |     /    /|:  | \n"
	"| |     |   / |:  /\\\n"
	"| |     /  /  |  /  \\\n"
	"| |    |  /  /  |    \\\n"
	"\\ |    |/\\/  |/|/\\    \\\n"
	" \\|\\ |\\|  |  | / /\\/\\__\\\n"
	"  \\ \\| | /   | |__\n"
	"       / |   |____)\n"
	"       |_/\n";

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "          [-s|--save [<file>]] [-i|--image <pgm file>]\n"
          "          [-n|--nummon <count>] [-p|--parse <0 or 1>\n",
          name);

  exit(-1);
}

int main(int argc, char *argv[])
{
  dungeon d;
  time_t seed;
  struct timeval tv;
  int32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_save_seed, do_save_image;
  uint32_t long_arg;
  char *save_file;
  char *load_file;
  char *pgm_file;
  uint32_t parseMon = 0;
  uint32_t parseItem = 0;
  uint32_t parseFail = 0;
  
  /* Quiet a false positive from valgrind. */
  memset(&d, 0, sizeof (d));
  
  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_save_seed = do_save_image = 0;
  do_seed = 1;
  save_file = load_file = NULL;
  d.max_monsters = MAX_MONSTERS;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
    * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */
 
 if (argc > 1) {
    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
      if (argv[i][0] == '-') { /* All switches start with a dash */
        if (argv[i][1] == '-') {
          argv[i]++;    /* Make the argument have a single dash so we can */
          long_arg = 1; /* handle long and short args at the same place.  */
        }
        switch (argv[i][1]) {
        case 'n':  //NUM MONSTERS
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-nummon")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%hu", &d.max_monsters)) {
            usage(argv[0]);
          }
          break;
        case 'r':  //SEED
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-rand")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
            usage(argv[0]);
          }
          do_seed = 0;
          break;
        case 'l':  //LOAD
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-load"))) {
            usage(argv[0]);
          }
          do_load = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            load_file = argv[++i];
          }
          break;
        case 'p': //PARSE MONSTERS OR ITEMS
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-parse")) ||
              argc < ++i + 1 /* No more arguments */ ||
              !sscanf(argv[i], "%hu", &d.parseThing)) {
            usage(argv[0]);
          }
          d.max_monsters = 0;
          if(d.parseThing == 0){
            parseMon = 1;
          }
          else if(d.parseThing == 1){
            parseItem = 1;
          }
          else{
            parseFail = 1;
          }
          break;
        case 's':  //SAVE
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-save"))) {
            usage(argv[0]);
          }
          do_save = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll save to it.  If it is "seed", we'll save to    *
	     * <the current seed>.rlg327.  If it is "image", we'll  *
	     * save to <the current image>.rlg327.                  */
	    if (!strcmp(argv[++i], "seed")) {
	      do_save_seed = 1;
	      do_save_image = 0;
	    } else if (!strcmp(argv[i], "image")) {
	      do_save_image = 1;
	      do_save_seed = 0;
	    } else {
	      save_file = argv[i];
	    }
          }
          break;
        case 'i':
          if ((!long_arg && argv[i][2]) ||
              (long_arg && strcmp(argv[i], "-image"))) {
            usage(argv[0]);
          }
          do_image = 1;
          if ((argc > i + 1) && argv[i + 1][0] != '-') {
            /* There is another argument, and it's not a switch, so *
             * we'll treat it as a save file and try to load it.    */
            pgm_file = argv[++i];
          }
          break;
        default:
          usage(argv[0]);
        }
      } else { /* No dash */
        usage(argv[0]);
      }
    }
  }

  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  srand(seed);

  io_init_terminal();
  init_dungeon(&d);

  if (do_load) {
    read_dungeon(&d, load_file);
  } else if (do_image) {
    read_pgm(&d, pgm_file);
  } else {
    gen_dungeon(&d);
  }


  /* Ignoring PC position in saved dungeons.  Not a bug. */
  config_pc(&d);
  gen_monsters(&d);

  io_display(&d);
  if (!do_load && !do_image) {
    io_queue_message("Seed is %u.", seed);
  }
  while (pc_is_alive(&d) && dungeon_has_npcs(&d) && !d.quit) {
    do_moves(&d);
  }
  io_display(&d);

  io_reset_terminal();

  if (do_save) {
    if (do_save_seed) {
       /* 10 bytes for number, plus dot, extention and null terminator. */
      save_file = (char *) malloc(18);
      sprintf(save_file, "%ld.rlg327", seed);
    }
    if (do_save_image) {
      if (!pgm_file) {
	fprintf(stderr, "No image file was loaded.  Using default.\n");
	do_save_image = 0;
      } else {
	/* Extension of 3 characters longer than image extension + null. */
	save_file = (char *) malloc(strlen(pgm_file) + 4);
	strcpy(save_file, pgm_file);
	strcpy(strchr(save_file, '.') + 1, "rlg327");
      }
    }
    write_dungeon(&d, save_file);

    if (do_save_seed || do_save_image) {
      free(save_file);
    }
  }

  if(d.PC->kills[kill_direct] == 1 && d.max_monsters == 1){
    printf("%s", triumph);
    printf("You defended your life in the face of %u deadly beasts.\n"
         "You avenged the cruel and untimely murders of %u "
         "peaceful dungeon residents.\n",
         d.PC->kills[kill_direct], d.PC->kills[kill_avenged]);
  }
  else if(parseFail == 1){
    printf("%s", "Sorry, I didn't understand what you wanted to parse.\n");
  }
  else if(d.max_monsters == 0 && parseMon == 0 && parseItem == 0){
    printf("%s", batman);
  }
  else if(parseMon == 1){
    read_monsters(&d);
  }
  else if(parseItem == 1){
    read_objects(&d);
  }
  else if(pc_is_alive(&d) && (&d.num_monsters > 0)){
    printf("%s", quitRLG);
  }
  else{
    printf("%s", pc_is_alive(&d) ? triumph : ganon);
    printf("You defended your life in the face of %u deadly beasts.\n"
         "You avenged the cruel and untimely murders of %u "
         "peaceful dungeon residents.\n",
         d.PC->kills[kill_direct], d.PC->kills[kill_avenged]);

  }


  if (pc_is_alive(&d)) {
    /* If the PC is dead, it's in the move heap and will get automatically *
     * deleted when the heap destructs.  In that case, we can't call       *
     * delete_pc(), because it will lead to a double delete.               */
    character_delete(d.PC);
  }

  delete_dungeon(&d);

  return 0;
}
