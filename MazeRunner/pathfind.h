/*

 .___________.    ___      .__   __.   ______
 |           |   /   \     |  \ |  |  /      |
 `---|  |----`  /  ^  \    |   \|  | |  ,----'
     |  |      /  /_\  \   |  . `  | |  |
     |  |     /  _____  \  |  |\   | |  `----.
     |__|    /__/     \__\ |__| \__|  \______|

 T railblazing A utonomous  N avigating   C ar
   A project by Ilan Katz and Akiva Lipshitz, although this part was written by Akiva Lipshitz

 */

 #ifdef __APPLE__
 #include <stdlib.h>
 #include <stdio.h>
 #include <math.h>
 #endif

#include "tcpoint.h"
#include "tcmath.h"
#include "tcconstants.h"

//################ Macro Functions #########################
#define inBound(val, min, max) ((val >= min) && (val <= max) ? 1 : 0)
//----
#define inBoundsX(x) inBound(x, 0, MAZE_WIDTH)
#define inBoundsY(x) inBound(y, 0, MAZE_HEIGHT)
//----
#define inBoundsIndX(x) inBound(x, 0, MAZE_WIDTH - 1)
#define inBoundsIndY(y) inBound(y, 0, MAZE_HEIGHT - 1)
#define oddOrEven(n) ((n % 2) == 0)

#define getAxis(direction) \
    (oddOrEven(direction) == EVEN ? Y_AXIS : X_AXIS)


//Bitfield:
//See https://msdn.microsoft.com/en-us/library/yszfawxh(v=vs.80).aspx
//This Bitfield only takes up
//(3 + 1) * MAZE_HEIGHT * MAZE_WIDTH bits

//This structure is used to store the information collected
// for one single square in a maze

//Status: EXPLORED, UNEXPLORED, DEAD_END
//Classification: WALL, FLOOR
typedef struct {
    unsigned short status         : 3; //Because it goes up to 3, not 2
    unsigned short classification : 1;// WALL or FLOOR
}MazeSquare;

typedef struct Vec4 {
    int& operator[](int i) { return vec[i]; }
    int vec[4];
} Vec4;

typedef struct Vec4f {
    float& operator[](int i) { return vec[i]; }
    float vec[4];
} Vec4f;

typedef Bool (*PointFunction)(Point, Point);

//############# Function Declarations ############
void printPoint(Point pt);

/*-------( State specific: WALLS or FLOORS )-------*/
Point pointToArrayIndex(Point p);

Point pointToReachableSquare(Point p, int dir);

/*--------( Index Checkers and solvers )-------*/

int pointInIndBounds(Point p);

Point cutToMazeBounds(Point n);

/*--------( Axis related functions )-------*/
Point translateOnAxisInDirection(Point start, int dir, int magnitude);

Point axialMagnitudeNormalized(Point a, Point b);

int pointsOnAxisInDirectionUntil(
                      Point p,
                      PointFunction checker,
                      int direction,
                      int axial_magnitude) ;

/*-------( Classifiers )--------*/
Vec4 classifyMeasurements(Vec4f mesments);
int wallOrFloor(int mes);
int floorsIlluminatedByMeasurement(float measurement);

/*--------( Storage Functions )-------*/

void storeClassificationWall(Point p);
void storeClassificationFloor(Point p);
Bool storeClassification(Point p, int classification);
Bool storeStatus(Point p, int status);

void save(Point currPos, Vec4 ultraSonicData);

/*-------( Retrieval )-------*/
int classification(Point p);
int status(Point p);
/*-------( Core Algorithm)-------*/
Bool stopAtWall(Point start, Point current);
Bool stopAtPerpendicularPassage(Point start, Point current);
int relationship(int a, int b);
int comparePathOptions(int n_length, int n_closeness, int n_explored,
                 int b_length, int b_closeness, int b_explored);
Point  getDesLocation(Point _curr_pos);
Point where_the_hech_should_I_go();
//Right now we are assuming next location is on the same x or x
//as the car. When car is surrounded completely by explored locations, it should make nextlocation the closest unexplored location. When that happens, we need to figure out what to do.
Point one_square_closer_to_next_location(Point _curr_pos, Point next_location);

/*-------( API )-------*/
Point pathfind_update(Point _curr_pos, Vec4f measurements);
void pathfind_init(Point start, Point heading);
void pathfind_print(void);
void pathfind_finish();

