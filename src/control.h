/*

 .___________.    ___      .__   __.   ______
 |           |   /   \     |  \ |  |  /      |
 `---|  |----`  /  ^  \    |   \|  | |  ,----'
     |  |      /  /_\  \   |  . `  | |  /
     |  |     /  _____  \  |  |\   | |  `----.
     |__|    /__/     \__\ |__| \__|  \______|

 T railblazing A utonomous  N avigating   C ar
   A project by Ilan Katz and Akiva Lipshitz, although this part was written by Akiva Lipshitz

 */

#include "HBridge.h"
#include "tcpathfind.h"


#define MTR_AA 2 //2 on hbridge
#define MTR_AB 7 //7

#define MTR_BA 3 //3
#define MTR_BB 6 //6

#define MOTOR_WRITE_VAL

#define _MOVE_TIME_INCH
#define INCH
#define MOVE_TIME_INCH (MOVE_TIME_INCH/INCH)
#define TIME_FOR_MOVE(n)(n*MOVE_TIME_INCH)

#define _TURN_TIME_ANGLE
#define ANGLE
#define TURN_TIME_ANGLE (TURN_TIME_ANGLE/ANGLE)
#define TIME_FOR_ANGLE(n) (n*TURN_TIME_ANGLE)



HBridge bridge(MTR_AA, MTR_AB, MTR_BA, MTR_BB);


int currAngle;

void setTurnLeft(int degrees);
void setTurnRight(int degrees);

void turn(int degrees);

void move(void);
