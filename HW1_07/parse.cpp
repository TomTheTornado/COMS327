#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <iostream>
using namespace std;


#include "headers/dungeon.h"
#include "headers/parse.h"


void printMonDesc(mon_desc_t description){

  cout << description.NAME << '\n';
  cout << description.DESC;
  cout << description.SYMB << '\n';
  cout << description.COLOR << '\n';
  cout << description.SPEED << '\n';
  cout << description.ABIL << '\n';
  cout << description.HP << '\n';
  cout << description.DAM << '\n';
  cout << description.RRTY << '\n';
  cout << '\n';

}

void read_monsters(dungeon *d){

  char *dir = getenv("HOME");
  char *path = strcat(dir, "/.rlg327/");

  std::ifstream mon_desc;
  mon_desc.open(strcat(path, "/monster_desc.txt"));
  string metadata;
  getline(mon_desc, metadata);

  if(metadata != "RLG327 MONSTER DESCRIPTION 1"){
    return;
  }

  mon_desc_t description;
  string curLine;

    while(getline(mon_desc, curLine)){
      if(curLine == "BEGIN MONSTER" || curLine == ""){
        continue;
      }
      else if(curLine == "END"){
        printMonDesc(description);
        description.NAME = "";
        description.DESC = "";
        description.COLOR = "";
        description.SPEED = "";
        description.ABIL = "";
        description.HP = "";
        description.DAM = "";
        description.SYMB = "";
        description.RRTY = "";
      }
      else if(curLine.substr(0, 4) == "NAME"){
        if(description.NAME == ""){ 
          description.NAME = curLine.substr(5, curLine.length());
        }
      }
      else if(curLine.substr(0, 4) == "DESC"){
        while(getline(mon_desc, curLine)){
          if(curLine.substr(0, 1) == "."){
            break;
          }
          else{
            description.DESC += curLine.substr(0, 77) + '\n';
          }
      
        }
      }
      else if(curLine.substr(0, 5) == "COLOR"){
        if(description.COLOR == ""){ 
          description.COLOR = curLine.substr(6, curLine.length());
        }
      }
      else if(curLine.substr(0, 5) == "SPEED"){
        if(description.SPEED == ""){ 
          description.SPEED = curLine.substr(6, curLine.length());
        }
      }
      else if(curLine.substr(0, 4) == "ABIL"){
        if(description.ABIL == ""){ 
          description.ABIL = curLine.substr(5, curLine.length());
        }
      }
      else if(curLine.substr(0, 2) == "HP"){
        if(description.HP == ""){ 
          description.HP = curLine.substr(3, curLine.length());
        }
      }
      else if(curLine.substr(0, 3) == "DAM"){
        if(description.DAM == ""){ 
          description.DAM = curLine.substr(4, curLine.length());
        }
      }
      else if(curLine.substr(0, 4) == "SYMB"){
        if(description.SYMB == ""){ 
          description.SYMB = curLine.substr(5, curLine.length());
        }
      }
      else if(curLine.substr(0, 4) == "RRTY"){
        if(description.RRTY == ""){ 
        description.RRTY = curLine.substr(5, curLine.length());
        }
      }
    }

  mon_desc.close();
}


//ITEMS
void printObjDesc(obj_desc_t description){

  cout << description.NAME << '\n';
  cout << description.DESC;
  cout << description.TYPE << '\n';
  cout << description.COLOR << '\n';
  cout << "Hit: " << description.HIT << '\n';
  cout << "Damage: " << description.DAM << '\n';
  cout << "Dodge: " << description.DODGE << '\n';
  cout << "Defense: " << description.DEF << '\n';
  cout << "Weight: " << description.WEIGHT << '\n';
  cout << "Speed: " << description.SPEED << '\n';
  cout << "Attribute: " << description.ATTR << '\n';
  cout << "Value: " << description.VAL << '\n';
  cout << "Artifact: " << description.ART << '\n';
  cout << "Rarity: " << description.RRTY << '\n';
  cout << '\n';

}

void read_objects(dungeon *d){

  char *dir = getenv("HOME");
  char *path = strcat(dir, "/.rlg327/");

  std::ifstream obj_desc;
  obj_desc.open(strcat(path, "/object_desc.txt"));
  string metadata;
  getline(obj_desc, metadata);

  if(metadata != "RLG327 OBJECT DESCRIPTION 1"){
    return;
  }

  obj_desc_t description;
  string curLine;

    while(getline(obj_desc, curLine)){
      if(curLine == "BEGIN MONSTER" || curLine == ""){
        //printf("Begin ");
        continue;
      }
      else if(curLine == "END"){
        printObjDesc(description);

        description.NAME = "";;
        description.DESC = "";;
        description.TYPE = "";;
        description.COLOR = "";;
        description.HIT = "";;
        description.DAM = "";;
        description.DODGE = "";;
        description.DEF = "";;
        description.WEIGHT = "";;
        description.SPEED = "";;
        description.ATTR = "";;
        description.VAL = "";;
        description.ART = "";;
        description.RRTY = "";;
        //i++;
      }
      else if(curLine.substr(0, 4) == "NAME"){
        if(description.NAME == ""){ 
          description.NAME = curLine.substr(5, curLine.length());
        }
      }
      else if(curLine.substr(0, 4) == "DESC"){
        while(getline(obj_desc, curLine)){
          if(curLine.substr(0, 1) == "."){
            break;
          }
          else{
            description.DESC += curLine.substr(0, 77) + '\n';
          }
      
        }
      }
      else if(curLine.substr(0, 4) == "TYPE"){
        if(description.TYPE == ""){ 
          description.TYPE = curLine.substr(5, curLine.length());
        }
      }
      else if(curLine.substr(0, 5) == "COLOR"){
        if(description.COLOR == ""){ 
          description.COLOR = curLine.substr(6, curLine.length());
        }
      }
      else if(curLine.substr(0, 3) == "HIT"){
        if(description.HIT == ""){ 
          description.HIT = curLine.substr(4, curLine.length());
        }
      }
      else if(curLine.substr(0, 3) == "DAM"){
        if(description.DAM == ""){ 
          description.DAM = curLine.substr(4, curLine.length());
        }
      }
      else if(curLine.substr(0, 5) == "DODGE"){
        if(description.DODGE == ""){ 
          description.DODGE = curLine.substr(6, curLine.length());
        }
      }
      else if(curLine.substr(0, 3) == "DEF"){
        if(description.DEF == ""){ 
          description.DEF = curLine.substr(4, curLine.length());
        }
      }
      else if(curLine.substr(0, 6) == "WEIGHT"){
        if(description.WEIGHT == ""){ 
          description.WEIGHT = curLine.substr(7, curLine.length());
        }
      }
      else if(curLine.substr(0, 5) == "SPEED"){
        if(description.SPEED == ""){ 
          description.SPEED = curLine.substr(6, curLine.length());
        }
      }
      else if(curLine.substr(0, 4) == "ATTR"){
        if(description.ATTR == ""){ 
          description.ATTR = curLine.substr(5, curLine.length());
        }
      }
      else if(curLine.substr(0, 3) == "VAL"){
        if(description.VAL == ""){ 
          description.VAL = curLine.substr(4, curLine.length());
        }
      }
      else if(curLine.substr(0, 3) == "ART"){
        if(description.ART == ""){ 
          description.ART = curLine.substr(4, curLine.length());
        }
      }
      else if(curLine.substr(0, 4) == "RRTY"){
        if(description.RRTY == ""){ 
          description.RRTY = curLine.substr(5, curLine.length());
        }
      }

    }

  obj_desc.close();
}