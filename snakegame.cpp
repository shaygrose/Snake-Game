#include <iostream>
#include <cstdlib>
#include <ncurses.h>

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, FruitX, FruitY, score;
enum eDirection {STOP=0, LEFT, RIGHT, UP, DOWN};
eDirection dir;

int TailX[100], TailY[100]; //can have up to 100 tail sections
int nTail = 0; //number of tail pieces at beginning


void Setup() {

    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);

    gameOver= false;
    dir = STOP;
    x = width/2; //starting snake in middle of the board
    y = height/2;
    FruitX = (rand() % width )+ 1; //makes sure fruit spawns within the board
    FruitY = (rand() % height) + 1;
    score = 0;
}

void Draw() {

    clear();
    
    for (int i = 0; i<width+2; i++){  //prints top border
        mvprintw(0, i, "+");
    }

    for (int i = 0; i<height+2; i++){  
        
        for(int j = 0; j<width+2; j++){  //for every column
            if(i==0 | i==21){           //if i is the top or bottom row
                mvprintw(i,j,"*");
            }
            else if(j==0 | j==21){   //if column is left or right edge
                mvprintw(i,j, "*");
            }
            else if(i==y && j==x){
                mvprintw(i,j, "@");
            }
            else if(i==FruitY && j==FruitX){
                mvprintw(i,j, "+");
            }
            else{
                for(int k=0; k<nTail; k++){
                    if(TailX[k] == j && TailY[k] == i){
                        mvprintw(i,j, "+");
                        
                    }
                }
            }
        }
    }

    mvprintw(23,0,"Score: %d", score);

    refresh();

}

void Input() {

    keypad(stdscr, TRUE);
    halfdelay(1);
    int c = getch();

    switch(c){ //switch case to change direction head wants to travel in
                //doesn't actually move the head, see Logic function

        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        
        case KEY_DOWN:
            dir = DOWN;
            break;

        case KEY_UP:
            dir = UP;
            break;
        case 113:  //ASCII code for q "quit"
            gameOver = true;
            break;
    }
}

void Logic() {

    switch(dir){ //moves snake head in direction specified
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
    }

    if(x>width | x<1 | y>height | y<1){  //if snake runs into a wall
        gameOver = true;
    }

    if(x==FruitX && y ==FruitY){ //if snake eats a fruit
        score++;  
        nTail++;
        FruitX = (rand() % width )+ 1; //respawn fruit somewhere else
        FruitY = (rand() % height) + 1; 
    }

    for(int i=0; i<nTail; i++){
        if(TailX[i] ==x && TailY[i]==y){ //if head is in same position as any tail piece
            gameOver = true;            //you die
        }
    }

    int prevX = TailX[0];
    int prevY = TailY[0];
    TailX[0] = x;
    TailY[0] = y;
    int prevprevX, prevprevY;

    for(int i=1; i<nTail; i++){
        prevprevX = TailX[i]; //set prevprev to next positionin tail
        prevprevY = TailY[i];
        TailX[i] = prevX; //shift tailx and taily
        TailY[i] = prevY;
        prevX = prevprevX;
        prevY = prevprevY;
    }

}

int main() {

    Setup();
    

    while(!gameOver){
        Draw();
        Input();
        Logic();
    }

    getch();
    endwin();

    return 0;

}