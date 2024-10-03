/******************************************************************************
*
* Module: OS
*
* File Name: OS_CFG.c
*
* Description: Configuration source file for TM4C123GH6PM Microcontroller - OS driver .
*
* Author: Team 9
******************************************************************************/

#include "OS.h"

Task_Config Task_CFG = {
  Task1_LEDS,PERIODICITY_100, PRIORITY_0,FIRST_DELAY_0, running,
  Task2_CAN,PERIODICITY_10, PRIORITY_1,FIRST_DELAY_1, running
  
};