/******************************************************************************
*
* Module: ECU2_App
*
* File Name: ECU2_App.h
*
* Description: Header file for TM4C123GH6PM Microcontroller Application layer.
*
* Author: Team 9
******************************************************************************/

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

/*******************************************************************************
*                              Module Definitions                              *
********************************************************************************/
#define PRIORITY_0              0
#define PRIORITY_1              1
#define PRIORITY_2              2
#define PRIORITY_3              3

#define PERIODICITY_1           0
#define PERIODICITY_2           1
#define PERIODICITY_3           2
#define PERIODICITY_5           4
#define PERIODICITY_11          10
#define PERIODICITY_100         99
#define PERIODICITY_500         499

#define FIRST_DELAY_0           0
#define FIRST_DELAY_1           1
#define FIRST_DELAY_2           2
#define FIRST_DELAY_52          52

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
void App_Init (void);
void Task1_LEDS (void);
void Task2_CAN (void);





#endif