//
//  pathfind.c
//
//
//  Created by Akiva Lipshitz on 4/22/15.
//
//

#include "tcpathfind.h"


//Variables
//Current location
Point location;
//Previous
Point prevLocation;
//Next location to run data analysis from
Point nextLocation;
//Maze endpoint
Point headingLocation;

MazeSquare hist[MAZE_WIDTH][MAZE_HEIGHT];


#if IS_BLOCKS


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

#elif IS_WALLS


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

#endif //Maze_Type == ?


#define GET_AROUND(name, func, equalsWhat, comp, statusOfPoint) \
    Vec4 numSurrounding##name(Point p) { \
        Vec4 ret;    \
        Point \
        n = translateOnAxisInDirection(p, 0, 1), \
        w = translateOnAxisInDirection(p, 3, 1),e = translateOnAxisInDirection(p, 1, 1), \
        s = translateOnAxisInDirection(p, 2, 1) \
        ;\
        ret[0] = (func(n) == equalsWhat)comp statusOfPoint(n), \
        ret[1] = (func(s) == equalsWhat)comp statusOfPoint(s), \
        ret[2] = (func(w) == equalsWhat)comp statusOfPoint(w), \
        ret[3] = (func(e) == equalsWhat)comp statusOfPoint(e); \
    return ret; \
}

GET_AROUND(walls, classification, WALL, ||, !pointInIndBounds)
GET_AROUND(pathways, classification, FLOOR, &&, pointInIndBounds)
GET_AROUND(explored_pathways, status, EXPLORED, &&, pointInIndBounds)

#undef GET_AROUND


Bool deadEnd(Point p, Point prev_loc, Vec4 walls){
    
    int bn = walls[0],
        be = walls[1],
        bs = walls[2],
        bw = walls[3],
        num_walls = bn+be+bs+bw;
    
    Point stc = ceilVec( getVec(prev_loc, p) );
    
    Bool blocked_pathways_are_ahead = ((stc.y==-1)&&bs) || ((stc.y== 1) &&bn) ||
    ((stc.x== 1)&&be) || ((stc.x==-1) &&bw);
    Bool hit_dead_end = num_walls == 3 && blocked_pathways_are_ahead; //none of the blocked pathways are in the vector going from prevPos to currPos and prevPos!=currPos;
    return hit_dead_end;

}


Bool stopAtWall(Point start, Point current){
    //Will we need to check for explored or unexplored.
    return classification(current) == WALL;
}

Bool stopAtCurrentLocation(Point start, Point current){
    return stopAtWall(start, current) || cmpVec(current, location);
}

Bool stopAtPerpendicularPassage(Point start, Point current) {
    Point vec = ceilVec( getVec(start, current));
    Point perp_vec = perpVec(vec);
    
    Point perpA = addVec(current, perp_vec);
    Point perpB = addVec(current, multVec(perp_vec, Vec(-1,-1)));
    
    Bool isSame = cmpVec(current, start), ret = (!isSame)&& (
                                                            ((classification(perpA) == FLOOR) || (classification(perpB) == FLOOR)) && (classification(start) == FLOOR));
    
    return ret;
}

Bool stopAtUnExplored(Point start, Point current){
    return status(current) == UNEXPLORED;
}

Bool stopAtUnExploredAndNotWall(Point start, Point current){
    return (status(start) == UNEXPLORED) && (classification(start)==FLOOR);
}

Vec4 collectDistancesToWalls(Point start){
    Vec4 measurements;
    measurements[0] = pointsOnAxisInDirectionUntil(start, stopAtWall, 0, 1),
    measurements[1] = pointsOnAxisInDirectionUntil(start, stopAtWall, 1, 1),
    measurements[2] = pointsOnAxisInDirectionUntil(start, stopAtWall, 2, 1),
    measurements[3] = pointsOnAxisInDirectionUntil(start, stopAtWall, 3, 1);
   return measurements;
}

Vec4 collectDistancesToUnexplored(Point start){
    Vec4 measurements;
    measurements[0] = pointsOnAxisInDirectionUntilWithStopChecker(start, stopAtUnExplored, stopAtWall, 0, 1,0),
    measurements[1] = pointsOnAxisInDirectionUntilWithStopChecker(start, stopAtUnExplored, stopAtWall, 1, 1,0),
    measurements[2] = pointsOnAxisInDirectionUntilWithStopChecker(start, stopAtUnExplored, stopAtWall, 2, 1,0),
    measurements[3] = pointsOnAxisInDirectionUntilWithStopChecker(start, stopAtUnExplored, stopAtWall, 3, 1,0);
    return measurements;
}

