#include <iostream>
using namespace std;
//Global Variables Declared
bool gameOver;
const int length = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
enum eDirection{
    STOP = 0, 
    LEFT, 
    UP, 
    RIGHT,
    DOWN
};
eDirection dir;


void setup(){
    gameOver = false;
    dir = STOP;
    x = length / 2;
    y = height / 2;
    fruitX = rand() % length;
    fruitY = rand() % height;
    score = 0;
}
void draw(){
    system("clear"); //Clears the console screen on linux
    int i, j;
    for(i = 0; i < length + 2; i++){
        cout << "#";  
    }
    cout << endl;
    for(i = 0; i < height; i++){
        for(j = 0; j < length; j++){
            if (j == 0){
                cout << "#";
                
            }
            cout << " ";
            if (j==length - 1){
                cout << "#";
            }
        }
        cout << endl; //end l stands for end line
    }
    for(i = 0; i < length + 2; i++){
        cout << "#";
    }
    cout << endl;
}
void input(){

}
void logic(){

}

int main(){
    setup();
    while(!gameOver){
        draw();
        input();
        logic();
        //sleep(10) to slow game
    }
    //draw();

    return 0;
}