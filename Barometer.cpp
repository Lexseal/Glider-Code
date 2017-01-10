#include "Barometer.h"

void baroMeter::calib()
{
  Wire.begin();

  ac1 = BMPReadInt(0xAA);
  ac2 = BMPReadInt(0xAC);
  ac3 = BMPReadInt(0xAE);
  ac4 = BMPReadInt(0xB0);
  ac5 = BMPReadInt(0xB2);
  ac6 = BMPReadInt(0xB4);
  b1 = BMPReadInt(0xB6);
  b2 = BMPReadInt(0xB8);
  mb = BMPReadInt(0xBA);
  mc = BMPReadInt(0xBC);
  md = BMPReadInt(0xBE);
}

int baroMeter::getAlt()
{
  long up = BMPReadUP();
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  b6 = b5 - 4000;
  // 计算B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  // 计算B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return (int)(44329-4946*pow(p, 0.19026));
}

int baroMeter::getTemp()
{
  long ut = BMPReadUT();
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;
  return (int)((b5 + 8)>>4);
}

unsigned int baroMeter::BMPReadUT()
{
  unsigned int ut;
  // 在寄存器0xF4写入0x2E
  // 这个用来请求进行温度读取
  Wire.beginTransmission(BMP);

  Wire.write(0xF4);

  Wire.write(0x2E);
  Wire.endTransmission();
  // 至少等待4.5ms
  delay(5);
  // 从寄存器0xF6与0xF7读取两个字节
  ut = BMPReadInt(0xF6);
  return ut;
}

// 读取未补偿的压力值
unsigned long baroMeter::BMPReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  // 写入0x34+(OSS<<6)到寄存器0xF4
  // 请求气压数据读取
  Wire.beginTransmission(BMP);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  // 等待转换，延迟时间依赖于OSS
  delay(2 + (3<<OSS));
  // 读取寄存器0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP, 3);
  // 等待数据变为可用
  while(Wire.available() < 3);
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  return up;
}

char baroMeter::BMPRead(unsigned char address)
{
  unsigned char data;
  Wire.beginTransmission(BMP);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(BMP, 1);
  while(!Wire.available())  ;
  return Wire.read();
}

int baroMeter::BMPReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  Wire.beginTransmission(BMP);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(BMP, 2);
  while(Wire.available()<2);
  msb = Wire.read();
  lsb = Wire.read();
  return (int) msb<<8 | lsb;
}