Vec4 collectDistancesToPerpendicular(Point start){
    Vec4 measurements;
    measurements[0] = pointsOnAxisInDirectionUntilWithStopChecker(start, stopAtPerpendicularPassage, stopAtWall, 0, 1,0),
    measurements[1] = pointsOnAxisInDirectionUntilWithStopChecker(start, stopAtPerpendicularPassage, stopAtWall, 1, 1,0),
    measurements[2] = pointsOnAxisInDirectionUntilWithStopChecker(start, stopAtPerpendicularPassage, stopAtWall, 2, 1,0),
    measurements[3] = pointsOnAxisInDirectionUntilWithStopChecker(start, stopAtPerpendicularPassage, stopAtWall, 3, 1,0);
    return measurements;
}



Point nearestUnexplored(Point start, Point comingFrom) {
    
    //if it hits a dead end
    //if it satisfies checker
    //Then return
    
    Bool adjacent =((start.x==location.x)||(start.y == location.y)),
        same_vec = cmpVec( ceilVec(  getVec(comingFrom, start)), ceilVec( getVec(location, start)) ),
        same_point = cmpVec(start, location);
    
    if (classification(start) == UNEXPLORED) return start;
    else if(deadEnd(start, comingFrom, numSurroundingwalls(start)) ||
        ((adjacent&&!same_vec)  && !same_point)   ) {
            return Vec(NULL, NULL);
        }
    
    Vec4 toUnexplored = collectDistancesToUnexplored(start);
    Vec4 toPerp = collectDistancesToPerpendicular(start);

    Vec4 points_available = {POINT_REACHABLE, POINT_REACHABLE, POINT_REACHABLE, POINT_REACHABLE};
    Vec4 directions = {0,1,2,3};
    int min = toUnexplored[0];
    int mindir = 0;
    //Collecting the fruitful directions
    int numfruitful=0;
    
    Point vec_to_start = ceilVec(getVec(start, comingFrom));
    
    for (int i=0; i<4; i++) {
        int direction_came_from = cmpVec(dirVec(i),vec_to_start);
        if(toUnexplored[i]>0) {
            numfruitful++, min = toUnexplored[i] < min ? toUnexplored[i] : min, mindir = i;
        }
        Point trans = translateOnAxisInDirection(start, i, 1) ;
        if ((toPerp[i] ==0) || (classification( trans ) ==WALL) || direction_came_from)
            points_available[i] = POINT_UNREACHABLE;
    }
    
    
    
    //If there are no immediately surrounding nearest point, then must traverse
    //Depth-first search works best, because there is high possibility of overlap,
    //and you are starting from the assumed best path.
    //Depth first just wastes time searching worser paths.
    if (numfruitful==0) {
        
        
        //See index card for guidlines. Implement A* search algorithm here.
        for (int i = 1; i < 4; i++) {
            int j = i - 1;
            while (
                   (j >= 0) && toPerp[ directions[i] ] > toPerp[ directions[j]]) {
                
                int temp = directions[i];
                directions[i] = directions[j];
                directions[j] = temp;
                j--;
            }
        }
        
        for (int i=0; i<4; i++) {
            int direction = directions[i];
            if (points_available[direction]==POINT_REACHABLE) {
                
                Point vec = ceilVec( getVec(comingFrom, start));
                Point perp_vec = perpVec(vec);
                Point translated = translateOnAxisInDirection(start, direction, toPerp[direction]);

                //if the perp is fruitful, return that location.
                //If it is not fruitful, then call this function a second time.
                Point perpA = addVec(translated, perp_vec), perpB =addVec(translated, multVec(perp_vec, Vec(-1,-1)));

                if ((status(perpA)==UNEXPLORED) && (classification(perpA)==FLOOR)) {
                    return perpA;
                } else if ((status(perpB)==UNEXPLORED) &&(classification(perpB)==FLOOR)) { return perpB; }
                else
                {
                    Point vec = nearestUnexplored(translated, start);
                    if (!isnullVec(vec)) {
                        return vec;
                    }
                }
                
            }

        }
        
    } else if (numfruitful>0){
        return translateOnAxisInDirection(start, mindir, toUnexplored[mindir]);
    }
    
    return nullVec();
}


