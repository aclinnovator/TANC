//
//  vec.c
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/13/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#include "tcpoint.h"
#include "stdio.h"

const Point _nullVec = {NULL, NULL};

Point Vec(int x, int y){
    Point p; p.x = x; p.y = y;
    return p;
}

Point getVec(Point a, Point b){
    return subVec(a, b);
}

Point nullVec(){
    return _nullVec;
}

Bool isnullVec(Point vec){
    return cmpVec(vec, nullVec());
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

Point perpVec(Point vec){
    Point ret = Vec(vec.y,vec.x);
    return ret;
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


void printPoint(Point pt) { printf("x: %d, y: %d\n",pt.x, pt.y); }