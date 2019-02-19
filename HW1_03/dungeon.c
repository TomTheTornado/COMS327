#include "dungeon.h"
#include "load_save.h"

void free_dungeon(dungeon_t *dungeon)
{
    free(dungeon->upstrs);
    free(dungeon->downstrs);
    free(dungeon->rooms);
}

void assign_weight(dungeon_t *dungeon){
    uint8_t x, y;
    for(y = 0; y < max_dimY; y++){
        for(x = 0; x < max_dimX; x++){
            if(dungeon->hardness[y][x] == 255){
                dungeon->weight[y][x] = 9;
            }
            if(dungeon->hardness[y][x] == 0){
                dungeon->weight[y][x] = 1;
            }
            else{
                dungeon->weight[y][x] = ((dungeon->hardness[y][x]) / 85) + 1;
            }
        }
    }
}

void print_playerMap(dungeon_t *dungeon){
    uint8_t type = 1;
    uint8_t playerMap;

    print_dungeon(dungeon, type);
}

void print_tunnelingMap(dungeon_t *dungeon){
    uint8_t type = 2;
    dungeon_t tunnelingMap;

    print_dungeon(dungeon, type);
}

void place_player(dungeon_t *dungeon)
{
    uint8_t x, y;

    while (1)
    {
        x = rand() % (max_dimX - 1) + 1;
        y = rand() % (max_dimY - 1) + 1;

        if (dungeon->hardness[y][x] == 0)
        {
            dungeon->pc.x = x;
            dungeon->pc.y = y;
            break;
        }
    }
    return;
}

/**
 * Main function for dungeon.c.
 */
