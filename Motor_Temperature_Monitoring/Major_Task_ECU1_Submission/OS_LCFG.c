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

Task_Config Task_CFG =
{
  Task1_Button,PERIODICITY_1, PRIORITY_0,FIRST_DELAY_0, running,
  Task2_CAN,PERIODICITY_5, PRIORITY_1,FIRST_DELAY_1, running,
  Task3_IdleCheck,PERIODICITY_200, PRIORITY_2,FIRST_DELAY_203, running
};