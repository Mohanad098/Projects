#ifndef Stopwatch_Func_H_
#define Stopwatch_Func_H_

#include <stdint.h>



void Clock(void);
void delayms(uint32_t time);

//void SystickIntTagroba(uint32_t time); //DELETE
void SystickInt(uint32_t time);

#endif