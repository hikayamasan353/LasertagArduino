#ifndef Wait_H
#define Wait_H
#include <Arduino.h>
//Alternative to delay() and delayMicroseconds()
//Permits multitasking
//Thanks ChatGPT



static unsigned long SYS_last_time;

static void WaitMicroseconds(int _us)
{
  unsigned long start_time = micros();
  while (micros() - start_time < _us);
  SYS_last_time = micros();
}

static void Wait(int _ms)
{
  unsigned long start_time = millis();
  while (millis() - start_time < _ms);
  SYS_last_time = millis();
}

#endif
