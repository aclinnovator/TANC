//
//  pathfind.c
//
//
//  Created by Akiva Lipshitz on 4/22/15.
//
//

#include "pathfind.h"
#ifdef __APPLE__
#include <stdio.h>
//Because stupid math.h had to give min() and max()
//wierd names.
#define min(n,b) (fmin(n,b))
#define max(n,b) (fmax(n,b))
#endif

//Variables
//Current location
Point location;
//Previous
Point prevLocation;
//Next location to run data analysis from
Point nextLocation;
//Maze endpoint
Point headingLocation;

MazeSquare history[_MAZE_WIDTH][_MAZE_HEIGHT];




#if IS_BLOCKS

/*

      #---#---#---#---#---#
  5   |   |   |   |   |   |
      #---#---#---#---#---#
  4   |   |   |   |   |   |
      #---#---#---#---#---#
  3   |   |   |   |   |   |
      #---#---#---#---#---#
  2   |   |   |   |   |   |
      #---#---#---#---#---#
  1   |   |   |   |   |   |
      #---#---#---#---#---#
        1   2   3   4   5

        5 * 5 = 25in^2
 */
void printPoint(Point pt){ printf("x: %d, y: %d\n",pt.x, pt.y); }
Point pointToArrayIndex(Point p){
    return cutToMazeBounds( p );
}

Point pointToReachableSquare(Point p, int dir){
    Point temp_p = p;
    // if(	(history[temp_p.x][temp_p.y].status != UNEXPLORED) &&
      //  (history[temp_p.x][temp_p.y].classification == FLOOR) ){
        return cutToMazeBounds(temp_p);
    // }
}

int floorsIlluminatedByMeasurement(float measurement){
    return measurement/MOVEMENT_UNIT_SIZE;

}

void storeClassificationWall(Point p){
    history[p.x][p.y].classification = WALL;
}

void storeClassificationFloor(Point p){
    history[p.x][p.y].classification = FLOOR;
}

#elif IS_WALLS

/*

 11   #---#---#---#---#---#
 10   |   |   |   |   |   |
  9   #---#---#---#---#---#
  8   |   |   |   |   |   |
  7   #---#---#---#---#---#
  6   |   |   |   |   |   |
  5   #---#---#---#---#---#
  4   |   |   |   |   |   |
  3   #---#---#---#---#---#
  2   |   |   |   |   |   |
  1   #---#---#---#---#---#
     1 2 3 4 5 6 7 8 9 10 11

    11 * 11 = 121in^2

 */
Point pointToArrayIndex(Point p){
    return cutToMazeBounds( addVec(p,p) );
}

Point pointToReachableSquare(Point p, int dir){

    if(oddOrEven(p.x) == ODD)
        p = translateOnAxisInDirection(p, getAxis(dir), 1);

    if(oddOrEven(p.y) == ODD)
        p = translateOnAxisInDirection(p, getAxis(dir), 1);

    p.x = cut(p.x, 0,1,_MAZE_WIDTH-1);
    p.y = cut(p.y, 0,1,_MAZE_HEIGHT-1);

    return cutToMazeBounds(p);
}

int floorsIlluminatedByMeasurement(float measurement){
    return measurement/MOVEMENT_UNIT_SIZE;
}

void storeClassificationWall(Point p){
    dir = getVec(p, location);

    if(oddOrEven(p.x) == ODD) {

        Point before, after;

        if ((p.x == location.x) && (absv(p.y, location.y) == 1)){

            before = translateOnAxisInDirection(p, 0, 1);
            after  = translateOnAxisInDirection(p, 2, 1);

        } else if ((p.y == location.y ) && (absv(p.x, location.x)==1) ){
            before = translateOnAxisInDirection(p, 1, 1);
            after  = translateOnAxisInDirection(p, 3, 1);
        }

        history[p.x][p.y].classification = WALL;
        history[before.x][before.y].classification = WALL;
        history[after.x][after.y].classification   = WALL;
    }
}

void storeClassificationFloor(Point p){
    if(oddOrEven(p.x) == EVEN)
        history[p.x][p.y].classification = FLOOR;
}


#endif //Maze_Type == ?

