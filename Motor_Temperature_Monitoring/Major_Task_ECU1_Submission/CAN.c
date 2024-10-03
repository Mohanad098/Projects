/******************************************************************************
*
* Module: CAN
*
* File Name: CAN.c
*
* Description: Source file for TM4C123GH6PM Microcontroller - CAN Driver.
*
* Author: Team 9
******************************************************************************/

#include "CAN.h"
#include "tm4c123gh6pm.h"
#include "Common_Macros.h"
#include <string.h>
#include "Det.h"
#include <assert.h>



STATIC const Can_Config_type * Configuration = NULL;
STATIC Can_StateInitType Can_Initialization = CAN_T_UNINIT;
STATIC Can_StateTransitionType Can_Status = CAN_T_STOP;
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
void CAN_Init(const Can_Config_type * ConfigPtr)
{   
  volatile unsigned long delay = 0; 
  
  volatile uint32 * Can_Base_Ptr = NULL_PTR;
  
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  if( Can_Initialization != CAN_T_UNINIT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_INIT_SID, CAN_E_TRANSITION);
  }
  else
  {
    /*Do Nothing*/     
  }
  /* check if the input configuration pointer is not a NULL_PTR */
  if (NULL_PTR == ConfigPtr)
  {
    
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_INIT_SID, CAN_E_PARAM_POINTER);
    
  }
  else
#endif
  {
  if(NULL_PTR == ConfigPtr)
  {
    assert(Can_ErrorState(PTR_IS_NULL));
  }
  else
  {
    /*This global pointer is global to be used by other functions to read the configuration structure*/
    Configuration = ConfigPtr;
    
    uint32 Msg;
    
    switch(Configuration -> can_num)
    {
    case CAN0:    Can_Base_Ptr = (volatile uint32 *)CAN0_BASE_ADDRESS;
    break;
    case CAN1:    Can_Base_Ptr = (volatile uint32 *)CAN1_BASE_ADDRESS;
    break;
    }
    
    /*Enable Clock for CAN Module*/
    SET_BIT(SYSCTL_RCGC0_R,Configuration-> can_num);       
    delay = SYSCTL_RCGC0_R; 
    
    /*Place CAN in Init Mode*/
    SET_BIT(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_CONTROL_REG_OFFSET),INIT_BIT);
    
    /*Wait while CAN Module is BUSY*/
    while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET),BUSY_BIT)){}
    
    *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CMASK_REG_OFFSET) =(CAN_IF1CMSK_WRNRD | CAN_IF1CMSK_ARB | CAN_IF1CMSK_CONTROL);
    
    *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_ARB2_REG_OFFSET) = 0;
    *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_MCTL_REG_OFFSET) = 0;
    
    for(Msg = FIRST_MSG; Msg <= MAX_MSG_NUM; Msg++)
    {
      
      /*Wait for busy bit to clear*/
      while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET),BUSY_BIT)){}
      
      /*Initiate programming the message object*/
      *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET) = Msg;
    }
    
    /*Make sure that new data flags are updated for the message objects.*/
    *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CMASK_REG_OFFSET) = CAN_IF1CMSK_NEWDAT;
    
    
    /*Setting the CCE bit to edit CANBIT Register*/
    SET_BIT(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_CONTROL_REG_OFFSET),CONFIG_CHANGE_ENABLE_BIT);
    /*Bit rate setting*/
    *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_BIT_REG_OFFSET) = Configuration-> bit_rate; 
    
   
    Can_Initialization = CAN_T_INIT;   
    Can_Status = CAN_T_STOP;
  }
  } 
}

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
void Can_SetControllerMode(Can_num_type Controller,Can_StateTransitionType Transition )
{
  uint32 Msg;
  
  volatile unsigned long delay = 0; 
  
  volatile uint32 * Can_Base_Ptr = NULL_PTR;
  
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  if( Can_Initialization == CAN_T_UNINIT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_SETCONTROLLERMODE_SID, CAN_E_UNINIT);
  }
  else
  {
    /*Do Nothing*/     
  }
  if(Controller == CAN0 || Controller == CAN1)
  {
    /*Do Nothing*/  
  }
  else
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_SETCONTROLLERMODE_SID, CAN_E_PARAM_CONTROLLER);   
  }
  if(Transition == CAN_T_START || Transition == CAN_T_STOP)
  {
    /*Do Nothing*/  
  }
  else
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_SETCONTROLLERMODE_SID, CAN_E_TRANSITION);       
  }
