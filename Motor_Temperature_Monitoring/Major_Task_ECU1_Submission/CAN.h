/******************************************************************************
*
* Module: CAN
*
* File Name: CAN.h
*
* Description: Header file for TM4C123GH6PM Microcontroller - CAN Driver.
*
* Author: Team9
******************************************************************************/

#ifndef _CAN_H
#define _CAN_H

#include "Std_Types.h"
#include "CAN_Cfg.h"

/* Id for the company in the AUTOSAR */
#define CAN_VENDOR_ID    (7000U)

/* CAN Module Id */
#define CAN_MODULE_ID    (80U)

/* CAN Instance Id */
#define CAN_INSTANCE_ID  (0U)




#define MAX_BYTE_NUM    8
#define MAX_MSG_NUM     32
#define FIRST_MSG       1
#define SHIFT_2_BITS    2
#define SHIFT_8_BITS    8
/*******************************************************************************
*                              Module Definitions                              *
********************************************************************************/

/* CAN Registers base addresses */
#define CAN0_BASE_ADDRESS           0x40040000
#define CAN1_BASE_ADDRESS           0x40041000

/* CAN Registers offset addresses */
#define CAN_CONTROL_REG_OFFSET              0x000
#define CAN_STATUS_REG_OFFSET               0x004
#define CAN_BIT_REG_OFFSET                  0x00C
#define CAN_TEST_REG_OFFSET                 0x014

#define CAN_IF1_CRQ_REG_OFFSET              0x020   
#define CAN_IF1_CMASK_REG_OFFSET            0x024
#define CAN_IF1_MSK1_REG_OFFSET             0x028
#define CAN_IF1_MSK2_REG_OFFSET             0x02C
#define CAN_IF1_ARB1_REG_OFFSET             0x030
#define CAN_IF1_ARB2_REG_OFFSET             0x034
#define CAN_IF1_MCTL_REG_OFFSET             0x038

#define CAN_IF1_DA1_REG_OFFSET              0x03C
#define CAN_IF1_DA2_REG_OFFSET              0x040
#define CAN_IF1_DB1_REG_OFFSET              0x044
#define CAN_IF1_DB2_REG_OFFSET              0x048

#define CAN_IF2_CRQ_REG_OFFSET              0x080
#define CAN_IF2_CMASK_REG_OFFSET            0x084
#define CAN_IF2_MSK1_REG_OFFSET             0x088
#define CAN_IF2_MSK2_REG_OFFSET             0x08C
#define CAN_IF2_ARB1_REG_OFFSET             0x090
#define CAN_IF2_ARB2_REG_OFFSET             0x094
#define CAN_IF2_MCTL_REG_OFFSET             0x098

#define CAN_IF2_DA1_REG_OFFSET              0x09C
#define CAN_IF2_DA2_REG_OFFSET              0x0A0
#define CAN_IF2_DB1_REG_OFFSET              0x0A4
#define CAN_IF2_DB2_REG_OFFSET              0x0A8
   

#define INIT_BIT                            0
#define CONFIG_CHANGE_ENABLE_BIT            6 
#define BUSY_BIT                            15 



typedef enum
{
  CAN_OK,
  CAN_NOT_OK,
  CAN_BUSY
}Can_ReturnType;

typedef enum
{
  CAN_T_START,
  CAN_T_STOP,
}Can_StateTransitionType;

typedef enum
{
  CAN_T_UNINIT,
  CAN_T_INIT
}Can_StateInitType;

#define PTR_IS_NULL                     1

/* Errors */
#define CAN_E_PARAM_POINTER 		0x01
#define CAN_E_PARAM_DLC 		0x03
#define CAN_E_PARAM_CONTROLLER 	        0x04
#define CAN_E_UNINIT 			0x05
#define CAN_E_TRANSITION		0x06


/* Functions service ID */
#define CAN_INIT_SID			0x00
#define CAN_CHANGEBAUDRATE_SID		0x0d
#define CAN_SETCONTROLLERMODE_SID	0x03
#define CAN_WRITE_SID			0x06

/*******************************************************************************
*                              Module Data Types                               *
********************************************************************************/
typedef enum 
{
  CAN0 = 24 , CAN1 = 25
}Can_num_type;

