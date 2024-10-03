/*******************************************************************************
*                                                                              *
* Module: Button                                                               *
*                                                                              *
* File Name: Button.c                                                          *
*                                                                              *
* Description: Source file for TM4C123GH6PM Microcontroller - Button Module    *
*                                                                              *
* Author: Team 9                                                               *  
********************************************************************************/

#include "Dio.h"
#include "Button.h"
#include "Std_Types.h"

/************************************************************************************
* Function Name : Button1_State
* Inputs        : None
* Outputs       : uint8 state
* Reentrancy    : Reentrant
* Sync/Async    : Synchronous
* Description   : Reads the state of the Button 1 if pressed or not through its Channel Index
************************************************************************************/
uint8 Button1_State(void)
{
  uint8 state = Dio_ReadChannel(DioConf_SW1_CHANNEL_ID_INDEX);
  return state;
}


/************************************************************************************
* Function Name : Button2_State
* Inputs        : None
* Outputs       : uint8 state
* Reentrancy    : Reentrant
* Sync/Async    : Synchronous
* Description   : Reads the state of the Button 2 if pressed or not through its Channel Index
************************************************************************************/
uint8 Button2_State(void)
{
  uint8 state = Dio_ReadChannel(DioConf_SW2_CHANNEL_ID_INDEX);
  return state;
}