int wallOrFloor(float mes){
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

int relationship(int a, int b){
    if (a < b) return LESS_THAN;
    if (a > b) return MORE_THAN;
    return EQUAL;
}

Point dirVec(int dir){
    if (!inBound(dir, 0, 3)) return Vec(0, 0);
    Point vecs[] = {
        Vec(0, 1),
        Vec(1, 0),
        Vec(0, -1),
        Vec(-1, 0)
    };
    return vecs[dir];
}

int relationshipDirection(int dirA, int dirB, Point currLoc, Point desLoc){

    if (dirA == dirB) return dirA;
    
    Point vec = getVec(currLoc, desLoc);
    Point ceil_vec = ceilVec(vec);
    Point dir_vec_a = dirVec(dirA);
    Point dir_vec_b = dirVec(dirB);
    

    if (cmpVec(dir_vec_a,ceil_vec) && !cmpVec(dir_vec_b, ceil_vec) ) {
        return LESS_THAN;
    } else if (!cmpVec(dir_vec_a,ceil_vec) && cmpVec(dir_vec_b, ceil_vec) ) {
        return MORE_THAN;
    } else {
        int distA = pointsOnAxisInDirectionUntil(currLoc, stopAtWall, dirA,1);
        int distB = pointsOnAxisInDirectionUntil(currLoc, stopAtWall, dirB,1);
        
        if (distA < distB){
            return LESS_THAN;
        }
    }

    return MORE_THAN;
}

int betterDirection(int dirA, int dirB, Point currLoc, Point desLoc){
    int rel = relationshipDirection(dirA, dirB, currLoc, desLoc);
    //The lower direction always wins. 
    if (rel == LESS_THAN) return dirA;
    else if (rel == MORE_THAN) return dirB;
    return dirA;
}

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

//Give directions a length rating and distance rating and then sorts the directions, leaving the best direction in the front.
//If we discover that we have already been in a location, we should block off the passage behind us leading to that location because it must be a loop.
Point  getDesLocation(Point _curr_pos){
#define TRANS(dir) translateOnAxisInDirection(_curr_pos, dir, 1)
#define _(dir) = TRANS(dir)
    
    //The points in all four directions
    
                Point
              n _(0),
           w _(3),e _(1),
              s _(2)
                  ;
    
    Vec4 cs/* = classification */ = numSurroundingwalls(_curr_pos), pw /* = pathway */ = numSurroundingpathways(_curr_pos);
    int num_walls = sumVec4(cs), num_pathways = sumVec4(pw), num_explored_pathways = sumVec4(numSurroundingexplored_pathways(_curr_pos));
    
    
    //none of the blocked pathways are in the vector going from prevPos to currPos and prevPos!=currPos;

    Bool hit_dead_end = deadEnd(_curr_pos, prevLocation, cs) && !cmpVec(prevLocation, _curr_pos);
    Bool blocked_in_by_wall = num_walls == 4;
    
    
    //If we have explored in every direction,
    //then find the nearest unexplored postion,
    //and set it as deslocation
    if (num_explored_pathways == num_pathways) {
        printf("SURROUNDED BY EXPLORED");
        return nearestUnexplored(_curr_pos, _curr_pos);
//        exit(EXIT_SUCCESS);
    } else if (hit_dead_end){
        printf("DEAD END");
        exit(EXIT_SUCCESS);
    } else if (blocked_in_by_wall){
        printf("BLOCKED IN BY WALL");
        exit(EXIT_SUCCESS);

    }
    
    Vec4 statuses,
    distances ,
    lengths = collectDistancesToWalls(location),
    availablities = {POINT_REACHABLE, POINT_REACHABLE, POINT_REACHABLE, POINT_REACHABLE},
    directions = {0,1,2,3};
    
    //For easy access by index
    Point *pts[] = {&n, &e, &s, &w};
    

    //Collect data for each direction:
    for (int i=0; i < 4; i++) {
        Point direction_translated = *pts[i];
        Point heading_direction = pointToReachableSquare(direction_translated , i);
        
        Point vec_to_end = subVec(heading_direction, headingLocation);
        //if the path does not exist, then delete it - mark as DEAD_END
        distances[i]    =  min(absv(vec_to_end.x), absv(vec_to_end.y));
        statuses[i]     =  status(heading_direction);
        
        if (
            cmpVec(heading_direction, _curr_pos)      ||
            classification(heading_direction) == WALL ||
            !pointInIndBounds(direction_translated)
            ){
            
            availablities[i] = POINT_UNREACHABLE;
        }
    }

    //Normal circumstances
    //Insertion sort the distances.
    for (int i = 1; i < 4; i++) {
        int j = i - 1;
        while (
               (j >= 0) &&
               comparePathOptions(
                                  statuses [ directions[i] ],
                                  distances[ directions[i] ],
                                  lengths  [ directions[i] ],
                                  statuses [ directions[j] ],
                                  distances[ directions[j] ],
                                  lengths  [ directions[j] ]
                                  ) == MORE_THAN
               ) {
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
#undef _
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
            } else if(status(Vec(width, height)) == UNEXPLORED){
                
                switch (classification(Vec(width, height))) {
                    case FLOOR:
                        printf("%c",UNEX_C);
                        break;
                    case WALL:
                        printf("%c",WALL_C);
                    default:
                        break;
                }
            }else if(status(Vec(width, height)) == EXPLORED){
                if (classification(Vec(width, height)) == FLOOR) {
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
