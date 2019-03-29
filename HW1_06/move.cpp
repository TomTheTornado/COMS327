#include "headers/move.h"

#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <cstdbool>

#include "headers/dungeon.h"
#include "headers/heap.h"
#include "headers/move.h"
#include "headers/npc.h"
#include "headers/pc.h"
#include "headers/character.h"
#include "headers/utils.h"
#include "headers/path.h"
#include "headers/event.h"
#include "headers/io.h"

void do_combat(dungeon_t *d, character_t *atk, character_t *def)
{
  int can_see_atk, can_see_def;
  char *organs[] = {
    (char*)"liver",                   /*  0 */
    (char*)"pancreas",                /*  1 */
    (char*)"heart",                   /*  2 */
    (char*)"eye",                     /*  3 */
    (char*)"arm",                     /*  4 */
    (char*)"leg",                     /*  5 */
    (char*)"intestines",              /*  6 */
    (char*)"gall bladder",            /*  7 */
    (char*)"lungs",                   /*  8 */
    (char*)"hand",                    /*  9 */
    (char*)"foot",                    /* 10 */
    (char*)"spinal cord",             /* 11 */
    (char*)"pituitary gland",         /* 12 */
    (char*)"thyroid",                 /* 13 */
    (char*)"tongue",                  /* 14 */
    (char*)"bladder",                 /* 15 */
    (char*)"diaphram",                /* 16 */
    (char*)"stomach",                 /* 17 */
    (char*)"pharynx",                 /* 18 */
    (char*)"esophagus",               /* 19 */
    (char*)"trachea",                 /* 20 */
    (char*)"urethra",                 /* 21 */
    (char*)"spleen",                  /* 22 */
    (char*)"ganglia",                 /* 23 */
    (char*)"ear",                     /* 24 */
    (char*)"subcutaneous tissue",     /* 25 */
    (char*)"cerebellum",              /* 26 */ /* Brain parts begin here */
    (char*)"hippocampus",             /* 27 */
    (char*)"frontal lobe",            /* 28 */
    (char*)"brain"                    /* 29 */
  };
  int part;

  if (def->alive) {
    def->alive = 0;
    charpair(def->position) = NULL;
    
    if (def != &d->pc) {
      d->num_monsters--;
    } else {
      if ((part = rand() % (sizeof (organs) / sizeof (organs[0]))) < 26) {
        io_queue_message("As the %c eats your %s, "
                         "you wonder if there is an afterlife.",
                         atk->symbol, organs[part]);
      } else {
        io_queue_message("Your last thoughts fade away as "
                         "the %c eats your %s...",
                         atk->symbol, organs[part]);
      }
      /* Queue an empty message, otherwise the game will not pause for *
       * player to see above.                                          */
      io_queue_message("");
    }
    atk->kills[kill_direct]++;
    atk->kills[kill_avenged] += (def->kills[kill_direct] +
                                  def->kills[kill_avenged]);
  }

  if (atk == &d->pc) {
    io_queue_message("You smite the %c!", def->symbol);
  }

  can_see_atk = can_see(d, &d->pc, atk);
  can_see_def = can_see(d, &d->pc, def);

  if (atk != &d->pc && def != &d->pc) {
    if (can_see_atk && !can_see_def) {
      io_queue_message("The %c callously murders some poor, "
                       "defenseless creature.", atk->symbol);
    }
    if (can_see_def && !can_see_atk) {
      io_queue_message("Something kills the helpless %c.", def->symbol);
    }
    if (can_see_atk && can_see_def) {
      io_queue_message("You watch in abject horror as the %c "
                       "gruesomely murders the %c!", atk->symbol, def->symbol);
    }
  }
}

void move_character(dungeon_t *d, character_t *c, pair_t next)
{
  if (charpair(next) &&
      ((next[dim_y] != c->position[dim_y]) ||
       (next[dim_x] != c->position[dim_x]))) {
    do_combat(d, c, charpair(next));
  } else {
    /* No character in new position. */

    d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
    c->position[dim_y] = next[dim_y];
    c->position[dim_x] = next[dim_x];
    d->character[c->position[dim_y]][c->position[dim_x]] = c;
  }
}