//#####################################################
int pointInIndBounds(Point p)
{ return inBoundsIndX(p.x) && inBoundsIndY(p.y); }

int cut(int n, int mag, int _min, int _max){
    int res;

#ifdef __APPLE__
    if (!inBound(n, _min, _max)) {

//        printf("%d out of bounds for %d and %d\n",n,_min,_max);
//        exit(1);
    }
#endif

    if       (n+mag < _min){ res =  _min; }
    else if  (n+mag > _max){ res = _max;  }
    else                   { res =  n  + mag; }

    return res;
}

Point cutToMazeBounds(Point n){
    n.x = cut(n.x, 0, 0, _MAZE_WIDTH -1);
    n.y = cut(n.y, 0, 0, _MAZE_HEIGHT-1);
    return n;
}

int absv(int n){
    return (n < 0 ? -n : n);
}

Point Vec(int x, int y){
    Point p; p.x = x; p.y = y;
  return p;
}

Point getVec(Point a, Point b){
    return subVec(a, b);
}

Point ceilVec(Point vec){
    int absX = absv(vec.x);
    int absY = absv(vec.y);
    if (absX != 0)vec.x /= absX;
    if (absY != 0)vec.y /= absY;

    return vec;
}

Point addVec(Point a, Point b){
    a.x += b.x;
    a.y += b.y;
    return a;
}

Point subVec(Point a, Point b){
    b.x -= a.x;
    b.y -= a.y;

    return b;
}

Point multVec(Point a, Point b){
    return Vec(a.x * b.x, a.y * b.y);
}

Point perpVec(Point a, Point b){
    return multVec(getVec(a,b), Vec(1,-1));
}

Point absVec(Point p){
    p.x = absv(p.x);
    p.y = absv(p.y);
    return p;
}

Bool cmpVec(Point n, Point l){
  Bool a = n.x == l.x, b = n.y == l.y;
  return a && b;
}

Point translateOnAxisInDirection(Point start, int dir, int magnitude){
    int axis = getAxis(dir);
    int axial_direction = (dir == 1) || (dir == 0) ? 1 : -1;

    if      (axis == X_AXIS) { start.x += (magnitude * axial_direction); }
    else if (axis == Y_AXIS) { start.y += (magnitude * axial_direction); }
    start = start; //pointToReachableSquare(start, dir);
    return start;
}

Vec4 collectDistancesToWalls(){
    Vec4 measurements;
    measurements[0] = 0;
    measurements[1] = 0;
    measurements[2] = 0;
    measurements[3] = 0;
    
    for(int i = 0; i < 4; i++){
        Bool hit_wall = false;
        Point checker = Vec(location.x,location.y);
        while(hit_wall == false){
            checker = translateOnAxisInDirection(checker, i, 1);
            //checking for out of bounds
            if(!pointInIndBounds(checker)){
                hit_wall = true;
            } else if(history[checker.x][checker.y].classification == WALL){
                hit_wall = true;
            } else {
                measurements[i] = measurements[i]+1;
            }
        }
    }
    return measurements;
}

Point axialMagnitudeNormalized(Point a, Point b){
    return ceilVec( getVec(a, b) );
}

int pointsOnAxisInDirectionUntil(
                      Point p,
                      PointFunction checker,
                      int direction) {

    int ctr = 0;
    while (pointInIndBounds(p)) {
        if (checker(location, p)) return ctr;
        p = translateOnAxisInDirection(p, direction, 1);
        ctr++;
    }
    return ctr;

}

int wallOrFloor(float mes){
//    printf("Measurement:%f, Square Size:%d\n",mes, SQUARE_SIZE_INCH);
    return (mes >= SQUARE_SIZE_INCH ? FLOOR : WALL);
}

Vec4 classifyMeasurements(Vec4f mesments){
    for (int i = 0; i < 4; i++){
        mesments[i] = wallOrFloor(mesments[i]);
    }

    Vec4 ret;

    for (int i=0; i < 4;i++){
      ret[i] = mesments[i];
    }

    return ret;

}

