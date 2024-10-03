/******************************************************************************
*
* Module: Communication
*
* File Name: Communication.h
*
* Description: header file for TM4C123GH6PM Microcontroller - Communication Driver.
*
* Author: Team 9
******************************************************************************/

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include "Platform_Types.h"



#define MAX_BYTE_NUM    8

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

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
void Comm_init(void);

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
void Comm_transmit_set(void);

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
void Comm_receive_set(void);

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
void Comm_transmit_data(sint8 data[MAX_BYTE_NUM]);

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
void Comm_receive_data(sint8 * Received);




#endif