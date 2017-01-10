#ifndef outPut_h
#define outPut_h

#include "Arduino.h"
#include <Servo.h>

struct pidData
{
  int errorX, errorY, errorZ;
  int rateX, rateY, rateZ;
};

struct PID
{
  int xPID, yPID, zPID;
};

const int servoBase = 80;
const int largestAllowed = 40;

class outPut
{
  public:
  	void initServos();
  	void updatePID(struct pidData data);
  private:
  	Servo servo1, servo2;

  	float xI, yI, zI;
  	struct PID _PID;

    int lastServo1, lastServo2;
    int lastErrX, lastErrY, lastErrZ;

    void updateServos();
};

#endif
