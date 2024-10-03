 /******************************************************************************
 *
 * Module: OS
 *
 * File Name: OS.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - OS Driver.
 *
 * Author: Team 9
 ******************************************************************************/
 
#ifndef _OS_H
#define _OS_H

#include "Std_Types.h"
#include <stdbool.h>
#include "OS_CFG.h"

/* Enum with the Task state */
typedef enum
{
	suspend,
	running
}TaskState;

/* Struct for each task */
typedef struct
{
	void (*TaskHandler)(void);
	uint8 Periodicity;
        uint8 Priority;
	uint8 FirstDelay;
	TaskState TSTATE;
}TASK;

typedef struct
{
  TASK OS_Tasks[MAX_TASKS_NUMBER];
}Task_Config;



/******** Error messages **********/
#define MAX_NUMBER_OF_TASKS_EXCEEDED    1
#define NO_TASK_GIVEN_TO_DELETE         2
#define NO_TASK_GIVEN_TO_SUSPEND        3
#define MAX_NUMBER_OF_TASKS_RESUME      4
#define NO_TASK_GIVEN_TO_CREATE         5        

extern Task_Config Task_CFG;

/***************************** Functions Prototypes ***************************/
void RTOS_CreateTask(Task_Config * Ptr_Tasks);

void RTOS_Init (void);

void Scheduler (void);

void RTOS_DeleteTask(uint8 priority);

void RTOS_SuspendTask (uint8 priority);

void RTOS_ResumeTask (uint8 priority);

bool Error_State(uint8 error_code);

void Timer_Callback(void);

#endif