#endif
  
  switch(Transition)
  {
  case CAN_T_START:
    
    switch(Controller)
    {
    case CAN0:    Can_Base_Ptr = (volatile uint32 *)CAN0_BASE_ADDRESS;
    break;
    case CAN1:    Can_Base_Ptr = (volatile uint32 *)CAN1_BASE_ADDRESS;
    break;
    }
    
        /*Enable Clock for CAN Module*/
        delay = SYSCTL_RCGC0_R;
        SET_BIT(SYSCTL_RCGC0_R,Configuration-> can_num);       
        
        /*Place CAN in Init Mode*/
        SET_BIT(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_CONTROL_REG_OFFSET),INIT_BIT);
        
        /*Wait while CAN Module is BUSY*/
        while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET),BUSY_BIT)){}
        
        *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CMASK_REG_OFFSET) =(CAN_IF1CMSK_WRNRD | CAN_IF1CMSK_ARB | CAN_IF1CMSK_CONTROL);
        
        *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_ARB2_REG_OFFSET) = 0;
        *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_MCTL_REG_OFFSET) = 0;
        
        for(Msg = FIRST_MSG; Msg <= MAX_MSG_NUM; Msg++)
        {
          
          /*Wait for busy bit to clear*/
          while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET),BUSY_BIT)){}
          
          /*Initiate programming the message object*/
          *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET) = Msg;
        }
        
        /*Make sure that new data flags are updated for the message objects.*/
        *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CMASK_REG_OFFSET) = CAN_IF1CMSK_NEWDAT;
        
        
        /*Setting the CCE bit to edit CANBIT Register*/
        SET_BIT(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_CONTROL_REG_OFFSET),CONFIG_CHANGE_ENABLE_BIT);
        /*Bit rate setting*/
        *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_BIT_REG_OFFSET) = Configuration-> bit_rate; 
        
    /*CAN is enabled (Init = 0)*/
    CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_CONTROL_REG_OFFSET),INIT_BIT); 
    CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_CONTROL_REG_OFFSET),CONFIG_CHANGE_ENABLE_BIT); 
    
    Can_Status = CAN_T_START;
    break;
  case CAN_T_STOP:
    /*Place CAN in Init Mode*/
    SET_BIT(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_CONTROL_REG_OFFSET),INIT_BIT);
    
    Can_Status = CAN_T_STOP;
    break;
    
  }
  
} 


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
void Can_ChangeBaudrate(Can_num_type Controller, const uint16 Baudrate)
{
  volatile uint32 * Can_Base_Ptr = NULL_PTR;
  
#if (CAN_DEV_ERROR_DETECT == STD_ON)
  if( Can_Initialization == CAN_T_UNINIT)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_CHANGEBAUDRATE_SID, CAN_E_UNINIT);
  }
  else
  {
    /*Do Nothing*/     
  }
  if(Controller == CAN0 || Controller == CAN1)
  {
    /*Do Nothing*/  
  }
  else
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_CHANGEBAUDRATE_SID, CAN_E_PARAM_CONTROLLER);
  }
  if(Can_Status != CAN_T_STOP)
  {
    Det_ReportError(CAN_MODULE_ID, CAN_INSTANCE_ID, CAN_CHANGEBAUDRATE_SID, CAN_E_TRANSITION);
  }
  else
  {
    /*Do Nothing*/      
  }
