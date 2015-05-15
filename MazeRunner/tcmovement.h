//
//  tcmovement.h
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/14/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#ifndef __MazeRunner__tcmovement__
#define __MazeRunner__tcmovement__

#include "tcmath.h"
#include "tcpoint.h"

typedef Bool (*PointFunction)(Point, Point);

/*--------( Index Checkers and solvers )-------*/

int pointInIndBounds(Point p);

Point cutToMazeBounds(Point n);

Point translateOnAxisInDirection(Point start, int dir, int magnitude);
Point axialMagnitudeNormalized(Point a, Point b);

int pointsOnAxisInDirectionUntil(
                             Point p,
                             PointFunction checker,
                             int direction,
                             int axial_magnitude);

#endif /* defined(__MazeRunner__tcmovement__) */
