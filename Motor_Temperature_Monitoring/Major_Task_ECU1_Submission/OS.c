/******************************************************************************
*
* Module: OS
*
* File Name: OS.c
*
* Description: Header file for TM4C123GH6PM Microcontroller - OS Driver.
*
* Author: Team 9
******************************************************************************/

#include <assert.h>
#include "MSYST_Int.h"
#include "OS.h"

/* Tasks Container */
TASK SystemTasks [MAX_TASKS_NUMBER]={0};

/* Array to store Tasks timing */
uint16 Timing [MAX_TASKS_NUMBER];

/* Empty struct to delete tasks */
TASK Empty={0};

/* A variable that changes with each tick */
uint8 Execution_Time = 0;

/* Pointer to the tasks struct */
static TASK * Ptr_TasksCfg = NULL_PTR;

/************************************************************************************
* Function Name: RTOS_u8CreateTask
* Inputs:
1) void(*handler)(void)         : Handler to function that will be a task
2) uint8 Copy_periodicity       : Periodicity of the task
3) uint8 priority               : Task Priority
4) uint8 firstdelay             : Task first delay for better CPU Load handling
5) TaskState State              : Running or Suspend
* Outputs:None
* Reentrancy: Non reentrant
* Sync/Async: Synchronous
* Description: Creates a task if given correct arguments.
************************************************************************************/
void RTOS_CreateTask(Task_Config * Ptr_Tasks)
{
  if(Ptr_Tasks == NULL_PTR)
  {
    assert(Error_State(NO_TASK_GIVEN_TO_CREATE));
  }
  else
  {
    Ptr_TasksCfg = Ptr_Tasks->OS_Tasks;
    /* Check that the priority index exist */
    for(uint8 i=0;i<MAX_TASKS_NUMBER;i++)
    {
      if(Ptr_TasksCfg[i].Priority<MAX_TASKS_NUMBER)
      {
        while(SystemTasks[Ptr_TasksCfg[i].Priority].TaskHandler!=0)
        {
          Ptr_TasksCfg[i].Priority++;
        }
        if(SystemTasks[Ptr_TasksCfg[i].Priority].TaskHandler==0)
        {
          SystemTasks[Ptr_TasksCfg[i].Priority].TaskHandler     = Ptr_TasksCfg[i].TaskHandler;
          SystemTasks[Ptr_TasksCfg[i].Priority].Periodicity     = Ptr_TasksCfg[i].Periodicity;
          Timing[Ptr_TasksCfg[i].Priority]			= Ptr_TasksCfg[i].FirstDelay;	/* First time to run the task */
          SystemTasks[Ptr_TasksCfg[i].Priority].TSTATE	        = Ptr_TasksCfg[i].TSTATE;
        }
        else
        {
          /*Do nothing*/
        }
      }
      else
      {
        assert(Error_State(MAX_NUMBER_OF_TASKS_EXCEEDED));
      }
    }
  }
}


/************************************************************************************
* Function Name : RTOS_Init
* Inputs        : None
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Initializes the OS.
************************************************************************************/
void RTOS_Init (void)
{
  MSYS_voidInit(&Config);
  MSYS_CallBack(Timer_Callback);
}

/************************************************************************************
* Function Name : Scheduler
* Inputs        : None
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Organizes the task, which excutes before the other.
************************************************************************************/
void Scheduler (void)
{
  uint8 Local_counter=0;
  while(1)
  {
    while(Execution_Time==1)
    {
      Execution_Time = 0;
      for (Local_counter=0;Local_counter<MAX_TASKS_NUMBER;Local_counter++)
      {
        /* check if a task present in this handler number */
        if(SystemTasks[Local_counter].TaskHandler!=0)
        {
          if(SystemTasks[Local_counter].TSTATE == running)
          {
            /* Check if it's time has come */
            if (Timing[Local_counter] == 0)
            {
              /* Run the task */
              SystemTasks	[Local_counter].TaskHandler();
              Timing		[Local_counter] = SystemTasks[Local_counter].Periodicity;
            }
            else
            {
              /* Decrement the task timer */
              Timing[Local_counter]--;
            }
          }
          else
          {
            /* Task is suspended so do nothing */
          }
        }
        else
        {
          /* No Task present given a priority equals to the local counter */
        }
      }
      // Execution_Time=0;
    }
    
  }
}


/************************************************************************************
* Function Name : RTOS_DeleteTask
* Inputs        : uint8 priority
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Deletes a task if given correct task number.
************************************************************************************/

void RTOS_DeleteTask(uint8 priority)
{
  if(SystemTasks[priority].TaskHandler!=0)
  {
    /* All elements = 0 */
    SystemTasks[priority] = Empty;	
  }
  else
  {
    assert(Error_State(NO_TASK_GIVEN_TO_DELETE));
  }
}


/************************************************************************************
* Function Name : RTOS_SuspendTask
* Inputs        : uint8 priority
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Suspends a task if given correct task number.
************************************************************************************/

void RTOS_SuspendTask (uint8 priority)
{
  if(SystemTasks[priority].TaskHandler!=0)
  {
    /* All elements = 0 */
    SystemTasks[priority].TSTATE = suspend;	
  }
  else
  {
    assert(Error_State(NO_TASK_GIVEN_TO_SUSPEND));
  }
}


/************************************************************************************
* Function Name : RTOS_ResumeTask
* Inputs        : uint8 priority
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Resumes a task if given correct task number.
************************************************************************************/

void RTOS_ResumeTask (uint8 priority)
{
  if(SystemTasks[priority].TaskHandler!=0)
  {
    /* All elements = 0 */
    SystemTasks[priority].TSTATE = running;	
  }
  else
  {
    assert(Error_State(MAX_NUMBER_OF_TASKS_RESUME));
  }
}


/************************************************************************************
* Function Name : Error_State
* Inputs        : uint8 error_code
* Outputs       : boolean value (True or False)
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Returns True if an error was reported.
************************************************************************************/

bool Error_State(uint8 error_code)
{
  uint8 error=0;
  /* If there is no error */
  if(error_code==error)
  {
    return TRUE;
  }
  /* If there is an error */
  else
  {
    return error;
  }
  
}


/************************************************************************************
* Function Name : Timer_Callback
* Inputs        : None
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Asynchronous
* Description   : Call back function for systick interrupt that activates the execution time.
************************************************************************************/

void Timer_Callback(void)
{
  Execution_Time=1;
}