/******************************************************************************
*
* Module: ECU1_App
*
* File Name: ECU1_App.h
*
* Description: Header file for TM4C123GH6PM Microcontroller Application layer.
*
* Author: Team 9
******************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_


/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
void App_Init (void);
void Task1_Button (void);
void Task2_CAN (void);
void Task3_IdleCheck (void);


#endif