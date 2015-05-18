/*

 .___________.    ___      .__   __.   ______
 |           |   /   \     |  \ |  |  /      |
 `---|  |----`  /  ^  \    |   \|  | |  ,----'
     |  |      /  /_\  \   |  . `  | |  |
     |  |     /  _____  \  |  |\   | |  `----.
     |__|    /__/     \__\ |__| \__|  \______|

 T railblazing A utonomous  N avigating   C ar
   A pathfinding algorithm written by Akiva Lipshitz,
 that controls a AUTONOUMOUS NAVIGATING CAR.

 */


#ifndef __MazeRunner__tcpathfind__
#define __MazeRunner__tcpathfind__



#ifdef __APPLE__
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#endif

#include "tcpoint.h"
#include "tcmath.h"
#include "tcconstants.h"
#include "tcmovement.h"
#include "tctypes.h"
#include "tcstorage.h"

//Bitfield:
//See https://msdn.microsoft.com/en-us/library/yszfawxh(v=vs.80).aspx
//This Bitfield only takes up
//(3 + 1) * MAZE_HEIGHT * MAZE_WIDTH bits

//############# Function Declarations ############
void printPoint(Point pt);

/*-------( State specific: WALLS or FLOORS )-------*/
Point pointToArrayIndex(Point p);

Point pointToReachableSquare(Point p, int dir);

/*-------( Classifiers )--------*/
Vec4 classifyMeasurements(Vec4f mesments);
int wallOrFloor(int mes);
int floorsIlluminatedByMeasurement(float measurement);


/*-------( Core Algorithm)-------*/
Bool stopAtWall(Point start, Point current);
Bool stopAtUnExplored(Point start, Point current);
Bool stopAtPerpendicularPassage(Point start, Point current);

Vec4 collectDistancesToWalls(Point start);
Vec4 collectDistancesToUnexplored(Point start);
Vec4 collectDistancesToPerpendicular(Point start);

int relationship(int a, int b);
Point dirVec(int dir);
int relationshipDirection(int dirA, int dirB, Point currLoc, Point desLoc);
int betterDirection(int dirA, int dirB, Point currLoc, Point desLoc);

int comparePathOptions(int n_explored, int n_closenessx, int n_closenessy, int n_length,
                       int b_explored, int b_closenessx, int b_closenessy, int b_length);
Point  getDesLocation(Point _curr_pos);
Point where_the_hech_should_I_go(void);
//Right now we are assuming next location is on the same x or x
//as the car. When car is surrounded completely by explored locations, it should make nextlocation the closest unexplored location. When that happens, we need to figure out what to do.
Point one_square_closer_to_next_location(Point _curr_pos, Point next_location);

/*-------( API )-------*/
Point pathfind_update(Point _curr_pos, Vec4f measurements);
void pathfind_init(Point start, Point heading);
void pathfind_print(void);
void pathfind_finish(void);

#endif