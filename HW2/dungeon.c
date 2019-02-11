#include "dungeon.h"

/**
 * Saves the dungeon file.
 */
void save_file(dungeon_t *dungeon){

}

/**
 * Loads the dungeon file.
 */
void load_file(dungeon_t *dungeon){
    
}

/**
 * Main function for dungeon.c.
 */
int main(int argc, char const *argv[]){
    //Creates the dungeon struct.
    dungeon_t dungeon;
    
    //Creates a random unique seed for the dungeon.
    time_t t;
    t = time(NULL);
    int seed = t;
    if(argv[1] == NULL){
        assign_border(&dungeon);
        create_rooms(&dungeon, seed);
        create_corridors(&dungeon, seed);
        create_stairs(&dungeon, seed);
        assign_hardness(&dungeon);
    }
    else if((strcmp(argv[1], "--load") == 0) && (strcmp(argv[2], "--save") == 0)){
        load_file(&dungeon);
        save_file(&dungeon);
    }
    else if(strcmp(argv[1], "--load") == 0 && argv[2] == NULL){
        load_file(&dungeon);
    }
    else if(strcmp(argv[1], "--save") == 0 && argv[2] == NULL){
        assign_border(&dungeon);
        create_rooms(&dungeon, seed);
        create_corridors(&dungeon, seed);
        create_stairs(&dungeon, seed);
        assign_hardness(&dungeon);
        save_file(&dungeon);
    }
    else{
        printf("Unknown Command: Please use \"--load \", \"--save\", or \"--load --save\".\n");
    }

    //Prints out the dungeon itself.
    int y, x;
    for(y = 0; y < max_dimY; y++){
        for(x = 0; x < max_dimX; x++){
            printf("%c", dungeon.map[y][x]);
        }
        printf("\n");
    }

    return 0;
}

