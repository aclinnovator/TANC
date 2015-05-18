//
//  tcstorage.cpp
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/14/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#include "tcstorage.h"
#include "tcconstants.h"
#include "tcmovement.h"


//This structure is used to store the information collected
// for one single square in a maze
//Sometime make this a const, and use a conditionally expanding macro
//to populate it, so you will use less memory?
MazeSquare history[_MAZE_WIDTH][_MAZE_HEIGHT];

#if IS_BLOCKS

void storeClassificationWall(Point p){
    history[p.x][p.y].classification = WALL;
}

void storeClassificationFloor(Point p){
    history[p.x][p.y].classification = FLOOR;
}

#elif IS_WALLS

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

int classification(Point p) {
    p = cutToMazeBounds(p);
    return history[p.x][p.y].classification;
}

int status(Point p){
    p = cutToMazeBounds(p);
    return history[p.x][p.y].status;
}

MazeSquare (***history_arr()){
    return (MazeSquare ***)(&history);
}