void fogTerrain(dungeon_t *d){
  bool UpR, Up, UpL, R, L, DownR, Down, DownL;
  UpR = false;
  Up = false;
  UpL = false;
  R = false;
  L = false;
  DownR = false;
  Down = false;
  DownL = false;

  d->pc_terrain[d->pc.position[dim_y]][d->pc.position[dim_x]] = 1;

  if(d->hardness[d->pc.position[dim_y] - 1][d->pc.position[dim_x] - 1] == 0){
    d->pc_terrain[d->pc.position[dim_y] - 1][d->pc.position[dim_x] - 1] = 1;
    UpL = true;
  }
  if(d->hardness[d->pc.position[dim_y] - 1][d->pc.position[dim_x]] == 0){
    d->pc_terrain[d->pc.position[dim_y] - 1][d->pc.position[dim_x]] = 1;
    Up = true;
  }
  if(d->hardness[d->pc.position[dim_y] - 1][d->pc.position[dim_x] + 1] == 0){
    d->pc_terrain[d->pc.position[dim_y] - 1][d->pc.position[dim_x] + 1] = 1;
    UpR = true;
  }
  if(d->hardness[d->pc.position[dim_y]][d->pc.position[dim_x] - 1] == 0){
    d->pc_terrain[d->pc.position[dim_y]][d->pc.position[dim_x] - 1] = 1;
    L = true;
  }
  if(d->hardness[d->pc.position[dim_y]][d->pc.position[dim_x] + 1] == 0){
    d->pc_terrain[d->pc.position[dim_y]][d->pc.position[dim_x] + 1] = 1;
    R = true;
  }
  if(d->hardness[d->pc.position[dim_y] + 1][d->pc.position[dim_x] - 1] == 0){
    d->pc_terrain[d->pc.position[dim_y] + 1][d->pc.position[dim_x] - 1] = 1;
    DownL = true;
  }
  if(d->hardness[d->pc.position[dim_y] + 1][d->pc.position[dim_x]] == 0){
    d->pc_terrain[d->pc.position[dim_y] + 1][d->pc.position[dim_x]] = 1;
    Down = true;
  }
  if(d->hardness[d->pc.position[dim_y] + 1][d->pc.position[dim_x] + 1] == 0){
    d->pc_terrain[d->pc.position[dim_y] + 1][d->pc.position[dim_x] + 1] = 1;
    DownR = true;
  }
  //Outer ring around

  //Top ring
  if(d->hardness[d->pc.position[dim_y] - 2][d->pc.position[dim_x] - 2] == 0 && UpL){
    d->pc_terrain[d->pc.position[dim_y] - 2][d->pc.position[dim_x] - 2] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] - 2][d->pc.position[dim_x] - 1] == 0 && UpL && Up){
    d->pc_terrain[d->pc.position[dim_y] - 2][d->pc.position[dim_x] - 1] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] - 2][d->pc.position[dim_x]] == 0 && Up){
    d->pc_terrain[d->pc.position[dim_y] - 2][d->pc.position[dim_x]] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] - 2][d->pc.position[dim_x] + 1] == 0 && UpR && Up){
    d->pc_terrain[d->pc.position[dim_y] - 2][d->pc.position[dim_x] + 1] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] - 2][d->pc.position[dim_x] + 2] == 0 && UpR){
    d->pc_terrain[d->pc.position[dim_y] - 2][d->pc.position[dim_x] + 2] = 1;
  }
  //Left
  if(d->hardness[d->pc.position[dim_y] - 1][d->pc.position[dim_x] - 2] == 0 && UpL && L){
    d->pc_terrain[d->pc.position[dim_y] - 1][d->pc.position[dim_x] - 2] = 1;
  }
  if(d->hardness[d->pc.position[dim_y]][d->pc.position[dim_x] - 2] == 0 && L){
    d->pc_terrain[d->pc.position[dim_y]][d->pc.position[dim_x] - 2] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] + 1][d->pc.position[dim_x] - 2] == 0 && DownL && L){
    d->pc_terrain[d->pc.position[dim_y] + 1][d->pc.position[dim_x] - 2] = 1;
  }
  //RIght
  if(d->hardness[d->pc.position[dim_y] - 1][d->pc.position[dim_x] + 2] == 0 && UpR && R){
    d->pc_terrain[d->pc.position[dim_y] - 1][d->pc.position[dim_x] + 2] = 1;
  }
  if(d->hardness[d->pc.position[dim_y]][d->pc.position[dim_x] + 2] == 0 && R){
    d->pc_terrain[d->pc.position[dim_y]][d->pc.position[dim_x] + 2] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] + 1][d->pc.position[dim_x] + 2] == 0 && DownR && R){
    d->pc_terrain[d->pc.position[dim_y] + 1][d->pc.position[dim_x] + 2] = 1;
  }
  //Down
  if(d->hardness[d->pc.position[dim_y] + 2][d->pc.position[dim_x] - 2] == 0 && DownL){
    d->pc_terrain[d->pc.position[dim_y] + 2][d->pc.position[dim_x] - 2] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] + 2][d->pc.position[dim_x] - 1] == 0 && DownL && Down){
    d->pc_terrain[d->pc.position[dim_y] + 2][d->pc.position[dim_x] - 1] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] + 2][d->pc.position[dim_x]] == 0 && Down){
    d->pc_terrain[d->pc.position[dim_y] + 2][d->pc.position[dim_x]] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] + 2][d->pc.position[dim_x] + 1] == 0 && DownR && Down){
    d->pc_terrain[d->pc.position[dim_y] + 2][d->pc.position[dim_x] + 1] = 1;
  }
  if(d->hardness[d->pc.position[dim_y] + 2][d->pc.position[dim_x] + 2] == 0 && DownR){
    d->pc_terrain[d->pc.position[dim_y] + 2][d->pc.position[dim_x] + 2] = 1;
  }
}

