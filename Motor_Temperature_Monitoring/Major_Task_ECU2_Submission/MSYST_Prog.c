/******************************************************************************
*
* Module: SYSTICK
*
* File Name: MSYST_Prog.c
*
* Description: Source file for TM4C123GH6PM Microcontroller - Systick Driver.
*
* Author: Team 9
******************************************************************************/

/***************************************** INCLUDES ************************************************/
#include "Common_Macros.h"
#include "tm4c123gh6pm.h"
#include "MSYST_Int.h"
#include "Std_Types.h"

/************************************** DECLERATIONS ***********************************************/

static void (*SYST_CallBack)(void)= NULL_PTR;


/************************************************************************************
* Function Name : MSYS_voidInit
* Inputs        : None
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Initiates the clock.
************************************************************************************/

void MSYS_voidInit(const SYS_Config * Config_Ptr)
{
  MSYST->CTRL = (MSYST->CTRL & SYSTICK_CLOCKSOURCE_MASK) | (Config_Ptr->CLKSource << CLKSOURCE);
  MSYST->CTRL = (MSYST->CTRL & SYSTICK_INTURRUPT_MASK)   | (Config_Ptr->Int_Enable << TICKINT);  
  NVIC_SYS_PRI3_R =  (NVIC_SYS_PRI3_R & SYSTICK_PRIORITY_MASK) | (Config_Ptr->Systick_Interupt_Priority << SYSTICK_PRIORITY_BITS_POS);
 
  CLEAR_BIT(MSYST->CTRL,ENABLE_PIN);
  MSYST->VAL=0;
  MSYST->LOAD=(Config_Ptr->PreloadValue*16000)-1;
  SET_BIT(MSYST->CTRL,ENABLE_PIN); 
  
}




/************************************************************************************
* Function Name : MSYS_voidStart
* Inputs        : uint32 Copy_u8PreloadValue
* Outputs       : None
* Reentrancy    : Non reentrant
* Sync/Async    : Synchronous
* Description   : Starts the systick timer.
************************************************************************************/

void MSYS_voidStart(uint32 Copy_u8PreloadValue)
{
  CLEAR_BIT(MSYST->CTRL,ENABLE_PIN);
  MSYST->VAL=0;
  MSYST->LOAD=(Copy_u8PreloadValue*16000)-1;
  SET_BIT(MSYST->CTRL,ENABLE_PIN); 
}


/************************************************************************************
* Function Name : MSYS_ReadFlag
* Inputs        : None
* Outputs       : sys_flag (flag value)
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Returns the flag value.
************************************************************************************/

uint8 MSYS_ReadFlag(void)
{
  uint8 sys_flag=0;
  sys_flag=GET_BIT(MSYST->CTRL,COUNT_FLAG);
  return sys_flag;
}


/************************************************************************************
* Function Name : MSYS_voidDelayUsec
* Inputs        : uint32 Copy_DelayUsec
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : A delay function in microseconds.
************************************************************************************/

void MSYS_voidDelayUsec(uint32 delayUs)
{
  MSYS_voidStart(delayUs);
  while(MSYS_ReadFlag()!=1){}
}


/************************************************************************************
* Function Name : MSYS_voidDelayMsec
* Inputs        : uint32 delayMs
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : A delay function in miliseconds.
************************************************************************************/

void MSYS_voidDelayMsec(uint32 delayMs)
{
  MSYS_voidStart(delayMs*1000);
  while(MSYS_ReadFlag()!=1){}
}


/************************************************************************************
* Function Name : MSYS_u32GetRemainingTime
* Inputs        : None
* Outputs       : Remaining time
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : A function returns remaining time.
************************************************************************************/

uint32 MSYS_u32GetRemainingTime(void)
{
  return (MSYST->VAL);
}

/************************************************************************************
* Function Name : MSYS_CallBack
* Inputs        : void(*callback)(void)         (Function handler)
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Asynchronous
* Description   : Assigns a function to the systick handler, callback function.
************************************************************************************/

void MSYS_CallBack(void(*callback)(void))
{
  SYST_CallBack=callback;
}

/* Systick handler */
void Systick_Handler(void)
{
  SYST_CallBack();
}

