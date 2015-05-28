#include "Arduino.h"
#include "control.h"
int wrap(int value, int _begin, int _end) {
    int normal_beg = 0, normal_end = _end - _begin, normal_val = value-_begin;
    int _overflow = abs(normal_beg - normal_end)/normal_val;

    if(_overflow < normal_beg){
      return _end-abs(_overflow);
    }else if(_overflow > normal_end){
      return _begin+abs(_overflow);
    }

    return _overflow;
}

int normalize(int n){
  if (n !=0) return n/abs(n);
  return 0;
}

void delayForDegrees(int _degrees){
  delay(TIME_FOR_ANGLE(_degrees));
}

void delayForInches(int inches){
  delay(TIME_FOR_MOVE(inches));
}

void setTurnLeft(int degrees){
  bridge.write(PIN_A, MOTOR_WRITE_VAL);
  bridge.write(PIN_B, MOTOR_WRITE_VAL);
  bridge.setDirection(PIN_A, PIN_BACKWARD);
  bridge.setDirection(PIN_B, PIN_FORWARD);

  bridge.update();

  delayForDegrees(degrees);
  bridge.write(PIN_A, 0);
  bridge.write(PIN_B, 0);
  bridge.update();

}

void setTurnRight(int degrees){
  bridge.write(PIN_A, MOTOR_WRITE_VAL);
  bridge.write(PIN_B, MOTOR_WRITE_VAL);
  bridge.setDirection(PIN_A, PIN_FORWARD);
  bridge.setDirection(PIN_B, PIN_BACKWARD);
  bridge.update();
  delayForDegrees(degrees);
  bridge.write(PIN_A, 0);
  bridge.write(PIN_B, 0);
  bridge.update();
}

void turnTo(int degrees){

  int dist = degrees-currAngle;
  int dir = normalize(dist);
  int dist_clockwise, dist_counter_clockwise, min_dist;


	if(dir == -1){
    dist_clockwise = (360-currAngle)+degrees;
    dist_counter_clockwise = dist;
      if(dist_clockwise< dist_counter_clockwise) dir = -1;
      else if(dist_clockwise > dist_counter_clockwise) dir = 1;
  } else if(dir == 1){
    dist_counter_clockwise = currAngle+(360-degrees);
    dist_clockwise = dist;
      if(dist_clockwise< dist_counter_clockwise) dir = 1;
      else if(dist_clockwise > dist_counter_clockwise) dir = -1;
  }

  if(dir ==-1) setTurnLeft(dist);
  else if(dir ==1) setTurnRight(dist);

  currAngle = degrees;

}

void adjustDistances(){

}

void adjustOrientation(){

}

void forward(int inches){
  bridge.write(PIN_A, MOTOR_WRITE_VAL);
  bridge.write(PIN_B, MOTOR_WRITE_VAL);
  bridge.setDirection(PIN_A, PIN_FORWARD);
  bridge.setDirection(PIN_B, PIN_FORWARD);
  bridge.update();

  delayForInches(inches);

  bridge.write(PIN_A, 0);
  bridge.write(PIN_B, 0);
  bridge.update();
}

void carInit(){
  pathfind_init(START_PT, END_PT);
}

void carUpdate(){
//  Vec4f measurements = getUltrasonicReadings();

}
