//#ifndef Wait
//#define Wait
#include "Arduino.h"
//Alternative to delay() and delayMicroseconds()
//Permits multitasking
//Thanks ChatGPT



unsigned long SYS_last_time;

void WaitMicroseconds(int _us)
{
  unsigned long start_time = micros();
  while (micros() - start_time < _us);
  SYS_last_time = micros();
}

void Wait(int _ms)
{
  unsigned long start_time = millis();
  while (millis() - start_time < _ms);
  SYS_last_time = millis();
}