void do_moves(dungeon_t *d)
{
  pair_t next;
  character_t *c;
  event_t *e;

  /* Remove the PC when it is PC turn.  Replace on next call.  This allows *
   * use to completely uninit the heap when generating a new level without *
   * worrying about deleting the PC.                                       */

  if (pc_is_alive(d)) {
    /* The PC always goes first one a tie, so we don't use new_event().  *
     * We generate one manually so that we can set the PC sequence       *
     * number to zero.                                                   */
    e = (event_t*)malloc(sizeof (*e));
    e->type = event_character_turn;
    /* Hack: New dungeons are marked.  Unmark and ensure PC goes at d->time, *
     * otherwise, monsters get a turn before the PC.                         */
    if (d->is_new) {
      d->is_new = 0;
      e->time = d->time;
    } else {
      e->time = d->time + (1000 / d->pc.speed);
    }
    e->sequence = 0;
    e->c = &d->pc;
    heap_insert(&d->events, e);
  }

  while (pc_is_alive(d) &&
         (e = (event_t*)heap_remove_min(&d->events)) &&
         ((e->type != event_character_turn) || (e->c != &d->pc))) {
    d->time = e->time;
    if (e->type == event_character_turn) {
      c = e->c;
    }
    if (!c->alive) {
      if (d->character[c->position[dim_y]][c->position[dim_x]] == c) {
        d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
      }
      if (c != &d->pc) {
        event_delete(e);
      }
      continue;
    }

    npc_next_pos(d, c, next);
    move_character(d, c, next);

    heap_insert(&d->events, update_event(d, e, 1000 / c->speed));
  }
  if(!pc_is_alive(d)){
    d->fog = 0;
  }

  //fog of war
  fogTerrain(d);
  
  //if fog of war false: print regular
  if(d->fog){
    io_displayFog(d);
  }
  else{
    io_display(d); //prints the map
  }
  //else: print fog of war
  if (pc_is_alive(d) && e->c == &d->pc) {
    c = e->c;
    d->time = e->time;
    /* Kind of kludgey, but because the PC is never in the queue when   *
     * we are outside of this function, the PC event has to get deleted *
     * and recreated every time we leave and re-enter this function.    */
    e->c = NULL;
    event_delete(e);
    io_handle_input(d); //reads input
  }
}

