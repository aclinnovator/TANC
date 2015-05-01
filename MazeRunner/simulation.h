#include "./pathfind.h"


void ppt(Point pt) { printf("x: %d, y: %d\n",pt.x, pt.y); }
void printVec4f(Vec4f _vec){
  for(int i = 0; i < 4; i++) { printf("%f\n",_vec[i]); }
}



/*
        #---#---#---#---#---#
    5   |   |   |   |   |   |
        #---#---#---#---#---#
    4   |   |###|###|###|   |
        #---#---#---#---#---#
    3   |   |###|###|###|   |
        #---#---#---#---#---#
    2   |   |###|###|###|   |
        #---#---#---#---#---#
    1   |   |###|###|###|   |
        #---#---#---#---#---#
      1   2   3   4   5

*/

#define START_POS Vec(0,4)

int maze[MAZE_WIDTH][MAZE_HEIGHT] = {
    { 0 , 0 , 0 , 0 , 0 },
    { 1 , 1 , 1 , 1 , 0 },
    { 1 , 1 , 1 , 1 , 0 },
    { 1 , 1 , 1 , 1 , 0 },
    { 0 , 0 , 0 , 0 , 0 }
};

Point currPos;

void runSimulation(){
    currPos = START_POS;
    pathfind_init(currPos);

    do{

        for(int i= MAZE_WIDTH-1; i >= 0; i--){
            printf(" %d |", i);
            for(int j = 0; j < MAZE_HEIGHT; j++){
                printf("%c", cmpVec(Vec(i,j), currPos) ? '*' : maze[i][j] == 0 ? ' ' : '#');
            }
            printf("|\n");
        }
        printf("    01234 \n");
  
        
      Vec4f measurements;
      measurements[0] = 0;
      measurements[1] = 0;
      measurements[2] = 0;
      measurements[3] = 0;

      for(int i = 0; i < 4; i++){
        Bool hit_wall = false;
        Point checker = Vec(currPos.x,currPos.y);
        while(hit_wall == false){
          checker = translateOnAxisInDirection(checker, i, 1);
          //checking for out of bounds
          if(!inBound(checker.x, 0, MAZE_WIDTH-1) || !inBound(checker.y, 0, MAZE_HEIGHT-1)){
                hit_wall = true;
          } else if(maze[checker.x][checker.y] == 1){
            hit_wall = true;
          } else {
            measurements[i] = measurements[i]+1;
          }
        }
      }
     
      printVec4f(measurements);
      
      currPos = pathfind_update(currPos, measurements);

       
           
    }while(1);
}


