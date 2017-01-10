#include "outPut.h"

void outPut::initServos()
{
  servo1.attach(A0);
  servo2.attach(A1);
  //pinMode(A0, OUTPUT);
  //pinMode(A1, OUTPUT);
}

void outPut::updateServos()
{
	int servo1Value, servo2Value;

	servo1Value = servoBase + _PID.xPID + _PID.yPID;
	servo2Value = servoBase - _PID.xPID + _PID.yPID;

  /*Serial.print(" ");
  Serial.print(_PID.xPID);
  Serial.print(" ");
  Serial.print(_PID.yPID);
  Serial.print(" ");
  Serial.print(_PID.zPID);
  Serial.print(" ");*/

	if (_PID.zPID > 0)
	{
		servo1Value += _PID.zPID;
		servo2Value -= _PID.zPID/4;
	}
	else
	{
		servo1Value -= _PID.zPID/4;
		servo2Value += _PID.zPID;
	}

	if (servo1Value < 40)
	{
		servo1Value = 40;
	}
	else if (servo1Value > 120)
	{
		servo1Value = 120;
	}

	if (servo2Value < 40)
	{
		servo2Value = 40;
	}
	else if (servo2Value > 120)
	{
		servo2Value = 120;
	}

  lastServo1 = (lastServo1+servo1Value)/2;
  lastServo2 = (lastServo2+servo2Value)/2;
  
	servo1.write(lastServo1);
	servo2.write(lastServo2);

  /*digitalWrite(A0, HIGH);
  delayMicroseconds(lastServo1*7.8+500);
  digitalWrite(A0, LOW);
  delayMicroseconds(16666);

  digitalWrite(A1, HIGH);
  delayMicroseconds(lastServo2*7.8+500);
  digitalWrite(A1, LOW);
  delayMicroseconds(16666);*/

  /*Serial.print(lastServo1);
  Serial.print("  ");
  Serial.print(lastServo2);*/
}

void outPut::updatePID(struct pidData data)
{
  if (data.errorZ < -180)
  {
    data.errorZ+=270;
    zI = 0;
  } else if (data.errorZ > 180)
  {
    data.errorZ-=270;
    zI = 0;
  }
  
	_PID.xPID = -data.errorX - 0.1*data.rateX;
	_PID.yPID = data.errorY + 0.1*data.rateY;
	_PID.zPID = data.errorZ;

	if (data.errorX > 1.5)
	{
		xI -= 0.06;
    if (xI < -15)
    {
      xI = -15;
    }
	}
	else if (data.errorX < -1.5)
	{
		xI += 0.06;
    if (xI > 15)
    {
      xI = 15;
    }
	}
	else
	{
		xI = 0;
	}

	if (data.errorY > 1.5)
	{
		yI += 0.06;
    if (yI > 15)
    {
      yI = 15;
    }
	}
	else if (data.errorY < -1.5)
	{
		yI -= 0.06;
    if (yI < -15)
    {
      yI = -15;
    }
	}
	else
	{
		yI = 0;
	}

  if (data.errorZ > 1.5)
  {
    zI += 0.04;
    if (zI > 15)
    {
      zI = 30;
    }
  }
  else if (data.errorZ < -1.5)
  {
    zI -= 0.04;
    if (zI < -15)
    {
      zI = -30;
    }
  }
  else
  {
    zI = 0;
  }

  if ((lastErrX>0 && data.errorX<0) || (lastErrX<0 && data.errorX>0))
  {
    xI = 0;
  } if ((lastErrY>0 && data.errorY<0) || (lastErrY<0 && data.errorY>0))
  {
    yI = 0;
  } if ((lastErrZ>0 && data.errorZ<0) || (lastErrZ<0 && data.errorZ>0))
  {
    zI = 0;
  }

	_PID.xPID += xI;
	_PID.yPID += yI;
  _PID.yPID += zI;

  if (_PID.xPID > largestAllowed)
  {
    _PID.xPID = largestAllowed;
  }
  else if (_PID.xPID < -largestAllowed)
  {
    _PID.xPID = -largestAllowed;
  }

  if (_PID.yPID > largestAllowed)
  {
    _PID.yPID = largestAllowed;
  }
  else if (_PID.yPID < -largestAllowed)
  {
    _PID.yPID = -largestAllowed;
  }

  if (_PID.zPID > largestAllowed)
  {
    _PID.zPID = largestAllowed;
  }
  else if (_PID.zPID < -largestAllowed)
  {
    _PID.zPID = -largestAllowed;
  }

  /*Serial.print("  pidx = "); Serial.print(_PID.xPID);
  Serial.print("  pidy = "); Serial.print(_PID.yPID);
  Serial.print("  pidz: "); Serial.print(_PID.zPID);*/

  updateServos();
}