Bool storeClassification(Point p, int classification) {
    p = cutToMazeBounds(p);
    if(inBound(classification,FLOOR,WALL)){
    if (classification == WALL){
        storeClassificationWall(p);
    } else if (classification == FLOOR){
        storeClassificationFloor(p);
    }
        return true;
    }
    return false;
}

Bool storeStatus(Point p, int status){
    p = cutToMazeBounds(p);
    if (inBound(status,UNEXPLORED,DEAD_END)){
        history[p.x][p.y].status = status;
        return true;
    }
    return false;
}

Bool stopAtWall(Point start, Point current){
    //Will we need to check for explored or unexplored.
    Bool ret = history[current.x][current.y].classification == WALL ? true : false;
    return ret;
}

Bool stopAtPerpendicularPassage(Point start, Point current) {
    Point perpA, perpB;
    Bool ret = history[current.x][current.y].classification == WALL ? true : false;
    return ret;
}

int relationship(int a, int b){
    if (a < b) return LESS_THAN;
    if (a > b) return MORE_THAN;
    return EQUAL;
}

//Edit this function
//TODO: Edit comparePathOptions.
//NOTE: Something
int comparePathOptions(int n_explored, int n_closeness, int n_length,
                 int b_explored, int b_closeness, int b_length){

    int objs[][3] = {
        {n_explored, n_closeness, n_length},
        {b_explored, b_closeness, b_length}
    };

    for (int i = 0; i < 3; i++){
        int rel = relationship(objs[1][i],objs[0][i]);
        if (rel!= EQUAL ) return rel;

    }
    return EQUAL;
}

//It is very important to edit this

Point  getDesLocation(Point _curr_pos){
#define TRANS(dir) translateOnAxisInDirection(_curr_pos, dir, 1)

    Vec4 statuses,
        distances ,
        lengths = collectDistancesToWalls(),
        availablities = {POINT_REACHABLE, POINT_REACHABLE, POINT_REACHABLE, POINT_REACHABLE},
        directions = {0,1,2,3};
    
    //The points in all four directions
        Point n = TRANS(0),
        s = TRANS(2),
        w = TRANS(3),
        e = TRANS(1);
    //For easy access by index
    Point *pts[] = {&n, &e, &s, &w};

    //Give directions a length rating and distance rating and then sorts the directions, leaving the best direction in the front.
    //If we discover that we have already been in a location, we should block off the passage behind us leading to that location because it must be a loop.
    
    //Collect data for each direction:
    for (int i=0; i < 4; i++) {
        Point direction_translated = *pts[i];
        Point heading_direction = pointToReachableSquare(direction_translated , i);
        
        Point vec_to_end = subVec(heading_direction, headingLocation);
        //if the path does not exist, then delete it - mark as DEAD_END
        distances[i]    =  min(absv(vec_to_end.x), absv(vec_to_end.y));
        statuses[i]     =  history[heading_direction.x][heading_direction.y].status;
        
        if (cmpVec(heading_direction, _curr_pos) || history[heading_direction.x][heading_direction.y].classification == WALL || !pointInIndBounds(direction_translated)) {
            availablities[i] = POINT_UNREACHABLE;
        }
    }
    
    Bool surrounded_by_explored =
        history[n.x][n.y].status == EXPLORED   &&
        history[s.x][s.y].status == EXPLORED   &&
        history[w.x][w.y].status == EXPLORED   &&
        history[e.x][e.y].status == EXPLORED;

    int num_pass_available = 0;
        num_pass_available += history[n.x][n.y].classification == WALL;
        num_pass_available += history[s.x][s.y].classification == WALL;
        num_pass_available += history[w.x][w.y].classification == WALL;
        num_pass_available += history[e.x][e.y].classification == WALL;
    
    Bool hit_dead_end = num_pass_available == 3;
    
    //If we have explored in every direction,
    //then find the nearest unexplored postion,
    //and set it as deslocation
    if (surrounded_by_explored) {
        
    } else if (hit_dead_end){
        
    }
    //Normal circumstances
    //Insertion sort the distances.
    for (int i = 1; i < 4; i++) {
        int j = i - 1;
        while ( (j >= 0) &&
               comparePathOptions(
                                  statuses[  directions[i] ],
                                  distances[ directions[i] ],
                                  lengths[   directions[i] ],
                                  statuses[  directions[j] ],
                                  distances[ directions[j] ],
                                  lengths[   directions[j] ]
                                  ) == MORE_THAN ) {
                   int temp = directions[i];
                   directions[i] = directions[j];
                   directions[j] = temp;
                   j--;
               }
    }
    //Get the first available endpoint
    int ind = 0;
    int dir = directions[ind];
    while (availablities[ directions[ind] ] == POINT_UNREACHABLE && (ind<4)) {
        dir = directions[ind+1];
        ind++;
    }
    return TRANS(dir);



#undef TRANS
}

