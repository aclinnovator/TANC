//
//  tcmovement.c
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/14/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#include "tcmovement.h"
#include "tcpoint.h"
#include "tcconstants.h"


//#####################################################
int pointInIndBounds(Point p)
{ return inBoundsIndX(p.x) && inBoundsIndY(p.y); }

Point cutToMazeBounds(Point n){
    n.x = cut(n.x, 0, 0, _MAZE_WIDTH -1);
    n.y = cut(n.y, 0, 0, _MAZE_HEIGHT-1);
    return n;
}

Point translateOnAxisInDirection(Point start, int dir, int magnitude){
    int axis = getAxis(dir);
    int axial_direction = (dir == 1) || (dir == 0) ? 1 : -1;
    
    if      (axis == X_AXIS) { start.x += (magnitude * axial_direction); }
    else if (axis == Y_AXIS) { start.y += (magnitude * axial_direction); }
    start = start; //pointToReachableSquare(start, dir);
    return start;
}
Point axialMagnitudeNormalized(Point a, Point b){
    return ceilVec( getVec(a, b) );
}

int pointsOnAxisInDirectionUntil(
                                 Point p,
                                 PointFunction checker,
                                 int direction,
                                 int axial_magnitude) {
    
    int ctr = 0;
    Point st = Vec(p.x,p.y);
    while (pointInIndBounds(p)) {
        if (checker(st, p)) return ctr;
        p = translateOnAxisInDirection(p, direction, axial_magnitude);
        ctr++;
    }
    return 0;
    
}