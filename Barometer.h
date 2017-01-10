#ifndef Barometer_h
#define Barometer_h

#include "Arduino.h"
#include <Wire.h>

#define BMP 0x77

const unsigned char OSS = 0;

class baroMeter
{
  public:
  	void calib();
  	int getAlt();
  	int getTemp();

  private:
  	int ac1;
	  int ac2; 
	  int ac3; 
	  unsigned int ac4;
	  unsigned int ac5;
	  unsigned int ac6;
	  int b1; 
	  int b2;
	  int mb;
	  int mc;
	  int md;

	  long b5;

	  unsigned int BMPReadUT();
	  unsigned long BMPReadUP();

	  char BMPRead(unsigned char address);
	  int BMPReadInt(unsigned char address);
};

#endif
