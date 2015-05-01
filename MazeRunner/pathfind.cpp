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
  Point p = {x, y};
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

    return start;
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
    printf("Measurement:%f, Square Size:%d\n",mes, SQUARE_SIZE_INCH);
    return (mes > SQUARE_SIZE_INCH ? FLOOR : WALL);
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
    if (inBound(status,EXPLORED,DEAD_END)){
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

int relationship(int a, int b){
    if (a < b) return LESS_THAN;
    if (a > b) return MORE_THAN;
    return EQUAL;
}

int comparePathOptions(int n_length, int n_closeness, int n_explored,
                 int b_length, int b_closeness, int b_explored){

    int objs[][3] = {
        {n_explored, n_length, n_closeness},
        {b_explored, b_length, b_closeness}
    };

    for (int i = 0; i < 3; i++){
        int rel = relationship(objs[0][i],objs[1][i]);
        if ((rel == MORE_THAN )||(rel == LESS_THAN) ) return rel;

    }
    return EQUAL;
}

//It is very important to edit this

Vec4 rateDirections(Point _curr_pos){
    Vec4 lengths, distances, statuses, directions = {0,1,2,3};

    //Give directions a length rating and distance rating and then sorts the directions, leaving the best direction in the front.

    //If we discover that we have already been in a location, we should block off the passage behind us leading to that location because it must be a loop.
    for (int i=0; i < 4; i++) {
        distances[i]    = 1;//pointsOnAxisInDirectionUntil(location, stopAtWall, i);
        Point heading_direction = pointToReachableSquare( translateOnAxisInDirection(_curr_pos, i,1),i ); //getVec(location, headingLocation);
        lengths[i]      = min(heading_direction.x, heading_direction.y);
        statuses[i]     = history[heading_direction.x][heading_direction.y].status;
    }

    int j, temp;

    //Insertion sort the distances.
    for (int i = 1; i < 4; i++) {
        temp = directions[i];
        j = i - 1;
        while (
            comparePathOptions(
            statuses[j],
            distances[j],
            lengths[j],
            statuses[i],
            distances[i],
            lengths[i]
               ) == LESS_THAN && (j >= 0)) {

                directions[j + 1] = directions[j];
                j = j - 1;
        }
        directions[j + 1] = temp;
    }

    return directions;
}

Point one_square_closer_to_next_location(Point _curr_pos, Point next_location){
  //Right now we are assuming next location is on the same x or x
  //as the car.

  return addVec( _curr_pos, axialMagnitudeNormalized(next_location, _curr_pos)  );
}

Point where_the_hech_should_I_go(Point whereIAmLocated){
    Point loc = whereIAmLocated;
    Vec4 distances = rateDirections(loc);
    
    Point to_go = pointToReachableSquare( translateOnAxisInDirection(location, distances[0], 1), distances[0] );//pointsOnAxisInDirectionUntil(location, stopAtWall, distances[0]);
    return to_go;
}

void save(Point currPos, Vec4f ultraSonicData){
    prevLocation = location;
    location = currPos;

    storeStatus(currPos, EXPLORED);

    Vec4 classifications = classifyMeasurements(ultraSonicData);
    printf("Classifications\n");
    for(int i=0;i<4;i++) { printf("%d:",classifications[i]); }
    printf("\n------\n");
    
    
    
    Point position;
    for (int i = 0; i < 4; i++){
        position = cutToMazeBounds( translateOnAxisInDirection(location, i, 1) );
        printf("Position: %d, %d\n",position.x, position.y);
        if (classifications[i] == WALL){
            storeClassification(position, WALL);
        } else if (classifications[i] == FLOOR){
            int doUntil = floorsIlluminatedByMeasurement(ultraSonicData[i]);

            for(int j = 1; j <= doUntil; j++){
                storeClassification(
                                    translateOnAxisInDirection(location, i, j),
                                    FLOOR );
            }
        }
    }
    
}

Point pathfind_update(Point _curr_pos, Vec4f measurements){

  save(_curr_pos, measurements);
  
    
  if (cmpVec(_curr_pos, nextLocation)){
     nextLocation = where_the_hech_should_I_go(_curr_pos);
  }
  
//  Point temp =  one_square_closer_to_next_location(_curr_pos, nextLocation);
    
  return nextLocation;
    
}

void pathfind_init(Point start){
    printf("H: %d, W:%d, Ss:%d\n", MAZE_HEIGHT, MAZE_WIDTH, SQUARE_SIZE_INCH);

  location = start;
  nextLocation = location;
  for (int i=0; i < _MAZE_WIDTH; i++){
    for(int j=0; j < _MAZE_HEIGHT; j++){
      Point p = Vec(i, j);
      storeClassification(p, FLOOR);
      storeStatus(p, UNEXPLORED);
    }
  }
}
