// DataStructures.h
#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include <Arduino.h>

struct JoystickData {
  // Classic joystick
  byte x;
  //byte y;

  // Holonom joystick
  byte holonomX;
  byte holonomY;
};

struct RadioData {
  JoystickData joystickData;
  
  // short grabberHeight;
  // short grabberOpeningAngle;

  // short score;

  // bool areMagnetsEnabled;
  // bool isRodDeployed;
  bool isRightPusherDeployed;
  bool isLeftPusherDeployed;
};

#endif