void assign_border(dungeon_t *dungeon){
    int y, x;

    for(int y = 0; y < max_dimY; y++){
        for(int x = 0; x < max_dimX; x++){
            if(y == 0 || y == max_dimY - 1){dungeon->map[y][x] = '-';}
            else if(x == 0 || x == max_dimX - 1){dungeon->map[y][x] = '|';}
            else {dungeon->map[y][x] = ' ';}
        }
    }
}
bool origin(dungeon_t *dungeon, int x, int y){
    //Checks if x-y position is already occupied.
    if(dungeon->map[y][x] == '|' || dungeon->map[y][x] == '-' || dungeon->map[y][x] == '.'){return false;}
    //Checks for a clear one space margin so the origin doesn't overlap in another room.
    else if (dungeon->map[y + 1][x] == '.' || dungeon->map[y - 1][x] == '.' || dungeon->map[y][x + 1] == '.' || dungeon->map[y][x - 1] == '.'){return false;}
    else {return true;}
    return true;
}
bool proximity(dungeon_t *dungeon, int startX, int startY, int length, int height){
    if((startX + length) >= max_dimX || (startY + height) >= max_dimY){return false;}

    int x, y;
    for(y = startY; y < (startY + height); y++){
        for(x = startX; x < (startX + length); x++){
            if(dungeon->map[y][x] == '.' || dungeon->map[y + 1][x] == '.' || dungeon->map[y][x + 1] == '.' 
            || dungeon->map[y - 1][x] == '.' || dungeon->map[y][x - 1] == '.'){return false;}
        }
    }
    return true;
}
void assign_rooms(dungeon_t *dungeon, int startX, int startY, int length, int height){
    int y, x;
    for(y = startY; y < startY + height; y++){
        for(x = startX; x < startX + length; x++){dungeon->map[y][x] = '.';}
    }
}
int numOfRooms(int seed){

    srand(seed);
    bool decay = true;
    double chance = 50;
    int rooms = min_rms;
    while(decay == true){
        //Random out of 100.
        double probability = (rand() % 101);
        if(probability <= chance){
            //Keeps decreasing the chance for additional rooms.
            chance = chance / 2.0;
            rooms += 1;
            continue;
        }
        if(rooms == max_rms){decay = false;}
        else{decay = false;}
    }
    return rooms;
    
}
void create_rooms(dungeon_t *dungeon, int seed){

    //Initializes random number generator with current seed.
    srand(seed);

    //Generates 6-10 rooms for the dungeon using a decay method.
    int numRooms = numOfRooms(seed);
    //Use [num = (rand() % (upper – lower + 1)) + lower] for number range
    
    //Initializes random origin points.
    int randX, randY;
    //Initializes random length and height.
    int rLength, rHeight;

    int attempts, r;
    for(r = 0; r < numRooms; r++){
        attempts++;

        randX = (rand() % (max_dimX + 1));
        randY = (rand() % (max_dimY + 1));

        rLength = (rand() % (max_rmX - min_rmX + 1) + min_rmX);
        rHeight = (rand() % (max_rmY - min_rmY + 1) + min_rmY);
        //Checks to see if the origin point is not open.
        if(!origin(dungeon, randX, randY)){
            //Breaks out if it is unable to place rooms after 2000 attempts and 6 rooms are placed.
            if(attempts > 2000 && (r == 5)){
                break;
            }
            else{
                r -= 1;
                continue;
            }
        }
        else{
            //Checks to see if length and height of room will fit in space.
            if(!proximity(dungeon, randX, randY, rLength, rHeight)){
                
                rLength = (rand() % (8 - min_rmX + 1) + min_rmX);
                rHeight = (rand() % (6 - min_rmY + 1) + min_rmY);
                //Checks to see if a smaller room will fit in the space.
                if(!proximity(dungeon, randX, randY, rLength, rHeight)){
                    r -= 1;
                    continue;
                }
                //If it fits, assigns locations to '.' and saves origin location.
                else{
                    assign_rooms(dungeon, randX, randY, rLength, rHeight);
                        dungeon->rooms[r].y = randY;
                        dungeon->rooms[r].x = randX;
                        dungeon->rooms[r].height = rHeight;
                        dungeon->rooms[r].length = rLength;
                }
            }
            //If it fits, assigns locations to '.' and saves origin location.
            else{
                assign_rooms(dungeon, randX, randY, rLength, rHeight);
                    dungeon->rooms[r].y = randY;
                    dungeon->rooms[r].x = randX;
                    dungeon->rooms[r].height = rHeight;
                    dungeon->rooms[r].length = rLength;
            }
        }
    }
    //Assigns player location starting point
    dungeon->pc.y = dungeon->rooms[0].y;
    dungeon->pc.x = dungeon->rooms[0].x;
}
void assign_corridors(dungeon_t *dungeon, int destX, int destY, int cornerX, int cornerY){
    int x, y;
    //Draws a y-corridor from the x-y destination backwards to the corridor corner.
    if (destY > cornerY) {
        for(y = destY; y > cornerY; y--){
            if(dungeon->map[y - 1][cornerX] == ' '){dungeon->map[y - 1][cornerX] = '#';}   
        }
    }
    //Draws a y-corridor from the x-y destination to the corridor corner.
    if(destY < cornerY){
        for(y = destY; y < cornerY; y++){
            if(dungeon->map[y + 1][cornerX] == ' '){dungeon->map[y + 1][cornerX] = '#';}   
        } 
    } 
    //Draws a x-corridor from the x-y destination backwards to the corridor corner.
    if(cornerX > destX){
        for(x = cornerX; x > destX; x--){
            if(dungeon->map[cornerY][x - 1] == ' '){dungeon->map[cornerY][x - 1] = '#';}
        }
    }
    //Draws a x-corridor from the x-y destination forwards to the corridor corner.
    if(cornerX < destX){
        for(x = cornerX; x < destX; x++){
            if(dungeon->map[cornerY][x + 1] == ' '){dungeon->map[cornerY][x + 1] = '#';}
        }
    }
}
void create_corridors(dungeon_t *dungeon, int seed){
    
    int rmNum = 0;
    int direction, cornerX, cornerY, startX, startY, endX, endY;
    srand(seed);
    dungeon->numRooms = numOfRooms(seed);

    while(rmNum < dungeon->numRooms - 1){
        direction = rand() % 2;

        //Corridor shifts by height, then length.
        if(direction == 0){
            cornerY = dungeon->rooms[rmNum].y;
            cornerX = dungeon->rooms[rmNum + 1].x;
        }
        //Corridor shifts by length, then height.
        else if(direction == 1){
            cornerY = dungeon->rooms[rmNum + 1].y;
            cornerX = dungeon->rooms[rmNum].x;
        }

        startY = dungeon->rooms[rmNum].y;
        startX = dungeon->rooms[rmNum].x;

        endY = dungeon->rooms[rmNum + 1].y;
        endX = dungeon->rooms[rmNum + 1].x;

        //For point 1 to corridor corner.
        assign_corridors(dungeon, startX, startY, cornerX, cornerY);
        //For corridor corner to point 2.
        assign_corridors(dungeon, endX, endY, cornerX, cornerY);
        // assign_corridors(dungeon, cornerX, cornerY, endX, endY, false);
        rmNum++;
    }
}
void create_stairs(dungeon_t *dungeon, int seed){
    srand(seed);

    int up = ((rand() % max_upstrs) + 1);
    int down = ((rand() % max_downstrs) + 1);
    int i = 0;
    while(i != down){//Upstairs
        int x = (rand() % (max_dimX + 1));
        int y = (rand() % (max_dimY + 1));
        if(dungeon->map[y][x] == '.' || dungeon->map[y][x] == '#'){
            dungeon->map[y][x] = '<';
            dungeon->upstrs[i].y = y;
            dungeon->upstrs[i].x = x;
            i++;
        }
    }
    i = 0;
    while(i != up){//Downstairs
        int x = (rand() % (max_dimX + 1));
        int y = (rand() % (max_dimY + 1));
        if(dungeon->map[y][x] == '.' || dungeon->map[y][x] == '#'){
            dungeon->map[y][x] = '>';
            dungeon->downstrs[i].y = y;
            dungeon->downstrs[i].x = x;
            i++;
        }
    }
}
void assign_hardness(dungeon_t *dungeon){
    int y, x;
    for(y = 0; y < max_dimY; y++){
        for(x = 0; x < max_dimX; x++){
            if(dungeon->map[y][x] == '-' || dungeon->map[y][x] == '|'){dungeon->hardness[y][x] = 255;} //Immutable Border
            else if(dungeon->map[y][x] == '#'){dungeon->hardness[y][x] = 0;}                      //Corridor
            else if(dungeon->map[y][x] == '.'){dungeon->hardness[y][x] = 1;}                      //Room
            else if(dungeon->map[y][x] == '<'){dungeon->hardness[y][x] = 2;}                      //Upstairs
            else if(dungeon->map[y][x] == '>'){dungeon->hardness[y][x] = 3;}                      //Downstairs
            else{dungeon->hardness[y][x] = 254;}                                             //Rock
        }
    }
}