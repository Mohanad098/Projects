/******************************************************************************
 *
 * Module: OS
 *
 * File Name: OS.h
 *
 * Description: Configuration header file for TM4C123GH6PM Microcontroller - OS Driver.
 *
 * Author: Team 9
 ******************************************************************************/
 
#ifndef _OS_CFG_H
#define _OS_CFG_H


/***************************
*                              Module Definitions                              *
****************************/
#define MAX_TASKS_NUMBER        2

#define PRIORITY_0              0
#define PRIORITY_1              1
#define PRIORITY_2              2
#define PRIORITY_3              3

#define PERIODICITY_1           0
#define PERIODICITY_2           1
#define PERIODICITY_3           2
#define PERIODICITY_5           4
#define PERIODICITY_10          9
#define PERIODICITY_100         99
#define PERIODICITY_200         199
#define PERIODICITY_500         499

#define FIRST_DELAY_0           0
#define FIRST_DELAY_1           1
#define FIRST_DELAY_2           2
#define FIRST_DELAY_3           3
#define FIRST_DELAY_203         203

/***************************
 *                      Function Prototypes                                    *
 ***************************/
extern void App_Init (void);
extern void Task1_LEDS (void);
extern void Task2_CAN (void);

#endif