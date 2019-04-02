#ifndef PARSE_H
# define PARSE_H

typedef class monster_desc{
public:
  string NAME;
  string SYMB;
  string COLOR;
  string DESC;
  string SPEED;
  string DAM;
  string HP;
  string ABIL;
  string RRTY;
} mon_desc_t;

typedef class obj_desc{
public:
  string NAME;
  string DESC;
  string TYPE;
  string COLOR;
  string HIT;
  string DAM;
  string DODGE;
  string DEF;
  string WEIGHT;
  string SPEED;
  string ATTR;
  string VAL;
  string ART;
  string RRTY;
} obj_desc_t;

void printMonDesc(mon_desc_t *description);
void read_monsters(dungeon *d);
void printObjDesc(mon_desc_t *description);
void read_objects(dungeon *d);

#endif