typedef enum 
{
  DATA_0_BYTE, DATA_1_BYTE, DATA_2_BYTE,DATA_3_BYTE, DATA_4_BYTE,\
    DATA_5_BYTE,DATA_6_BYTE,DATA_7_BYTE,DATA_8_BYTE
}Data_len_type;

typedef enum 
{
  BR_1MBPS = 0x0201 , BR_500KBPS = 0x0203
}Bit_rate_type;

typedef enum
{
  OBJ_NUM_1 = 1,
  OBJ_NUM_2,
  OBJ_NUM_3,
  OBJ_NUM_4,
  OBJ_NUM_5,
  OBJ_NUM_6,
  OBJ_NUM_7,
  OBJ_NUM_8,
  OBJ_NUM_9,
  OBJ_NUM_10,
  OBJ_NUM_11,
  OBJ_NUM_12,
  OBJ_NUM_13,
  OBJ_NUM_14,
  OBJ_NUM_15,
  OBJ_NUM_16,
  OBJ_NUM_17,
  OBJ_NUM_18,
  OBJ_NUM_19,
  OBJ_NUM_20,
  OBJ_NUM_21,
  OBJ_NUM_22,
  OBJ_NUM_23,
  OBJ_NUM_24,
  OBJ_NUM_25,
  OBJ_NUM_26,
  OBJ_NUM_27,
  OBJ_NUM_28,
  OBJ_NUM_29,
  OBJ_NUM_30,
  OBJ_NUM_31,
  OBJ_NUM_32
}Obj_num_type;
typedef struct
{
  Can_num_type can_num;
  
  /* The CAN message identifier used for 11 or 29 bit identifiers. */
  uint32 Msg_ID;
  
  Bit_rate_type bit_rate;
  
  Obj_num_type obj_num;
  
  /* This value is the number of bytes of data in the message object.*/
  Data_len_type Data_Length; 
  
}Can_Config_type;




/*******************************************************************************
*                      Function Prototypes                                     *
********************************************************************************/

/*******************************************************************************
* Service Name: CAN_Init
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the CAN Driver module.
********************************************************************************/
void CAN_Init(const Can_Config_type * ConfigPtr);

/*******************************************************************************
* Service Name: Can_SetControllerMode
* Service ID: 0x03
* Sync/Async: Asynchronous
* Reentrancy: Non-reentrant
* Parameters (in):  Controller: CAN controller for which the status shall be changed
*                   Transition: Transition value to request new CAN controller state
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: This function performs software triggered state transitions of the CAN controller State machine.
********************************************************************************/
void Can_SetControllerMode(Can_num_type Controller,Can_StateTransitionType Transition );

/*******************************************************************************
* Service Name: Can_ChangeBaudrate
* Service ID: 0x0d
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in):  Controller: CAN Controller, whose baudrate shall be changed
*                   Baudrate: Requested baudrate in kbps
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: This service shall change the baudrate of the CAN controller.
********************************************************************************/
void Can_ChangeBaudrate(Can_num_type Controller, const uint16 Baudrate);

/*******************************************************************************
* Service Name: CAN_Transmit_Set
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the transmit message object.
********************************************************************************/
void CAN_Transmit_Set(void);

/*******************************************************************************
* Service Name: CAN_Transmit_Data
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): sint8 data[MAX_BYTE_NUM]: Array of sint8
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: This function actually sends the data on the TX pin.
********************************************************************************/
void CAN_Transmit_Data(sint8 data[MAX_BYTE_NUM]);

/*******************************************************************************
* Service Name: CAN_Receive_Set
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initialise the Receive message object.
********************************************************************************/
void CAN_Receive_Set(void);

/*******************************************************************************
* Service Name: CAN_Receive_Data
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): sint8 * Received: Pointer to Array of sint8
* Parameters (out): None
* Return value: None
* Description: Since registers have already been initialised this function receives data.
********************************************************************************/
void CAN_Receive_Data( sint8 * Received);

/************************************************************************************
* Function Name : Can_ErrorState
* Inputs        : uint8 error_code
* Outputs       : boolean value (True or False)
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Returns True if an error was reported.
************************************************************************************/
boolean Can_ErrorState(uint8 error_code);

/* Extern structures to be used by Can and other modules */
extern const Can_Config_type Can_configuration;


#endif