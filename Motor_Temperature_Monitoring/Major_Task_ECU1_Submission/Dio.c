/******************************************************************************
*                                                                             *
* Module: Dio                                                                 *
*                                                                             * 
* File Name: Dio.c                                                            *
*                                                                             *
* Description: Source file for TM4C123GH6PM Microcontroller - Dio Driver      *
*                                                                             *
* Author: Team9                                                               *
******************************************************************************/

#include "Dio.h"
#include "Dio_Regs.h"
#include <assert.h>

#if (DIO_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"

#endif

STATIC const Dio_ConfigChannel * Dio_PortChannels = NULL_PTR;
STATIC const  Dio_ConfigType * ConfigPtr = &Dio_Configuration;

/*******************************************************************************
* Service Name: Dio_WriteChannel
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): ChannelId - ID of DIO channel.
*                  Level - Value to be written.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to set a level of a channel.
********************************************************************************/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
  volatile uint32 * Port_Ptr = NULL_PTR;
  Dio_PortChannels = ConfigPtr->Channels;
  boolean error = FALSE;
  
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  /* check if the input configuration pointer is not a NULL_PTR */
  if (NULL_PTR == ConfigPtr)
  {
    Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_WRITE_CHANNEL_SID,
                    DIO_E_PARAM_POINTER);
  }
  else
  {
    /* No Action Required */
  }
  
  /* Check if the used channel is within the valid range */
  if (DIO_CONFIGURED_CHANNLES <= ChannelId)
  {
    
    Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
                    DIO_WRITE_CHANNEL_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
    error = TRUE;
  }
  else
  {
    /* No Action Required */
  }
#endif
  if(NULL_PTR == ConfigPtr)
  {
    assert(Dio_ErrorState(PTR_IS_NULL));
  }
  else
  {
  /* In-case there are no errors */
  if(FALSE == error)
  {
    /* Point to the correct PORT register according to the Port Id stored in the Port_Num member */
    switch(Dio_PortChannels[ChannelId].Port_Num)
    {
    case 0:    Port_Ptr = &GPIO_PORTA_DATA_REG;
    break;
    case 1:    Port_Ptr = &GPIO_PORTB_DATA_REG;
    break;
    case 2:    Port_Ptr = &GPIO_PORTC_DATA_REG;
    break;
    case 3:    Port_Ptr = &GPIO_PORTD_DATA_REG;
    break;
    case 4:    Port_Ptr = &GPIO_PORTE_DATA_REG;
    break;
    case 5:    Port_Ptr = &GPIO_PORTF_DATA_REG;
    break;
    }
    if(Level == STD_HIGH)
    {
      /* Write Logic High */
      SET_BIT(*Port_Ptr,Dio_PortChannels[ChannelId].Ch_Num);
    }
    else if(Level == STD_LOW)
    {
      /* Write Logic Low */
      CLEAR_BIT(*Port_Ptr,Dio_PortChannels[ChannelId].Ch_Num);
    }
  }
  else
  {
    /* No Action Required */
  }
  }
}

/*******************************************************************************
* Service Name: Dio_ReadChannel
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): ChannelId - ID of DIO channel.
* Parameters (inout): None
* Parameters (out): None
* Return value: Dio_LevelType
* Description: Function to return the value of the specified DIO channel.
********************************************************************************/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
  volatile uint32 * Port_Ptr = NULL_PTR;
  Dio_PortChannels = ConfigPtr->Channels;
  Dio_LevelType output = STD_LOW;
  boolean error = FALSE;
  
#if (DIO_DEV_ERROR_DETECT == STD_ON)
  /* check if the input configuration pointer is not a NULL_PTR */
  if (NULL_PTR == ConfigPtr)
  {
    Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, DIO_READ_CHANNEL_SID,
                    DIO_E_PARAM_POINTER);
  }
  else
  {
    /* No Action Required */
  }
  /*Check if the used channel is within the valid range */
  if (DIO_CONFIGURED_CHANNLES <= ChannelId)
  {
    
    Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID,
                    DIO_READ_CHANNEL_SID, DIO_E_PARAM_INVALID_CHANNEL_ID);
    error = TRUE;
  }
  else
  {
    /* No Action Required */
  }
#endif
  
  /* In-case there are no errors */
  if(FALSE == error)
  {
    /* Point to the correct PORT register according to the Port Id stored in the Port_Num member */
    switch(Dio_PortChannels[ChannelId].Port_Num)
    {
    case 0:    Port_Ptr = &GPIO_PORTA_DATA_REG;
    break;
    case 1:    Port_Ptr = &GPIO_PORTB_DATA_REG;
    break;
    case 2:    Port_Ptr = &GPIO_PORTC_DATA_REG;
    break;
    case 3:    Port_Ptr = &GPIO_PORTD_DATA_REG;
    break;
    case 4:    Port_Ptr = &GPIO_PORTE_DATA_REG;
    break;
    case 5:    Port_Ptr = &GPIO_PORTF_DATA_REG;
    break;
    }
    /* Read the required channel */
    if(BIT_IS_SET(*Port_Ptr,Dio_PortChannels[ChannelId].Ch_Num))
    {
      output = STD_HIGH;
    }
    else
    {
      output = STD_LOW;
    }
  }
  else
  {
    /* No Action Required */
  }
  return output;
}

/************************************************************************************
* Function Name : Dio_ErrorState
* Inputs        : uint8 error_code
* Outputs       : boolean value (True or False)
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Returns True if an error was reported.
************************************************************************************/
boolean Dio_ErrorState(uint8 error_code)
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
