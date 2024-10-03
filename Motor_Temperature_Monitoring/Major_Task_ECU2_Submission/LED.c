/******************************************************************************
*
* Module: LED
*
* File Name: LED.c
*
* Description: Source file for TM4C123GH6PM Microcontroller - LED Module
*
* Author: Team 9
******************************************************************************/

#include "LED.h"
#include "Dio.h"

/************************************************************************************
* Function Name : RED_LED
* Inputs        : None
* Outputs       : None
* Reentrancy    : Reentrant
* Sync/Async    : Synchronous
* Description   : Displays Red light on ECU2 and Turns off other led channels
************************************************************************************/
void RED_LED(void){
  Dio_WriteChannel(DioConf_RED_LED_CHANNEL_ID_INDEX, LED_ON);
  Dio_WriteChannel(DioConf_BLUE_LED_CHANNEL_ID_INDEX, LED_OFF);
  Dio_WriteChannel(DioConf_GREEN_LED_CHANNEL_ID_INDEX, LED_OFF);
}

/************************************************************************************
* Function Name : BLUE_LED
* Inputs        : None
* Outputs       : None
* Reentrancy    : Reentrant
* Sync/Async    : Synchronous
* Description   : Displays Blue light on ECU2 and Turns off other led channels
************************************************************************************/
void BLUE_LED(void){
  Dio_WriteChannel(DioConf_RED_LED_CHANNEL_ID_INDEX, LED_OFF);
  Dio_WriteChannel(DioConf_BLUE_LED_CHANNEL_ID_INDEX, LED_ON);
  Dio_WriteChannel(DioConf_GREEN_LED_CHANNEL_ID_INDEX, LED_OFF);
}

/************************************************************************************
* Function Name : GREEN_LED
* Inputs        : None
* Outputs       : None
* Reentrancy    : Reentrant
* Sync/Async    : Synchronous
* Description   : Displays Green light on ECU2 and Turns off other led channels
************************************************************************************/
void GREEN_LED(void){
  Dio_WriteChannel(DioConf_RED_LED_CHANNEL_ID_INDEX, LED_OFF);
  Dio_WriteChannel(DioConf_BLUE_LED_CHANNEL_ID_INDEX, LED_OFF);
  Dio_WriteChannel(DioConf_GREEN_LED_CHANNEL_ID_INDEX, LED_ON);
}

/************************************************************************************
* Function Name : WHITE_LED
* Inputs        : None
* Outputs       : None
* Reentrancy    : Reentrant
* Sync/Async    : Synchronous
* Description   : Displays White light turning on all leds
************************************************************************************/
void WHITE_LED(void){
  Dio_WriteChannel(DioConf_RED_LED_CHANNEL_ID_INDEX, LED_ON);
  Dio_WriteChannel(DioConf_BLUE_LED_CHANNEL_ID_INDEX, LED_ON);
  Dio_WriteChannel(DioConf_GREEN_LED_CHANNEL_ID_INDEX, LED_ON);
}

/************************************************************************************
* Function Name : TURN_OFF_LEDS
* Inputs        : None
* Outputs       : None
* Reentrancy    : Reentrant
* Sync/Async    : Synchronous
* Description   : Turns off all leds
************************************************************************************/
void TURN_OFF_LEDS(void)
{
  Dio_WriteChannel(DioConf_RED_LED_CHANNEL_ID_INDEX, LED_OFF);
  Dio_WriteChannel(DioConf_BLUE_LED_CHANNEL_ID_INDEX, LED_OFF);
  Dio_WriteChannel(DioConf_GREEN_LED_CHANNEL_ID_INDEX, LED_OFF);
  
}
