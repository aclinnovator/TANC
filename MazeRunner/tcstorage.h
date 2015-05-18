//
//  tcstorage.h
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/14/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#ifndef __MazeRunner__tcstorage__
#define __MazeRunner__tcstorage__

#include "tcpoint.h"
#include "tctypes.h"
//Status: EXPLORED, UNEXPLORED, DEAD_END
//Classification: WALL, FLOOR
typedef struct {
    unsigned short status         : 3; //Because it goes up to 3, not 2
    unsigned short classification : 1; // WALL or FLOOR
    unsigned short onqueue: 1;
}MazeSquare;

/*--------( Storage Functions )-------*/

void storeClassificationWall(Point p);
void storeClassificationFloor(Point p);
Bool storeClassification(Point p, int classification);
Bool storeStatus(Point p, int status);

Bool addToQueue(Point p);
    Bool removeFromQueue(Point p);

void save(Point currPos, Vec4 ultraSonicData);

/*-------( Retrieval )-------*/
int classification(Point p);
int status(Point p);

Bool onQueue(Point p);


#endif /* defined(__MazeRunner__tcstorage__) */
