//
//  vec.h
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/13/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#ifndef __MazeRunner__vec__
#define __MazeRunner__vec__

#include <stdio.h>
#include "tcmath.h"
#endif /* defined(__MazeRunner__vec__) */

/*--------( Vector functions )-------*/

//Representing one single point
typedef struct {
    int x;
    int y;
} Point;

Point Vec(int x, int y);

Bool cmpVec(Point n, Point l);

Point getVec(Point a, Point b);

Point ceilVec(Point vec);

Point addVec(Point a, Point b);

Point subVec(Point a, Point b);

Point multVec(Point a, Point b);

Point perpVec(Point a, Point b);

Point absVec(Point a);