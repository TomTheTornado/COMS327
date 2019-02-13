#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>


/**
 * Assigns a border for the 80(x-axis) x 21(y-axis) output.
 * X-axis uses '-' and Y-axis uses '|'
 */
void assign_border(char dungeon[21][80]){
    int y;
    int x;

    for(int y = 0; y < 21; y++){
        for(int x = 0; x < 80; x++){
            if(y == 0 || y == 20){
                dungeon[y][x] = '-';
            }
            else if(x == 0 || x == 79){
                dungeon[y][x] = '|';
            }
            else {
                dungeon[y][x] = ' ';
            }
        }
    }
}

/**
 * Checks to see if a room can be originated in its random x-y location.
 */
bool origin(char dungeon[21][80], int x, int y){
    //Checks if x-y position is already occupied.
    if(dungeon[y][x] == '|' || dungeon[y][x] == '-' || dungeon[y][x] == '.'){
        return false;
    }
    //Checks for a clear one space margin so the origin doesn't overlap in another room.
    else if (dungeon[y + 1][x] == '.' || dungeon[y - 1][x] == '.' || dungeon[y][x + 1] == '.' || dungeon[y][x - 1] == '.'){
        return false;
    }
    else {
        return true;
    }
    return true;
}

/**
 * Checks to see if a room is able to be fit in its random location.
 */
bool proximity(char dungeon[21][80], int startX, int startY, int length, int height){
    if((startX + length) >= 80 || (startY + height) >= 21){
        return false;
    }

    int x;
    int y;
    for(y = startY; y < (startY + height); y++){
        for(x = startX; x < (startX + length); x++){
            if(dungeon[y][x] == '.' || dungeon[y + 1][x] == '.' || dungeon[y][x + 1] == '.' || dungeon[y - 1][x] == '.' || dungeon[y][x - 1] == '.'){
                return false;
            }
        }
    }

    return true;
}

/**
 * Assigns the '.' character to the positions of the rooms.
 */
void assign_rooms(char dungeon[21][80], int startX, int startY, int length, int height){
    int y;
    int x;
    for(y = startY; y < startY + height; y++){
        for(x = startX; x < startX + length; x++){
            dungeon[y][x] = '.';
        }
    }
}

/**
 * Generates the number of rooms with a decay method. 50% chance for only 6 rooms.
 * 25% chance for 7 rooms, 12.5% chance for 8 rooms, etc. No more than 10 rooms.
 */
int numOfRooms(int seed){
    //Sets the maximum and minimum amount of rooms.
    #define MIN_RMS 6
    #define MAX_RMS 10

    srand(seed);
    bool decay = true;
    double chance = 50;
    int rooms = 6;
    while(decay == true){
        //Random out of 100.
        double probability = (rand() % 101);
        if(probability <= chance){
            //Keeps decreasing the chance for additional rooms.
            chance = chance / 2.0;
            rooms += 1;
            continue;
        }
        if(rooms == MAX_RMS){
            decay = false;
        }
        else{
            decay = false;
        }

    }
    return rooms;
    
}

/**
 * Creates the rooms for the dungeon.
 */
void create_rooms(char dungeon[21][80], int seed, int rmCorners[10][2]){

    //Sets min dimensions for the rooms as constants.
    #define min_rmX 4
    #define min_rmY 3
    //Sets max dimensions for the rooms as constants.
    #define max_rmX 15
    #define max_rmY 12

    //Initializes random number generator with current seed.
    srand(seed);

    //Generates 6-10 rooms for the dungeon using a decay method.
    int numRooms = numOfRooms(seed);
    //Use [num = (rand() % (upper – lower + 1)) + lower] for number range
    
    //Initializes random origin points.
    int randX;
    int randY;

    //Initializes random length and height.
    int rLength;
    int rHeight;

    int attempts;
    int r;
    for(r = 0; r < numRooms; r++){
        attempts++;

        randX = (rand() % (80 - 0 + 1));
        randY = (rand() % (21 - 0 + 1));

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
                        rmCorners[r][0] = randY;
                        rmCorners[r][1] = randX;
                }
            }
            //If it fits, assigns locations to '.' and saves origin location.
            else{
                assign_rooms(dungeon, randX, randY, rLength, rHeight);
                    rmCorners[r][0] = randY;
                    rmCorners[r][1] = randX;
            }
        }
    }
}

/**
 * Assigns the char '#' to the corridors.
 */
