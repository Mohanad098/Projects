 /******************************************************************************
 *
 * Module: SYSTICK
 *
 * File Name: MSYST_Int.h
 *
 * Description: Interface file for TM4C123GH6PM Microcontroller - Systick Driver.
 *
 * Author: Team 9
 ******************************************************************************/

#ifndef _MSYST_INT_H_
#define _MSYST_INT_H_

#include "Platform_Types.h"

/**************************************** Base Addresses ********************************************
****************************************************************************************************/

#define MSYST_BASE_ADDRESS		0xE000E010

/******************************** Struct for Systick registers **************************************
****************************************************************************************************/

typedef struct
{  
  uint32 CTRL;
  uint32 LOAD;
  uint32 VAL;  
}MSYST_t;

typedef enum
{
  ENABLE_PIN,TICKINT,CLKSOURCE,COUNT_FLAG=16
}MSYST_Peripherals;

typedef enum
{
  PRIORITY0,PRIORITY1,PRIORITY2,PRIORITY3,PRIORITY4,PRIORITY5,PRIORITY6,PRIORITY7
}Priority;
typedef enum
{
  PIOSC,SYSTEM_CLOCK
}ClockSource_Types;

typedef enum
{
  DISABLE,ENABLE
}Enable_Types;

typedef struct
{
  Enable_Types Int_Enable;
  ClockSource_Types CLKSource;
  uint32 PreloadValue;
  Priority Systick_Interupt_Priority;
}SYS_Config;


/***************************************** Declerations *********************************************
****************************************************************************************************/

#define MSYST ((volatile MSYST_t*)MSYST_BASE_ADDRESS)
#define SYSTICK_PRIORITY_MASK  0x1FFFFFFF
#define SYSTICK_CLOCKSOURCE_MASK  0xFFFFFFFB
#define SYSTICK_INTURRUPT_MASK 0xFFFFFFFD
//#define SYSTICK_INTERRUPT_PRIORITY  3
#define SYSTICK_PRIORITY_BITS_POS   29

/***************************************** Prototypes *********************************************
****************************************************************************************************/
void MSYS_voidInit(const SYS_Config * Config_Ptr);

void MSYS_voidIntState(void);

uint8 MSYS_ReadFlag(void);

void MSYS_voidDelayUsec(uint32 delayUs);

void MSYS_voidDelayMsec(uint32 delayMs);

uint32 MSYS_u32GetRemainingTime(void);

void MSYS_CallBack(void(*callback)(void));

extern const SYS_Config Config;


#endif /* _MSYST_INT_H_ */
