/******************************************************************************
*
* Module: Communication
*
* File Name: Communication.c
*
* Description: Source file for TM4C123GH6PM Microcontroller - Communication Driver.
*
* Author: Team 9
******************************************************************************/

#include "Communication.h"
#include "CAN.h"

/*******************************************************************************
* Service Name: Comm_init
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes Communication Modules.
********************************************************************************/
void Comm_init(void)
{
  CAN_Init(&Can_configuration);
  Can_SetControllerMode(CAN0,CAN_T_START);
}

/*******************************************************************************
* Service Name: Comm_transmit_set
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Adjusts Communication Modules Transmit Settings.
********************************************************************************/
void Comm_transmit_set(void)
{
  CAN_Transmit_Set();  
}

/*******************************************************************************
* Service Name: Comm_receive_set
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Adjusts Communication Modules Receive Settings.
********************************************************************************/
void Comm_receive_set(void)
{
  CAN_Receive_Set(); 
}

/*******************************************************************************
* Service Name: Comm_transmit_data
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): sint8 data[MAX_BYTE_NUM]: Array of sint8
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: This function actually sends the data.
********************************************************************************/
void Comm_transmit_data(sint8 data[MAX_BYTE_NUM])
{
  CAN_Transmit_Data(data);
}

/*******************************************************************************
* Service Name: Comm_receive_data
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initialise the Receive message.
********************************************************************************/
void Comm_receive_data(sint8 * Received)
{
  CAN_Receive_Data(Received);  
}

