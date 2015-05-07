#include "./pathfind.h"


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

#define START_POS Vec(0,0)
#define HEADING Vec(4,0)


int maze[MAZE_WIDTH][MAZE_HEIGHT] = {
    { 0 , 0 , 0 , 0 , 0 },
    { 1 , 1 , 1 , 1 , 0 },
    { 1 , 1 , 1 , 1 , 0 },
    { 1 , 1 , 1 , 1 , 0 },
    { 0 , 0 , 0 , 0 , 0 }
};

//Gershon's phone number: 551-795-9634

Point pts[] = {
    Vec(0,0),
    Vec(0,1),
    Vec(0,2),
    Vec(0,3),
    Vec(0,4),
    Vec(1,4),
    Vec(2,4),
    Vec(3,4),
    Vec(4,4),
    Vec(4,3),
    Vec(4,2),
    Vec(4,1),
    Vec(4,0)
};

//int maze[MAZE_WIDTH][MAZE_HEIGHT] = {
//    { 0 },
//    { 0 },
//    { 0 },
//    { 0 },
//    { 0 }
//};


int currPt = 0;
Point currentPosition;




Vec4f collectMeasurements(){
    Vec4f measurements;
    measurements[0] = 0;
    measurements[1] = 0;
    measurements[2] = 0;
    measurements[3] = 0;
    
    for(int i = 0; i < 4; i++){
        Bool hit_wall = false;
        Point checker = Vec(currentPosition.x,currentPosition.y);
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
    /*
    printf("Measurements\n");
    printf(" %d \n",(int)measurements[0]);
    printf("%d %d\n",(int)measurements[3], (int)measurements[1]);
    printf(" %d",(int)measurements[2]);
    
    
    printf("\n------\n");
    */
    return measurements;
}

void runSimulation(){
    
    currentPosition.x = 0;currentPosition.y = 0;
    printPoint(currentPosition);
    
    pathfind_init(currentPosition, HEADING);

    do{
    
    Vec4f measurements = collectMeasurements();
    currPt++;
    currentPosition = pathfind_update(currentPosition, measurements);
           
    } while(!cmpVec(currentPosition, HEADING));

}


