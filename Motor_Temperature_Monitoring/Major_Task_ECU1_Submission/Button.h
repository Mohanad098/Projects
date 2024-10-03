/*******************************************************************************
*                                                                              *
* Module: Button                                                               *
*                                                                              *
* File Name: Button.h                                                          *
*                                                                              *
* Description: Header file for TM4C123GH6PM Microcontroller - Button Module    *
*                                                                              *
* Author: Team 9                                                               *  
********************************************************************************/

#ifndef  _BUTTON_H_
#define  _BUTTON_H_

#include "Std_Types.h"

/* Button States */
#define Pressed_Button      STD_LOW
#define Released_Button     STD_HIGH

/*******************************************************************************
*                      Function Prototypes                                     *
********************************************************************************/
uint8 Button1_State(void);
uint8 Button2_State(void);





#endif