/******************************************************************************
*
* Module: LED
*
* File Name: LED.h
*
* Description: Header file for TM4C123GH6PM Microcontroller - LED Module
*
* Author: Team 9
******************************************************************************/
#ifndef _LED_H
#define _LED_H

#include "Std_Types.h"

#define LED_ON      STD_HIGH
#define LED_OFF     STD_LOW

void LEDS_Init(void);
void RED_LED(void);
void BLUE_LED(void);
void GREEN_LED(void);
void WHITE_LED(void);
void TURN_OFF_LEDS(void);

#endif