void dir_nearest_wall(dungeon_t *d, character_t *c, pair_t dir)
{
  dir[dim_x] = dir[dim_y] = 0;

  if (c->position[dim_x] != 1 && c->position[dim_x] != DUNGEON_X - 2) {
    dir[dim_x] = (c->position[dim_x] > DUNGEON_X - c->position[dim_x] ? 1 : -1);
  }
  if (c->position[dim_y] != 1 && c->position[dim_y] != DUNGEON_Y - 2) {
    dir[dim_y] = (c->position[dim_y] > DUNGEON_Y - c->position[dim_y] ? 1 : -1);
  }
}

uint32_t against_wall(dungeon_t *d, character_t *c)
{
  return ((mapxy(c->position[dim_x] - 1,
                 c->position[dim_y]    ) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x] + 1,
                 c->position[dim_y]    ) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x]    ,
                 c->position[dim_y] - 1) == ter_wall_immutable) ||
          (mapxy(c->position[dim_x]    ,
                 c->position[dim_y] + 1) == ter_wall_immutable));
}

uint32_t in_corner(dungeon_t *d, character_t *c)
{
  uint32_t num_immutable;

  num_immutable = 0;

  num_immutable += (mapxy(c->position[dim_x] - 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x] + 1,
                          c->position[dim_y]    ) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] - 1) == ter_wall_immutable);
  num_immutable += (mapxy(c->position[dim_x]    ,
                          c->position[dim_y] + 1) == ter_wall_immutable);

  return num_immutable > 1;
}

static void new_dungeon_level(dungeon_t *d, uint32_t dir)
{
  /* Eventually up and down will be independantly meaningful. *
   * For now, simply generate a new dungeon.                  */

  switch (dir) {
  case '<':
  case '>':
    new_dungeon(d);
    break;
  default:
    break;
  }
}


uint32_t move_pc(dungeon_t *d, uint32_t dir)
{
  pair_t next;
  uint32_t was_stairs = 0;
  char *wallmsg[] = {
    (char*)"There's a wall in the way.",
    (char*)"BUMP!",
    (char*)"Ouch!",
    (char*)"You stub your toe.",
    (char*)"You can't go that way.",
    (char*)"You admire the engravings.",
    (char*)"Are you drunk?"
  };

  next[dim_y] = d->pc.position[dim_y];
  next[dim_x] = d->pc.position[dim_x];


  switch (dir) {
  case 1:
  case 2:
  case 3:
    next[dim_y]++;
    break;
  case 4:
  case 5:
  case 6:
    break;
  case 7:
  case 8:
  case 9:
    next[dim_y]--;
    break;
  }
  switch (dir) {
  case 1:
  case 4:
  case 7:
    next[dim_x]--;
    break;
  case 2:
  case 5:
  case 8:
    break;
  case 3:
  case 6:
  case 9:
    next[dim_x]++;
    break;
  case '<':
    if (mappair(d->pc.position) == ter_stairs_up) {
      was_stairs = 1;
      new_dungeon_level(d, '<');
    }
    break;
  case '>':
    if (mappair(d->pc.position) == ter_stairs_down) {
      was_stairs = 1;
      new_dungeon_level(d, '>');
    }
    break;
  }

  if (was_stairs) {
    return 0;
  }

  if ((dir != '>') && (dir != '<') && (mappair(next) >= ter_floor)) {
    move_character(d, &d->pc, next);
    dijkstra(d);
    dijkstra_tunnel(d);

    return 0;
  } else if (mappair(next) < ter_floor) {
    io_queue_message(wallmsg[rand() % (sizeof (wallmsg) /
                                       sizeof (wallmsg[0]))]);
    if(d->fog){
    io_displayFog(d);
    }
    else{
      io_display(d); //prints the map
    }
  }

  return 1;
}
