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

#ifndef __Control__
#define __Control__
#include "tcpathfind.h"
#include "./HBridge.h"

#define MTR_AA 2 //2 on hbridge
#define MTR_AB 7 //7

#define MTR_BA 3 //3
#define MTR_BB 6 //6

#define MOTOR_WRITE_VAL  255

#define _MOVE_TIME_INCH  100
#define INCH    1
#define MOVE_TIME_INCH (_MOVE_TIME_INCH/INCH)
#define TIME_FOR_MOVE(n)(n*MOVE_TIME_INCH)

#define _TURN_TIME_ANGLE 1000
#define ANGLE            1
#define TURN_TIME_ANGLE (_TURN_TIME_ANGLE/ANGLE)
#define TIME_FOR_ANGLE(n) (n*TURN_TIME_ANGLE)


#define START_PT Vec(0,0)
#define END_PT Vec(5,5)

HBridge bridge(MTR_AA, MTR_AB, MTR_BA, MTR_BB);


int currAngle;

void setTurnLeft(int degrees);
void setTurnRight(int degrees);

void turn(int degrees);

void move(void);
#endif