int main(int argc, char const *argv[]){
    //Creates a random unique seed for the dungeon.
    time_t t;
    t = time(NULL);
    int seed = t;
    srand(seed);
    
    //Creates the dungeon struct.
    dungeon_t dungeon;

    action_t action;

    int i;
    for (i = 1; i < argc; i++) //check for flags
    {
        if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "-s"))
            action = (action != load) ? save : load_save;
        else if (!strcmp(argv[i], "--load") || !strcmp(argv[i], "-l"))
            action = (action != save) ? load : load_save;
        else if (!strcmp(argv[i], "--seed") && (i + 1 < argc))
            seed = atoi(argv[i + 1]); 
        else if(!strcmp(argv[i], "-sl") || !strcmp(argv[i], "-ls")){
            action = load_save;
        }
    }

    switch (action)
    {
    case load:
        printf("loading...\n");
        load_dungeon(&dungeon);
        break;
    case save:
        printf("saving...\n");
        generate_dungeon(&dungeon);
        save_dungeon(&dungeon);
        break;
    case load_save:
        printf("loading and saving...\n");
        load_dungeon(&dungeon);
        save_dungeon(&dungeon);
        break;
    default:
        generate_dungeon(&dungeon);
        break;
    }

    //printf("Seed: %d\n", seed);
    print_dungeon(&dungeon, 0);//normal
    //hardness
    print_dungeon(&dungeon, 4);
    //weight
    print_dungeon(&dungeon, 3);
    //print_playerMap(&dungeon);
    //print_tunnelingMap(&dungeon);


    //free_dungeon(&dungeon);
    return 0;
}
void generate_dungeon(dungeon_t *dungeon){
    assign_border(dungeon);
    create_rooms(dungeon);
    create_corridors(dungeon);
    assign_hardness(dungeon);
    create_stairs(dungeon);
    place_player(dungeon);
    assign_weight(dungeon);
    
}
void print_dungeon(dungeon_t *dungeon, uint8_t type){
    int y, x;
    for(y = 0; y < max_dimY; y++){
        for(x = 0; x < max_dimX; x++){
            if(x == dungeon->pc.x && y == dungeon->pc.y){
                printf("%c", '@');
            }
            //char moves
            else if(type == 1){
                printf("%d", dungeon->playerMap[y][x]);
            }
            else if(type == 2){
                printf("%d", dungeon->tunnelingMap[y][x]);
            }
            else if(type == 3){
                printf("%d", dungeon->weight[y][x]);
            }
            else if(type == 4){
                printf("%2x", dungeon->hardness[y][x]);
            }
            else{
                printf("%c", dungeon->map[y][x]);
            }
        }
        printf("\n");
    }
    printf("\n");
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
int numOfRooms(){

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
void create_rooms(dungeon_t *dungeon){

    //Creates the maximum number of rooms
    dungeon->numRooms = numOfRooms();
    
    //Gets 6-10 rooms for the dungeon
    
    int totalRooms;
    totalRooms = dungeon->numRooms;

    //Initializes random origin points.
    int randX, randY;
    //Initializes random length and height.
    int rLength, rHeight;
    int roomDetails[dungeon->numRooms][4];

    int attempts, r;

    for(r = 0; r < totalRooms; r++){
        attempts++;

        randX = (rand() % (max_dimX + 1));
        randY = (rand() % (max_dimY + 1));

        rLength = (rand() % (max_rmX - min_rmX + 1) + min_rmX);
        rHeight = (rand() % (max_rmY - min_rmY + 1) + min_rmY);
        //Checks to see if the origin point is not open.
        if(!origin(dungeon, randX, randY)){
            //Breaks out if it is unable to place rooms after 1000 attempts and 6 rooms are placed.
            if(attempts > 1000 && (r >= 6)){
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
                        roomDetails[r][1] = randY;
                        roomDetails[r][2] = randX;
                        roomDetails[r][3] = rHeight;
                        roomDetails[r][4] = rLength;
                }
            }
            //If it fits, assigns locations to '.' and saves origin location.
            else{
                assign_rooms(dungeon, randX, randY, rLength, rHeight);
                    // dungeon->rooms[r].y = randY;
                    // dungeon->rooms[r].x = randX;
                    // dungeon->rooms[r].height = rHeight;
                    // dungeon->rooms[r].length = rLength;

                    roomDetails[r][1] = randY;
                    roomDetails[r][2] = randX;
                    roomDetails[r][3] = rHeight;
                    roomDetails[r][4] = rLength;
            }
        }
    }

    dungeon->rooms = malloc(sizeof(room_t)*dungeon->numRooms);
    int i;
    for(i = 0; i < r; i++){
        dungeon->rooms[i].y = roomDetails[i][1];
        dungeon->rooms[i].x = roomDetails[i][2];
        dungeon->rooms[i].height = roomDetails[i][3];
        dungeon->rooms[i].length = roomDetails[i][4];
    }

}
void assign_corridor(dungeon_t *dungeon, uint8_t x, uint8_t y){

    if (dungeon->map[y][x] == ' '){
        dungeon->map[y][x] = '#';
    }
    return;
}
void create_corridors(dungeon_t *dungeon){

    int i;
    for (i = 0; i < dungeon->numRooms - 1; i++){
        uint8_t startX, startY, endX, endY;

        //start location
        startX = dungeon->rooms[i].x + (rand() % dungeon->rooms[i].length);
        startY = dungeon->rooms[i].y + (rand() % dungeon->rooms[i].height);

        //end location
        endX = dungeon->rooms[i + 1].x + (rand() % dungeon->rooms[i + 1].length);
        endY = dungeon->rooms[i + 1].y + (rand() % dungeon->rooms[i + 1].height);

        while ((startX != endX) || (startY != endY)){
            int8_t moveX = (startX < endX) ? 1 : -1;
            int8_t moveY = (startY < endY) ? 1 : -1;

            uint8_t pos_x = startX + moveX;
            uint8_t pos_y = startY + moveY;

            float distance_x = sqrt(pow((endX - pos_x), 2) + pow(endY - startY, 2));
            float distance_y = sqrt(pow((endX - startX), 2) + pow(endY - pos_y, 2));
            float previous_d = sqrt(pow((endX - startX), 2) + pow(endY - startY, 2));

            if (distance_x < previous_d && distance_y < previous_d){
                if (dungeon->hardness[startY][pos_x] < dungeon->hardness[pos_y][startY]){
                    startX += moveX;
                }
                else {
                    startY += moveY;
                }
            }

            else{
                if (distance_x < distance_y){
                    startX += moveX;
                }
                else{
                    startY += moveY;
                }
            }

            assign_corridor(dungeon, startX, startY);
        }
    }
    return;
}
void create_stairs(dungeon_t *dungeon){
    
    dungeon->numUpStrs = ((rand() % max_upstrs) + 1);
    dungeon->numDownStrs = ((rand() % max_downstrs) + 1);
    dungeon->upstrs = malloc(sizeof(location_t)*dungeon->numUpStrs);
    dungeon->downstrs = malloc(sizeof(location_t)*dungeon->numDownStrs);
 

    int up = dungeon->numUpStrs;
    int down = dungeon->numDownStrs;
    int i = 0;
    while(i != up){//Upstairs
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
    while(i != down){//Downstairs
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
            else if(dungeon->map[y][x] == '.'){dungeon->hardness[y][x] = 0;}                      //Downstairs
            else{dungeon->hardness[y][x] = (rand() % 254) + 1;}                                             //Rock
        }
    }
} 