void assign_corridors(char dungeon[21][80], int destX, int destY, int cornerX, int cornerY){
    int x;
    int y;
    //Draws a y-corridor from the x-y destination backwards to the corridor corner.
    if (destY > cornerY) {
        for(y = destY; y > cornerY; y--){
            if(dungeon[y - 1][cornerX] == ' '){
                dungeon[y - 1][cornerX] = '#';
            }   
        }
    }
    //Draws a y-corridor from the x-y destination to the corridor corner.
    if(destY < cornerY){
        for(y = destY; y < cornerY; y++){
            if(dungeon[y + 1][cornerX] == ' '){
                dungeon[y + 1][cornerX] = '#';
            }   
        } 
    } 
    //Draws a x-corridor from the x-y destination backwards to the corridor corner.
    if(cornerX > destX){
        for(x = cornerX; x > destX; x--){
            if(dungeon[cornerY][x - 1] == ' '){
                dungeon[cornerY][x - 1] = '#';
            }
        }
    }
    //Draws a x-corridor from the x-y destination forwards to the corridor corner.
    if(cornerX < destX){
        for(x = cornerX; x < destX; x++){
            if(dungeon[cornerY][x + 1] == ' '){
                dungeon[cornerY][x + 1] = '#';
            }
        }
    }
}

/**
 * Creates corridors to connect the rooms.
 */
void create_corridors(char dungeon[21][80], int corners[10][2], int seed){
    int rmNum = 0;
    srand(seed);
    int direction;
    int cornerX;
    int cornerY;
    int startX;
    int startY;
    int endX;
    int endY;
    int numRooms = numOfRooms(seed);

    while(rmNum < numRooms - 1){
        direction = rand() % 2;

        //Corridor shifts by height, then length.
        if(direction == 0){
            cornerY = corners[rmNum][0];
            cornerX = corners[rmNum + 1][1];
        }
        //Corridor shifts by length, then height.
        else if(direction == 1){
            cornerY = corners[rmNum + 1][0];
            cornerX = corners[rmNum][1];
        }

        startY = corners[rmNum][0];
        startX = corners[rmNum][1];

        endY = corners[rmNum + 1][0];
        endX = corners[rmNum + 1][1];

        //For point 1 to corridor corner.
        assign_corridors(dungeon, startX, startY, cornerX, cornerY);
        //For corridor corner to point 2.
        assign_corridors(dungeon, endX, endY, cornerX, cornerY);
        // assign_corridors(dungeon, cornerX, cornerY, endX, endY, false);
        rmNum++;
    }
}

/**
 * Creates stairs.
 */
void create_stairs(char dungeon[21][80], int seed){
    srand(seed);

    //Maximum of 2 upstairs and 2 downstairs
    int up = ((rand() % 2) + 1);
    int down = ((rand() % 2) + 1);
    int i = 0;
    while(i != down){
        int x = (rand() % (80 - 0 + 1));
        int y = (rand() % (21 - 0 + 1));
        if(dungeon[y][x] == '.' || dungeon[y][x] == '#'){
            dungeon[y][x] = '<';
            i++;
        }
    }
    i = 0;
    while(i != up){
        int x = (rand() % (80 - 0 + 1));
        int y = (rand() % (21 - 0 + 1));
        if(dungeon[y][x] == '.' || dungeon[y][x] == '#'){
            dungeon[y][x] = '>';
            i++;
        }
    }
}

/**
 * Main function for dungeon.c.
 */
int main(int argc, char const *argv[])
{

    //Sets max dimensions of output as constants.
    #define dimX 80
    #define dimY 21
    //Creates 2D Dungeon array.
    char dungeon[dimY][dimX];
    
    //Creates a random unique seed for the dungeon.
    time_t t;
    t = time(NULL);
    int seed = t;

    //Gives a border for the 80 x 21 output
    assign_border(dungeon);

    //Holds the x-y corner points for the rooms. Needed to connect corridors.
    int rmCorners[10][2];

    //Creates the rooms for the dungeons.
    create_rooms(dungeon, seed, rmCorners);

    //Creates the corridors for the dungeons.
    create_corridors(dungeon, rmCorners, seed);

    //Adds in stairs for the dungeon.
    create_stairs(dungeon, seed);

    //Prints out the dungeon itself.
    int y;
    int x;
    for(y = 0; y < dimY; y++){
        for(x = 0; x < dimX; x++){
            printf("%c", dungeon[y][x]);
        }
        printf("\n");
    }

    return 0;
}