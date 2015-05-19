#include "tcpathfind.h"


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

#define START_POS Vec(3,2)
#define HEADING Vec(1,2)
#define X 1

#define MAZE int maze[MAZE_WIDTH][MAZE_HEIGHT] = {
#define END };
#define _(n) {
#define E }

MAZE
    _(0) X,X,X,X,0,X,0 E,
    _(1) 0,0,0,X,0,X,0 E,
    _(2) 0,X,X,X,0,X,0 E,
    _(3) 0,X,0,0,0,X,0 E,
    _(4) 0,X,X,0,X,X,0 E,
    _(5) 0,0,0,0,0,0,0 E,
    //   0 1 2 3 4 5 6 7
END

/* This proves that our algorithm can solve any looping maze:
 x: 0, y: 0
 H: 7, W:7, Ss:1

 6|#######|
 5|#######|
 4|#######|
 3|#######|
 2|#######|
 1|#######|
 0| *:::::|
   0123456
 ...
 6|#######|
 5|#######|
 4|##   ##|
 3|## # ##|
 2|##*: ##|
 1|#### ##|
 0|     ::|
   01234
 6|#######|
 5|#######|
 4|##   ##|
 3|## # ##|
 2|## * ##|
 1|#### ##|
 0|     ::|
 01234
 SURROUNDED BY EXPLORED
 ...
 6|#######|
 5|#######|
 4|##   ##|
 3|## # ##|
 2|##   ##|
 1|#### ##|
 0|    *::|
 01234
 ...
 6|:*     |
 5|###### |
 4|##   # |
 3|## # # |
 2|##   # |
 1|#### # |
 0|       |
 01234 
 6|*      |
 5|###### |
 4|##   # |
 3|## # # |
 2|##   # |
 1|#### # |
 0|       |
 01234
 
*/


#undef E
#undef _
#undef MAZE
#undef END

//Gershon's phone number: 551-795-9634

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
    
    currentPosition = START_POS;
    printPoint(currentPosition);
    
    pathfind_init(currentPosition, HEADING);

    do{
    
    Vec4f measurements = collectMeasurements();
    currentPosition = pathfind_update(currentPosition, measurements);
           
    } while(!cmpVec(currentPosition, HEADING));

    pathfind_finish();
    pathfind_print();
#ifdef __APPLE__
    printf("FINISHED. MAZE SOLVED\n");
#endif

}