Point one_square_closer_to_next_location(Point _curr_pos, Point next_location){
  //Right now we are assuming next location is on the same x or x
  //as the car.

  return addVec( _curr_pos, axialMagnitudeNormalized(next_location, _curr_pos)  );
}

Point where_the_hech_should_I_go(Point whereIAmLocated){
    Point loc = whereIAmLocated;
    Point best_loc = getDesLocation(loc);
    
//    Point to_go = pointToReachableSquare( translateOnAxisInDirection(location, best_dir, 1), best_dir );
    return best_loc;
}


void save(Point currPos, Vec4f ultraSonicData){
    prevLocation = location;
    location = currPos;

    storeStatus(currPos, EXPLORED);

    Vec4 classifications = classifyMeasurements(ultraSonicData);
    
    Point position;
    for (int i = 0; i < 4; i++){
        position = cutToMazeBounds( translateOnAxisInDirection(location, i, 1) );
        if (classifications[i] == WALL && !cmpVec(location, position)){
            /*if car is in corner, pos. will be cut to its position*/
            storeClassification(position, WALL);
        } else if (classifications[i] == FLOOR){
            int doUntil = floorsIlluminatedByMeasurement(ultraSonicData[i]);

            for(int j = 1; j <= doUntil; j++){
                Point store_loc = translateOnAxisInDirection(location, i, j);
                storeClassification(store_loc, FLOOR );
            }
        }
    }
}

Point pathfind_update(Point _curr_pos, Vec4f measurements){

  save(_curr_pos, measurements);
  pathfind_print();

    
  if (cmpVec(_curr_pos, nextLocation)){
     nextLocation = where_the_hech_should_I_go(_curr_pos);
  }
  
//  Point temp =  one_square_closer_to_next_location(_curr_pos, nextLocation);
    
  return nextLocation;
    
}

void pathfind_init(Point start, Point heading){
    printf("H: %d, W:%d, Ss:%d\n", MAZE_HEIGHT, MAZE_WIDTH, SQUARE_SIZE_INCH);

  location = start;
  nextLocation = location;
  headingLocation = heading;
  for (int i=0; i < _MAZE_WIDTH; i++){
    for(int j=0; j < _MAZE_HEIGHT; j++){
      Point p = Vec(i, j);
      storeClassification(p, WALL);
      storeStatus(p, UNEXPLORED);
    }
  }
    storeClassification(location, FLOOR);
    storeStatus(location, EXPLORED);
}

void pathfind_finish(){
    location = headingLocation;
}

#define CAR_C '*'
#define WALL_C '#'
#define FLOOR_C ' '
#define UNEX_C  ':'

void pathfind_print(void){
    
    for(int height = MAZE_HEIGHT -1; height >= 0;height--){
        printf("%d|",height);
        for(int width = 0; width < MAZE_WIDTH; width++){
            
            if (cmpVec(Vec(width,height), location)) {
                printf("%c",CAR_C);
            } else if(history[width][height].status == UNEXPLORED){
                
                switch (history[width][height].classification) {
                    case FLOOR:
                        printf("%c",UNEX_C);
                        break;
                    case WALL:
                        printf("%c",WALL_C);
                    default:
                        break;
                }
            }else if(history[width][height].status == EXPLORED){
                if (history[width][height].classification == FLOOR) {
                    printf("%c",FLOOR_C);
                } else {
                    printf("%c",WALL_C);
                }
            }
        }
        printf("|\n");
    }
    printf("  01234 \n");
}
