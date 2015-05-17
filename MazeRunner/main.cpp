//
//  main.cpp
//  MazeRunner
//
//  Created by Akiva Lipshitz on 4/30/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#include "simulation.h"

int main(int argc, const char * argv[]) {
        runSimulation();
    return 0;
}

/*
 int main(){
 
 title("Cut");
 //Cut n, mag, min, max
 puts("Cut1: %d", cut(5, 3, 3, 10));
 puts("Cut2: %d", cut(5,-2, 3, 10));
 puts("Cut3: %d", cut(5, 7, 3, 10));
 puts("Cut3: %d", cut(5,-7, 3, 10));
 
 title("Translations");
 Point p;
 p.x = 3, p.y = 4;
 puts("Start p:");
 pPt(p);
 
 puts("North translation");
 pPt(translateOnAxisInDirection(p, NORTH, 4 ));
 puts("South Translation");
 pPt(translateOnAxisInDirection(p, EAST, 2));
 
 title("Normalize Axial Magnitude");
 
 Point p2 = translateOnAxisInDirection(p, SOUTH, 15);
 
 printf("p: ");
 pPt(p);
 printf("p2: ");
 pPt(p2);
 
 printf("translated");
 pPt(axialMagnitudeNormalized(p, p2));
 
 title("Vec4");
 Vec4 v;
 v.vec[0] = 12;
 v.vec[1] = 42;
 v.vec[2] = 100;
 v.vec[3] = 34;
 
 printVec(v);
 
 puts("After changing");
 v[2] = 34;
 printVec(v);
 }
 */
