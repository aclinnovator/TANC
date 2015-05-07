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


//################ Program level settings ##################

//What type of maze we are dealing with
#define MAZE_TYPE_BLOCKS      1
#define MAZE_TYPE_WALLS       2

#define MAZE_TYPE        MAZE_TYPE_BLOCKS

#define IS_BLOCKS MAZE_TYPE == MAZE_TYPE_BLOCKS
#define IS_WALLS  MAZE_TYPE == MAZE_TYPE_WALLS

//############### Maze specific data #######################

//There are three parameters that can be changed -
//MAZE_HEIGHT, MAZE_WIDTH, and SQUARE_SIZE_INCH  -
//If they are not already implemented, then use defaults.
#define MAZE_HEIGHT           5
#define MAZE_WIDTH            5
#define SQUARE_SIZE_INCH      1


#define INTERSQUARE_SPACE     0


#if IS_BLOCKS

#define  _MAZE_WIDTH  MAZE_WIDTH
#define  _MAZE_HEIGHT MAZE_HEIGHT
#define MOVEMENT_UNIT_SIZE SQUARE_SIZE_INCH

#elif IS_WALLS

#define _MAZE_WIDTH  MAZE_WIDTH * 2 + 1
#define _MAZE_HEIGHT MAZE_HEIGHT* 2 + 1
#define MOVEMENT_UNIT_SIZE SQUARE_SIZE_INCH + INTERSQUARE_SPACE

#endif
//############### Directions ###############################
//When dealing with a Vec4 holding values in 4 directions
//These hold both direction and axis on the same plane
#define NORTH_IND             0
#define SOUTH_IND             1
#define EAST_IND              2
#define WEST_IND              3

//When dealing with directions each value represents its direction on its own plane
#define NORTH                 1
#define SOUTH                -1
#define EAST                  1
#define WEST                 -1

//Specifying an axis
#define X_AXIS                1
#define Y_AXIS                2

//############### "Anonymous" Enums ###############
#define WALL                  1
#define FLOOR                 0
//----
#define UNEXPLORED            1
#define EXPLORED              2
#define DEAD_END              3


/*--( Unexplored )------*/
#define    UNEXPLORED_CLOSE_SHORT    1
#define    UNEXPLORED_CLOSE_LONG     2
#define    UNEXPLORED_FAR_SHORT      3
#define    UNEXPLORED_FAR_LONG       4

/*-- ( Explored )------*/
#define    EXPLORED_CLOSE_SHORT      5
#define    EXPLORED_CLOSE_LONG       6
#define    EXPLORED_FAR_SHORT        7
#define    EXPLORED_FAR_LONG         8

#define POINT_REACHABLE              1
#define POINT_UNREACHABLE            2



#define EVEN                         1
#define ODD                          2

#define EQUAL                        0
#define MORE_THAN                    1
#define LESS_THAN                    2
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

//Representing one single point
typedef struct {
    int x;
    int y;
} Point;

typedef int Bool;
#define true 1
#define false 0

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

/*-------( Math functions )-------*/
int cut(int n, int mag, int _min, int _max);

int absv(int n);

/*-------( State specific: WALLS or FLOORS )-------*/
Point pointToArrayIndex(Point p);

Point pointToReachableSquare(Point p, int dir);

/*--------( Index Checkers and solvers )-------*/

int pointInIndBounds(Point p);

Point cutToMazeBounds(Point n);

/*--------( Vector functions )-------*/

Point Vec(int x, int y);

Bool cmpVec(Point n, Point l);

Point getVec(Point a, Point b);

Point ceilVec(Point vec);

Point addVec(Point a, Point b);

Point subVec(Point a, Point b);

Point absVec(Point a);

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
/*-------( Core Algorithm)-------*/
Bool stopAtWall(Point start, Point current);
int relationship(int a, int b);
int comparePathOptions(int n_length, int n_closeness, int n_explored,
                 int b_length, int b_closeness, int b_explored);
int  bestDirection(Point _curr_pos);
Point where_the_hech_should_I_go();
//Right now we are assuming next location is on the same x or x
//as the car. When car is surrounded completely by explored locations, it should make nextlocation the closest unexplored location. When that happens, we need to figure out what to do.
Point one_square_closer_to_next_location(Point _curr_pos, Point next_location);

/*-------( API )-------*/
Point pathfind_update(Point _curr_pos, Vec4f measurements);
void pathfind_init(Point start, Point heading);
void pathfind_print(void);