#endif
  switch(Controller)
  {
  case CAN0:    Can_Base_Ptr = (volatile uint32 *)CAN0_BASE_ADDRESS;
  break;
  case CAN1:    Can_Base_Ptr = (volatile uint32 *)CAN1_BASE_ADDRESS;
  break;
  }
  
  /*Setting the CCE bit to edit CANBIT Register*/
  SET_BIT(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_CONTROL_REG_OFFSET),CONFIG_CHANGE_ENABLE_BIT);
  /*Bit rate setting*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_BIT_REG_OFFSET) = Configuration-> bit_rate; 
  
}

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
void CAN_Transmit_Set(void) 
{

  uint32 CmdMaskReg = 0;
  uint32 ArbReg0 = 0;
  uint32 ArbReg1 = 0;
  uint32 MsgCtrl = 0;
  uint32 MaskReg0 = 0;
  uint32 MaskReg1 = 0;
  
  volatile uint32 * Can_Base_Ptr = NULL_PTR;
  
  switch(Configuration -> can_num)
  {
  case CAN0:    Can_Base_Ptr = (volatile uint32 *)CAN0_BASE_ADDRESS;
  break;
  case CAN1:    Can_Base_Ptr = (volatile uint32 *)CAN1_BASE_ADDRESS;
  break;
  }
  
  /*Wait while CAN Module is BUSY*/
  while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET),BUSY_BIT)){}
  
  /*Setting WRNRD, CONTROL, DATAA, DATAB*/
  CmdMaskReg |= (CAN_IF1CMSK_WRNRD | CAN_IF1CMSK_DATAA | CAN_IF1CMSK_DATAB | CAN_IF1CMSK_CONTROL); 
  
  /*Setting TXRQST bit */
  MsgCtrl |= CAN_IF1MCTL_TXRQST;
  
  /*setting DIR bit in ARB2 reg*/
  ArbReg1 = CAN_IF1ARB2_DIR; 
  
  /* Set the Arb bit in CMSK register so that this gets transferred to the Message object.*/
  CmdMaskReg |= CAN_IF1CMSK_ARB;
  
  /* MSG ID set in ARB2 reg*/
  ArbReg1 |= ((Configuration -> Msg_ID) << SHIFT_2_BITS);
  
  /*MSG Valid bit set in ARB2 reg*/
  ArbReg1 |= CAN_IF1ARB2_MSGVAL;
  
  /*Set the data length since this is set for all transfers.  This is also a
    single transfer and not a FIFO transfer so set EOB bit.*/
  MsgCtrl |= CAN_IF1MCTL_EOB | (Configuration -> Data_Length);
  
  
  /*Updating registers */
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CMASK_REG_OFFSET) = CmdMaskReg; 
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_MSK1_REG_OFFSET) = MaskReg0;
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_MSK2_REG_OFFSET) = MaskReg1;
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_ARB1_REG_OFFSET) = ArbReg0;
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_ARB2_REG_OFFSET) = ArbReg1;
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_MCTL_REG_OFFSET) = MsgCtrl;
  
  /*Transfer the message object to the message object specified object number*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET) = (Configuration -> obj_num);
}

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
void CAN_Transmit_Data(sint8 data[MAX_BYTE_NUM])
{
 
  volatile uint32 * Can_Base_Ptr = NULL_PTR;
  
  switch(Configuration -> can_num)
  {
  case CAN0:    Can_Base_Ptr = (volatile uint32 *)CAN0_BASE_ADDRESS;
  break;
  case CAN1:    Can_Base_Ptr = (volatile uint32 *)CAN1_BASE_ADDRESS;
  break;
  }

  /*Clear ARB and CONTROL bits in CMSK reg*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CMASK_REG_OFFSET) &= ~( CAN_IF1CMSK_ARB | CAN_IF1CMSK_CONTROL); 
  /*Set WRNRD, DATAA, DATAB bits in CMSK*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CMASK_REG_OFFSET) |= (CAN_IF1CMSK_WRNRD | CAN_IF1CMSK_DATAA | CAN_IF1CMSK_DATAB); 
  /*Write the data out to the CAN Data registers*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_DA1_REG_OFFSET) = ((data[1]<<SHIFT_8_BITS)+data[0]); 
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_DA2_REG_OFFSET) = ((data[3]<<SHIFT_8_BITS)+data[2]);
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_DB1_REG_OFFSET) = ((data[5]<<SHIFT_8_BITS)+data[4]);
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_DB2_REG_OFFSET)= ((data[7]<<SHIFT_8_BITS)+data[6]);
  
  /*Set the NEWDAT and TXRQST bit in CMSK reg*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CMASK_REG_OFFSET) |= (CAN_IF1CMSK_WRNRD | CAN_IF1CMSK_DATAA | CAN_IF1CMSK_DATAB | CAN_IF1CMSK_TXRQST);  
  
  /*Transfer the message object to the message object specified object number*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET) = (Configuration -> obj_num); 
  
 /*Wait while CAN Module is BUSY*/
  while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF1_CRQ_REG_OFFSET),BUSY_BIT)){}
  
} 


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
void CAN_Receive_Set(void)
{
  uint32 CmdMaskReg = 0;
  uint32 ArbReg0 = 0;
  uint32 ArbReg1 = 0;
  uint32 MsgCtrl = 0;
  uint32 MaskReg0 = 0;
  uint32 MaskReg1 = 0;
  
  volatile uint32 * Can_Base_Ptr = NULL_PTR;
  
  switch(Configuration -> can_num)
  {
  case CAN0:    Can_Base_Ptr = (volatile uint32 *)CAN0_BASE_ADDRESS;
  break;
  case CAN1:    Can_Base_Ptr = (volatile uint32 *)CAN1_BASE_ADDRESS;
  break;
  }
  
 /*Wait while CAN Module is BUSY*/
  while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_CRQ_REG_OFFSET),BUSY_BIT)){}
  
   /*Setting WRNRD, CONTROL, MASK, DATAA, DATAB */
  CmdMaskReg |= (CAN_IF2CMSK_WRNRD | CAN_IF2CMSK_CONTROL| CAN_IF2CMSK_MASK | CAN_IF2CMSK_DATAA |
                                  CAN_IF2CMSK_DATAB); 
  
  /*Clear DIR bit in ARB2 register to receive*/ 
  ArbReg1 &= ~(CAN_IF2ARB2_DIR);

  
  /*ARB bit set in CMSK register */
  CmdMaskReg |= CAN_IF2CMSK_ARB; 
  
  /* MSG ID set in ARB2 reg*/
  ArbReg1 |= (Configuration -> Msg_ID);

  
  /*Setting MSG Valid bit in ARB2 register*/
  ArbReg1 |= CAN_IF2ARB2_MSGVAL; 
  
  /*Set no. of data bytes in MCTL Reg, set EOB  and UMASK .*/ 
  MsgCtrl |= CAN_IF2MCTL_EOB | CAN_IF2MCTL_UMASK | (Configuration -> Data_Length);
  
  /*Updating registers */
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_CMASK_REG_OFFSET) = CmdMaskReg; 
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_MSK1_REG_OFFSET) = MaskReg0;
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_MSK2_REG_OFFSET) = MaskReg1;
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_ARB1_REG_OFFSET) = ArbReg0;
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_ARB2_REG_OFFSET) = ArbReg1;
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_MCTL_REG_OFFSET) = MsgCtrl;
  
    /*Transfer the message object to the message object specified object number*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_CRQ_REG_OFFSET) = (Configuration -> obj_num);
  
  /*Wait while CAN Module is BUSY*/
  while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_CRQ_REG_OFFSET),BUSY_BIT)){}

}

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
void CAN_Receive_Data(sint8 * Received)
{
 volatile uint32 ArbReg0 = 0;
  volatile uint32 ArbReg1 = 0;
  uint32 MsgCtrl = 0;
  volatile uint32 MaskReg0 = 0;
  volatile uint32 MaskReg1 = 0;
  sint8 data[MAX_BYTE_NUM];  
  
  
  volatile uint32 * Can_Base_Ptr = NULL_PTR;
  
  switch(Configuration -> can_num)
  {
  case CAN0:    Can_Base_Ptr = (volatile uint32 *)CAN0_BASE_ADDRESS;
  break;
  case CAN1:    Can_Base_Ptr = (volatile uint32 *)CAN1_BASE_ADDRESS;
  break;
  }
  
  /*Setting ARB, CONTROL, MASK, DATAA, DATAB bits */
  CAN0_IF2CMSK_R = ( CAN_IF2CMSK_ARB | CAN_IF2CMSK_CONTROL| CAN_IF2CMSK_MASK | CAN_IF2CMSK_DATAA |
                                  CAN_IF2CMSK_DATAB);  
  
   /*Transfer the message object to the message object specified object number*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_CRQ_REG_OFFSET) = (Configuration -> obj_num);
  
   /*Wait while CAN Module is BUSY*/
  while(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_CRQ_REG_OFFSET),BUSY_BIT)){}
  
  /*Get data from registers*/
  MaskReg0 = *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_MSK1_REG_OFFSET);
  MaskReg1 = *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_MSK2_REG_OFFSET);
  ArbReg0 = *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_ARB1_REG_OFFSET);
  ArbReg1 = *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_ARB2_REG_OFFSET);
  MsgCtrl = *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_MCTL_REG_OFFSET);
  
  /*Check If NEWDAT bit is set to get new data in the data registers*/
  if(MsgCtrl & CAN_IF2MCTL_NEWDAT) 
  {
    data[0] = (0x00FF & (*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_DA1_REG_OFFSET))); 
    data[1] = ((0xFF00 & (*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_DA1_REG_OFFSET)))>>SHIFT_8_BITS);
    data[2] = (0x00FF & (*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_DA2_REG_OFFSET)));
    data[3] = ((0xFF00 & (*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_DA2_REG_OFFSET)))>>SHIFT_8_BITS);
    data[4] = (0x00FF & (*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_DB1_REG_OFFSET)));
    data[5] = ((0xFF00 & (*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_DB1_REG_OFFSET)))>>SHIFT_8_BITS);
    data[6] = (0x00FF & (*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_DB2_REG_OFFSET)));
    data[7] = ((0xFF00 & (*(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_DB2_REG_OFFSET)))>>SHIFT_8_BITS);
    
    /*Set NEWDAT in CMSK to Clear the NEWDAT bit in MCTL*/
   *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_CMASK_REG_OFFSET) = CAN_IF2CMSK_NEWDAT;
  }
  /*If MSGLST bit is set i.e. there was a message lost*/
  if(MsgCtrl & CAN_IF2MCTL_MSGLST) 
  {
    /*Clear the MSGLST bit*/
    MsgCtrl &= ~(CAN_IF2MCTL_MSGLST); 
    *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_MCTL_REG_OFFSET) = MsgCtrl; 
   //printf("Message lost\n");
  }
  /*Note: when the message object number is written the values in register (MCTL, ARB etc.) are updated or transfered.*/
  *(volatile uint32*)((volatile uint8*) Can_Base_Ptr + CAN_IF2_CRQ_REG_OFFSET) = (Configuration -> obj_num);
  
  strcpy(Received, data);


}

/************************************************************************************
* Function Name : Can_ErrorState
* Inputs        : uint8 error_code
* Outputs       : boolean value (True or False)
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Returns True if an error was reported.
************************************************************************************/
boolean Can_ErrorState(uint8 error_code)
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