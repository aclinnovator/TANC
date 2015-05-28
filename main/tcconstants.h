//
//  constants.h
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/13/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#ifndef MazeRunner_constants_h
#define MazeRunner_constants_h



//################ Program level settings ##################

//What type of maze we are dealing with
#define MAZE_TYPE_BLOCKS      1
                /*
                 
                     #---#---#---#---#---#
                 5   |   |   |   |   |   |
                     #---#---#---#---#---#
                 4   |   |   |   |   |   |
                     #---#---#---#---#---#
                 3   |   |   |   |   |   |
                     #---#---#---#---#---#
                 2   |   |   |   |   |   |
                     #---#---#---#---#---#
                 1   |   |   |   |   |   |
                     #---#---#---#---#---#
                       1   2   3   4   5
                 
                 5 * 5 = 25in^2
                 */
#define MAZE_TYPE_WALLS       2

                /*
                 
                 
                 11  #---#---#---#---#---#
                 10  |   |   |   |   |   |
                 9   #---#---#---#---#---#
                 8   |   |   |   |   |   |
                 7   #---#---#---#---#---#
                 6   |   |   |   |   |   |
                 5   #---#---#---#---#---#
                 4   |   |   |   |   |   |
                 3   #---#---#---#---#---#
                 2   |   |   |   |   |   |
                 1   #---#---#---#---#---#
                     1 2 3 4 5 6 7 8 9 10 11
                 
                 11 * 11 = 121in^2
                 
                 */
#define MAZE_TYPE        MAZE_TYPE_BLOCKS

#define IS_BLOCKS MAZE_TYPE == MAZE_TYPE_BLOCKS
#define IS_WALLS  MAZE_TYPE == MAZE_TYPE_WALLS

//############### Maze specific data #######################

//There are three parameters that can be changed -
//MAZE_HEIGHT, MAZE_WIDTH, and SQUARE_SIZE_INCH  -
//If they are not already implemented, then use defaults.
#define MAZE_HEIGHT           7
#define MAZE_WIDTH            6
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


#define ON_QUEUE              1
#define OFF_QUEUE             0

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
#define POINT_UNREACHABLE            0

/*-------( Math related )-------*/
#define EVEN                         1
#define ODD                          2

#define EQUAL                        0
#define MORE_THAN                    1
#define LESS_THAN                    2



//################ Macro Functions #########################
#define inBound(val, min, max) ((val >= min) && (val <= max))
//----
#define inBoundsX(x) inBound(x, 0, MAZE_WIDTH)
#define inBoundsY(x) inBound(y, 0, MAZE_HEIGHT)
//----
#define inBoundsIndX(x) inBound(x, 0, MAZE_WIDTH - 1)
#define inBoundsIndY(y) inBound(y, 0, MAZE_HEIGHT - 1)
#define oddOrEven(n) ((n % 2) == 0)

#define getAxis(direction) \
(oddOrEven(direction) == EVEN ? Y_AXIS : X_AXIS)

#define sumVec4(vec) (vec[0] + vec[1] + vec[2] + vec[3])

#endif
