/*******************************************************************************
*                                                                              *
* Module: Port                                                                 *
*                                                                              *
* File Name: Port.c                                                            *
*                                                                              *
* Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.     *
*                                                                              *
* Author: Team9                                                                *  
********************************************************************************/

#include "Port.h"
#include "tm4c123gh6pm.h"
#include <assert.h>

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
|| (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
  || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const Port_ConfigChannel * Port_PortChannels = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/*******************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
********************************************************************************/
void Port_Init(const Port_ConfigType * ConfigPtr)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* check if the input configuration pointer is not a NULL_PTR */
  if (NULL_PTR == ConfigPtr)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,
                    PORT_E_PARAM_CONFIG);
  }
  else
  {
    
  }
#endif
  if(NULL_PTR == ConfigPtr)
  {
    assert(Port_ErrorState(PTR_IS_NULL));
  }
  else
  {
    
    volatile unsigned long delay = 0;    
    volatile uint32 * Port_Ptr = NULL_PTR;
    
    Port_PinType ChannelId;
    /*
    * This global pointer is global to be used by other functions to read the PB configuration structures
    */ 
    Port_PortChannels = ConfigPtr->Channels; /* address of the first Channels structure --> Channels[0] */
    
    for(ChannelId = ZERO; ChannelId < PORT_CONFIGURED_CHANNLES; ChannelId++)
    {   
      switch(Port_PortChannels[ChannelId].port_num)
      {
      case PORT_A_ID:    Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
      break;
      case PORT_B_ID:    Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
      break;
      case PORT_C_ID:    Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
      break;
      case PORT_D_ID:    Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
      break;
      case PORT_E_ID:    Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
      break;
      case PORT_F_ID:    Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
      break;
      }
      
      /* Enable clock for PORT and allow time for clock to start*/
      SET_BIT(SYSCTL_RCGC2_R,Port_PortChannels[ChannelId].port_num);
      delay = SYSCTL_RCGC2_R;
      
      
      if (((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&(PIN_7_ID == Port_PortChannels[ChannelId].pin_num))\
        ||((PORT_F_ID == Port_PortChannels[ChannelId].port_num)&&(PIN_0_ID == Port_PortChannels[ChannelId].pin_num)))
      {
        // Put the unlcok number (0X4C4F434B) in GPIOLOCK register to enable write access to GPIOCR register
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_LOCK_REG_OFFSET) = UNLOCK;
        
        //Set the corresponding bit in the GPIOAFSEL register to disable alternative function on this pin 
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_COMMIT_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
      }
      else if ((PORT_C_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num <= PIN_3_ID))
      {
        //Do Nothing to protect JTAG pins from any changes
      }
      else 
      {
        //Do Nothing other pins does not need to be unlcoked or to set the commit bit
      }
      
      switch (Port_PortChannels[ChannelId].pin_mode)
      {
        
      case PORT_PIN_MODE_DIO:
        
        /*Clear the corresponding bit in the GPIOAFSEL register to disable alternative function on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num); 
        
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        
        /*Clear the corresponding bits in the GPIOCTL register to use it as GPIO pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) &=~(0x0000000F << (Port_PortChannels[ChannelId].pin_num * 4));
        
        break;
        
      case PORT_PIN_MODE_ADC:
        
        if(((PORT_B_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num)))\
          ||((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num <= PIN_3_ID))\
            ||((PORT_E_ID == Port_PortChannels[ChannelId].port_num)))
        {
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAMSEL register to enable analog on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Clear the PMCx in the GPIODEN register to disable digital on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        }
        else
        {
          /*Do Nothing mode is not available for other pins*/
        }
        break;
        
      case PORT_PIN_MODE_CAN:
        
        if((PORT_F_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_0_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_3_ID == Port_PortChannels[ChannelId].pin_num)))
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCx in the GPIOCTL register to use it as CAN pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Port_PortChannels[ChannelId].pin_num * 4));
        }
        else if(((PORT_A_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_0_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_1_ID == Port_PortChannels[ChannelId].pin_num)))\
          ||((PORT_B_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num)))\
            ||((PORT_E_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num))))
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as CAN pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_PortChannels[ChannelId].pin_num * 4));
        }
        else
        {
          /*Do Nothing mode is not available for other pins*/
        }
        break;
        
      case PORT_PIN_MODE_DIO_GPT:
        
        if((PORT_B_ID == Port_PortChannels[ChannelId].port_num)||(PORT_F_ID == Port_PortChannels[ChannelId].port_num))
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as timer pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (Port_PortChannels[ChannelId].pin_num * 4));
        }
        else
        {
          /*Do Nothing mode is not available for other pins*/
        }
        break;
        
      case PORT_PIN_MODE_DIO_WDG:
        
        if(((PORT_C_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num >= PIN_4_ID))||(PORT_D_ID == Port_PortChannels[ChannelId].port_num))
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as watchdog pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (Port_PortChannels[ChannelId].pin_num * 4));
        }
        else
        {
          /*Do Nothing mode is not available for other pins*/
        }
        break;
        
      case PORT_PIN_MODE_USB:
        
        if(((PORT_C_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_6_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_7_ID == Port_PortChannels[ChannelId].pin_num)))\
          ||((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_2_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_3_ID == Port_PortChannels[ChannelId].pin_num)))\
            ||((PORT_F_ID == Port_PortChannels[ChannelId].port_num)&& PIN_4_ID == Port_PortChannels[ChannelId].pin_num))
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as USB pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_PortChannels[ChannelId].pin_num * 4));
          
        }
        else 
        {
          /*Do Nothing mode is not available for other pins*/
        }
        break;
        
      case PORT_PIN_MODE_I2C:
        
        if(((PORT_A_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_6_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_7_ID == Port_PortChannels[ChannelId].pin_num)))\
          ||((PORT_B_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_2_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_3_ID == Port_PortChannels[ChannelId].pin_num)))\
            ||((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_0_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_1_ID == Port_PortChannels[ChannelId].pin_num)))\
              ||((PORT_E_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num))))
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as I2C pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Port_PortChannels[ChannelId].pin_num * 4));
        }
        else
        {
          /*Do Nothing mode is not available for other pins*/
        }
        break;
        
      case PORT_PIN_MODE_PWM:
        
        if((PORT_B_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num >= PIN_4_ID)\
          ||((PORT_C_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num)))\
            ||((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num <= PIN_1_ID)\
              &&((INSTANCE_6 == Port_PortChannels[ChannelId].pin_mode_instance)||(INSTANCE_7 == Port_PortChannels[ChannelId].pin_mode_instance)))\
                ||((PORT_E_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num))\
                  &&((INSTANCE_4 == Port_PortChannels[ChannelId].pin_mode_instance)||(INSTANCE_5 == Port_PortChannels[ChannelId].pin_mode_instance))))
        {    
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as PWM pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (Port_PortChannels[ChannelId].pin_num * 4));
        }
        else if((PORT_A_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_6_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_7_ID == Port_PortChannels[ChannelId].pin_num))\
          ||((PORT_E_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num)))\
            ||((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num <= PIN_1_ID)\
              &&((INSTANCE_0 == Port_PortChannels[ChannelId].pin_mode_instance)||(INSTANCE_1 == Port_PortChannels[ChannelId].pin_mode_instance)))
              ||((PORT_F_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num <= PIN_3_ID)))  
        {    
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as PWM pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005 << (Port_PortChannels[ChannelId].pin_num * 4));
        }
        else
        {
          /*Do Nothing mode is not available for other pins*/
        }
        break;
        
      case PORT_PIN_MODE_SSI:
        
        if(((PORT_A_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_2_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_3_ID == Port_PortChannels[ChannelId].pin_num)\
          ||(PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num)))\
            ||((PORT_B_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num >= PIN_4_ID))\
              ||((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num <= PIN_3_ID)&&(INSTANCE_3 == Port_PortChannels[ChannelId].pin_mode_instance)))
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as SSI pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (Port_PortChannels[ChannelId].pin_num * 4));
          
        }
        else if (((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num <= PIN_3_ID)\
          &&(INSTANCE_1 == Port_PortChannels[ChannelId].pin_mode_instance))\
            ||((PORT_F_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num <= PIN_3_ID)))
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the PMCxin the GPIOCTL register to use it as SSI pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Port_PortChannels[ChannelId].pin_num * 4));
          
        }
        else
        {
          /*Do Nothing mode is not available for other pins*/
        }
        break;
        
      case PORT_PIN_MODE_UART:
        
        if(((PORT_A_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_0_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_1_ID == Port_PortChannels[ChannelId].pin_num)))\
          ||((PORT_B_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_0_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_1_ID == Port_PortChannels[ChannelId].pin_num)))\
            ||((PORT_C_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num >= PIN_4_ID))\
              ||((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&(Port_PortChannels[ChannelId].pin_num >= PIN_4_ID))\
                ||((PORT_E_ID == Port_PortChannels[ChannelId].port_num)&&((Port_PortChannels[ChannelId].pin_num <= PIN_1_ID)||(Port_PortChannels[ChannelId].pin_num >= PIN_4_ID)))\
                  ||((PORT_F_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_0_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_1_ID == Port_PortChannels[ChannelId].pin_num))))        
        {
          /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          
          
          if((PORT_C_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)\
            ||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num))&&(INSTANCE_1 == Port_PortChannels[ChannelId].pin_mode_instance))
          {
            /*Set the PMCxin the GPIOCTL register to use it as UART pin */
            *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (Port_PortChannels[ChannelId].pin_num * 4) );
          }
          else if ((PORT_C_ID == Port_PortChannels[ChannelId].port_num)&&((PIN_4_ID == Port_PortChannels[ChannelId].pin_num)||(PIN_5_ID == Port_PortChannels[ChannelId].pin_num))\
            &&((INSTANCE_U1RTS == Port_PortChannels[ChannelId].pin_mode_instance)||(INSTANCE_U1CTS == Port_PortChannels[ChannelId].pin_mode_instance)))
          {
            /*Set the PMCxin the GPIOCTL register to use it as UART pin */
            *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_PortChannels[ChannelId].pin_num * 4) );
          }
          
          else
          {
            /*Set the PMCxin the GPIOCTL register to use it as UART pin */
            *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (Port_PortChannels[ChannelId].pin_num * 4) );
          }
        }
        else
        {
          /*Do Nothing mode is not available for other pins*/
        }
        
        break;
        
      case PORT_PIN_INITIAL_MODE_NMI:
        
        if(((PORT_D_ID == Port_PortChannels[ChannelId].port_num)&&(PIN_7_ID == Port_PortChannels[ChannelId].pin_num))\
          ||((PORT_F_ID == Port_PortChannels[ChannelId].port_num)&&(PIN_0_ID == Port_PortChannels[ChannelId].pin_num)))                                                      
        {
          /*Set the PMCxin the GPIOCTL register to use it as NMI pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_PortChannels[ChannelId].pin_num * 4) );
        }
        else 
        {
          /*Do Nothing mode is not available for other pins*/
        }
        
        break;
      }
      if (PORT_PIN_IN == Port_PortChannels[ChannelId].pin_direction)
      {
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        if (OFF == Port_PortChannels[ChannelId].pin_resistor)
        {
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_PULL_DOWN_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_PULL_UP_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        }
        else if(PULL_UP == Port_PortChannels[ChannelId].pin_resistor)
        {
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_PULL_UP_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        }
        else
        {
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_PULL_DOWN_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        }   
      }
      else if(PORT_PIN_OUT == Port_PortChannels[ChannelId].pin_direction)
      {
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        if (PORT_PIN_LEVE_LOW == Port_PortChannels[ChannelId].pin_initial_value)
        {
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DATA_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        }
        
        else if(PORT_PIN_LEVEL_HIGH == Port_PortChannels[ChannelId].pin_initial_value)
        {
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DATA_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        }
        
        else
        {
          /*Do Nothing*/ 
        }
      }
      else
      {
        /*Do Nothing*/ 
      }
      
      
    }
    /*Set the module state to initialized and point to the PB configuration structure using a global pointer.*/
    Port_Status = PORT_INITIALIZED;
  }
}

/******************************************************************************* 
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x01
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number & Direction - Port Pin Direction:
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction
********************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* check if the Port Driver initialized*/
  if ( PORT_INITIALIZED != Port_Status )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
                    PORT_E_UNINIT);
  }
  else
  {
    /*Do Nothing*/ 
  }
  /* check if the pin number is in range*/
  if(PORT_CONFIGURED_CHANNLES <= Pin)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
                    PORT_E_PARAM_PIN);
  }
  else 
  {
    /*Do Nothing*/ 
  }
  /* check if the Pin direction changeable option in runtime is on this pin*/
  if( STD_OFF == Port_PortChannels[Pin].pin_change_direction)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
                    PORT_E_DIRECTION_UNCHANGEABLE);
  }
  else
  {
    /* Do nothing */
  }
#endif
  if(NULL_PTR == Port_PortChannels)
  {
    assert(Port_ErrorState(PTR_IS_NULL));
  }
  else
  {
    volatile uint32 * Port_Ptr = NULL_PTR;
    
    switch(Port_PortChannels[Pin].port_num)
    {
    case PORT_A_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
      break;
    case PORT_B_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
      break;
    case PORT_C_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
      break;
    case PORT_D_ID:   
      Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
      break;
    case PORT_E_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
      break;
    case PORT_F_ID:   
      Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
      break;    
    }
    if (PORT_PIN_IN == Direction)
    {
      CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[Pin].pin_num);
    }
    else if(PORT_PIN_OUT == Direction)
    {
      SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[Pin].pin_num);
    }
    else
    {
      /*Do Nothing*/ 
    }
    
  }
  
}
#endif

/******************************************************************************* 
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x02
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
********************************************************************************/
void Port_RefreshPortDirection( void )
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* check if the Port Driver initialized*/
  if ( PORT_INITIALIZED != Port_Status )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID,
                    PORT_E_UNINIT);
  }
  else
  {
    /* Do nothing */
  }
#endif
  if(NULL_PTR == Port_PortChannels)
  {
    assert(Port_ErrorState(PTR_IS_NULL));
  }
  else
  {
    volatile uint32 * Port_Ptr = NULL_PTR;
    Port_PinType ChannelId;
    
    for(ChannelId = ZERO; ChannelId < PORT_CONFIGURED_CHANNLES; ChannelId++)
    {   
      switch(Port_PortChannels[ChannelId].port_num)
      {
      case PORT_A_ID:    
        Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
        break;
      case PORT_B_ID:    
        Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
        break;
      case PORT_C_ID:    
        Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
        break;
      case PORT_D_ID:    
        Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
        break;
      case PORT_E_ID:    
        Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS;
        break;
      case PORT_F_ID:   
        Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS;
        break;
      }
      /* Exclude the port pins from refreshing that are configured as pin direction changeable during runtime. */
      if(STD_OFF == Port_PortChannels[ChannelId].pin_change_direction)
      {
        if (BIT_IS_CLEAR(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[ChannelId].pin_num))
        {
          CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        }
        else if(BIT_IS_SET(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[ChannelId].pin_num))
        {
          SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIR_REG_OFFSET),Port_PortChannels[ChannelId].pin_num);
        }
        else
        {
          /*Do Nothing*/ 
        }
        
      }
      else 
      {
        /*Do Nothing*/ 
      }
      
      
    }
  }
  
  
}

/******************************************************************************* 
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Returns the version information of this module:
*                                                             - Module Id
*                                                             - Vendor Id
*                                                             - Vendor specific version numbers
********************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* Check if input pointer is not Null pointer */
  if(NULL_PTR == versioninfo)
  {
    /* Report to DET  */
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID,
                    PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
  }
  else
#endif /* (DIO_DEV_ERROR_DETECT == STD_ON) */
  {
  if(NULL_PTR == versioninfo)
  {
    assert(Port_ErrorState(PTR_IS_NULL));
  }
  else
  {
    /* Copy the vendor Id */
    versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
    /* Copy the module Id */
    versioninfo->moduleID = (uint16)PORT_MODULE_ID;
    /* Copy Software Major Version */
    versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
    /* Copy Software Minor Version */
    versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
    /* Copy Software Patch Version */
    versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
  }
  }
}
#endif

/******************************************************************************* 
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number & Mode - New Port Pin mode to be set on port pin.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
********************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode )
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  /* check if the Port Driver initialized*/
  if ( PORT_INITIALIZED != Port_Status )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
                    PORT_E_UNINIT);
  }
  else
  {
    /*Do Nothing*/ 
  }
  /* check if the pin number is in range*/
  if(PORT_CONFIGURED_CHANNLES <= Pin)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
                    PORT_E_PARAM_PIN);
  }
  else 
  {
    /*Do Nothing*/ 
  }
  /* check if the Pin mode changeable option in runtime is on this pin*/
  if( PORT_CONFIGURED_CHANNLES_MODES >= Mode)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID,
                    PORT_E_PARAM_INVALID_MODE);
  }
  else
  {
    /*Do Nothing*/ 
  }
  if( STD_OFF == Port_PortChannels[Pin].pin_change_direction)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID,
                    PORT_E_DIRECTION_UNCHANGEABLE);
  }
  else
  {
    /* Do nothing */
  }
#endif
  if(NULL_PTR == Port_PortChannels)
  {
    assert(Port_ErrorState(PTR_IS_NULL));
  }
  else
  {
    
    volatile uint32 * Port_Ptr = NULL_PTR;
    
    switch(Port_PortChannels[Pin].port_num)
    {
    case PORT_A_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
      break;
    case PORT_B_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS;
      break;
    case PORT_C_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS;
      break;
    case PORT_D_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS;
      break;
    case PORT_E_ID:    
      Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
      break;
    case PORT_F_ID:   
      Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS;
      break;    
    }
    
    switch (Port_PortChannels[Pin].pin_mode)
    {
      
    case PORT_PIN_MODE_DIO:
      
      /*Clear the corresponding bit in the GPIOAFSEL register to disable alternative function on this pin */
      CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num); 
      
      /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
      CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
      
      /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
      SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
      
      /*Clear the corresponding bits in the GPIOCTL register to use it as GPIO pin */
      *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) &=~(0x0000000F << (Port_PortChannels[Pin].pin_num * 4));
      
      break;
      
    case PORT_PIN_MODE_ADC:
      
      if(((PORT_B_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num)))\
        ||((PORT_D_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num <= PIN_3_ID))\
          ||((PORT_E_ID == Port_PortChannels[Pin].port_num)))
      {
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAMSEL register to enable analog on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Clear the PMCx in the GPIODEN register to disable digital on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
      }
      else
      {
        /*Do Nothing mode is not available for other pins*/
      }
      break;
      
    case PORT_PIN_MODE_CAN:
      
      if((PORT_F_ID == Port_PortChannels[Pin].port_num)&&((PIN_0_ID == Port_PortChannels[Pin].pin_num)||(PIN_3_ID == Port_PortChannels[Pin].pin_num)))
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCx in the GPIOCTL register to use it as CAN pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Port_PortChannels[Pin].pin_num * 4));
      }
      else if(((PORT_A_ID == Port_PortChannels[Pin].port_num)&&((PIN_0_ID == Port_PortChannels[Pin].pin_num)||(PIN_1_ID == Port_PortChannels[Pin].pin_num)))\
        ||((PORT_B_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num)))\
          ||((PORT_E_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num))))
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as CAN pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_PortChannels[Pin].pin_num * 4));
      }
      else
      {
        /*Do Nothing mode is not available for other pins*/
      }
      break;
      
    case PORT_PIN_MODE_DIO_GPT:
      
      if((PORT_B_ID == Port_PortChannels[Pin].port_num)||(PORT_F_ID == Port_PortChannels[Pin].port_num))
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as timer pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (Port_PortChannels[Pin].pin_num * 4));
      }
      else
      {
        /*Do Nothing mode is not available for other pins*/
      }
      break;
      
    case PORT_PIN_MODE_DIO_WDG:
      
      if(((PORT_C_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num >= PIN_4_ID))||(PORT_D_ID == Port_PortChannels[Pin].port_num))
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as watchdog pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000007 << (Port_PortChannels[Pin].pin_num * 4));
      }
      else
      {
        /*Do Nothing mode is not available for other pins*/
      }
      break;
      
    case PORT_PIN_MODE_USB:
      
      if(((PORT_C_ID == Port_PortChannels[Pin].port_num)&&((PIN_6_ID == Port_PortChannels[Pin].pin_num)||(PIN_7_ID == Port_PortChannels[Pin].pin_num)))\
        ||((PORT_D_ID == Port_PortChannels[Pin].port_num)&&((PIN_2_ID == Port_PortChannels[Pin].pin_num)||(PIN_3_ID == Port_PortChannels[Pin].pin_num)))\
          ||((PORT_F_ID == Port_PortChannels[Pin].port_num)&& PIN_4_ID == Port_PortChannels[Pin].pin_num))
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as USB pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_PortChannels[Pin].pin_num * 4));
        
      }
      else 
      {
        /*Do Nothing mode is not available for other pins*/
      }
      break;
      
    case PORT_PIN_MODE_I2C:
      
      if(((PORT_A_ID == Port_PortChannels[Pin].port_num)&&((PIN_6_ID == Port_PortChannels[Pin].pin_num)||(PIN_7_ID == Port_PortChannels[Pin].pin_num)))\
        ||((PORT_B_ID == Port_PortChannels[Pin].port_num)&&((PIN_2_ID == Port_PortChannels[Pin].pin_num)||(PIN_3_ID == Port_PortChannels[Pin].pin_num)))\
          ||((PORT_D_ID == Port_PortChannels[Pin].port_num)&&((PIN_0_ID == Port_PortChannels[Pin].pin_num)||(PIN_1_ID == Port_PortChannels[Pin].pin_num)))\
            ||((PORT_E_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num))))
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as I2C pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Port_PortChannels[Pin].pin_num * 4));
      }
      else
      {
        /*Do Nothing mode is not available for other pins*/
      }
      break;
      
    case PORT_PIN_MODE_PWM:
      
      if((PORT_B_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num >= PIN_4_ID)\
        ||((PORT_C_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num)))\
          ||((PORT_D_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num <= PIN_1_ID)\
            &&((INSTANCE_6 == Port_PortChannels[Pin].pin_mode_instance)||(INSTANCE_7 == Port_PortChannels[Pin].pin_mode_instance)))\
              ||((PORT_E_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num))\
                &&((INSTANCE_4 == Port_PortChannels[Pin].pin_mode_instance)||(INSTANCE_5 == Port_PortChannels[Pin].pin_mode_instance))))
      {    
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as PWM pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000004 << (Port_PortChannels[Pin].pin_num * 4));
      }
      else if((PORT_A_ID == Port_PortChannels[Pin].port_num)&&((PIN_6_ID == Port_PortChannels[Pin].pin_num)||(PIN_7_ID == Port_PortChannels[Pin].pin_num))\
        ||((PORT_E_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num)))\
          ||((PORT_D_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num <= PIN_1_ID)\
            &&((INSTANCE_0 == Port_PortChannels[Pin].pin_mode_instance)||(INSTANCE_1 == Port_PortChannels[Pin].pin_mode_instance)))
            ||((PORT_F_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num <= PIN_3_ID)))  
      {    
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as PWM pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000005 << (Port_PortChannels[Pin].pin_num * 4));
      }
      else
      {
        /*Do Nothing mode is not available for other pins*/
      }
      break;
      
    case PORT_PIN_MODE_SSI:
      
      if(((PORT_A_ID == Port_PortChannels[Pin].port_num)&&((PIN_2_ID == Port_PortChannels[Pin].pin_num)||(PIN_3_ID == Port_PortChannels[Pin].pin_num)\
        ||(PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num)))\
          ||((PORT_B_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num >= PIN_4_ID))\
            ||((PORT_D_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num <= PIN_3_ID)&&(INSTANCE_3 == Port_PortChannels[Pin].pin_mode_instance)))
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as SSI pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (Port_PortChannels[Pin].pin_num * 4));
        
      }
      else if (((PORT_D_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num <= PIN_3_ID)\
        &&(INSTANCE_1 == Port_PortChannels[Pin].pin_mode_instance))\
          ||((PORT_F_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num <= PIN_3_ID)))
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the PMCxin the GPIOCTL register to use it as SSI pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000003 << (Port_PortChannels[Pin].pin_num * 4));
        
      }
      else
      {
        /*Do Nothing mode is not available for other pins*/
      }
      break;
      
    case PORT_PIN_MODE_UART:
      
      if(((PORT_A_ID == Port_PortChannels[Pin].port_num)&&((PIN_0_ID == Port_PortChannels[Pin].pin_num)||(PIN_1_ID == Port_PortChannels[Pin].pin_num)))\
        ||((PORT_B_ID == Port_PortChannels[Pin].port_num)&&((PIN_0_ID == Port_PortChannels[Pin].pin_num)||(PIN_1_ID == Port_PortChannels[Pin].pin_num)))\
          ||((PORT_C_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num >= PIN_4_ID))\
            ||((PORT_D_ID == Port_PortChannels[Pin].port_num)&&(Port_PortChannels[Pin].pin_num >= PIN_4_ID))\
              ||((PORT_E_ID == Port_PortChannels[Pin].port_num)&&((Port_PortChannels[Pin].pin_num <= PIN_1_ID)||(Port_PortChannels[Pin].pin_num >= PIN_4_ID)))\
                ||((PORT_F_ID == Port_PortChannels[Pin].port_num)&&((PIN_0_ID == Port_PortChannels[Pin].pin_num)||(PIN_1_ID == Port_PortChannels[Pin].pin_num))))        
      {
        /*Clear the corresponding bit in the GPIOAMSEL register to disable analog on this pin */
        CLEAR_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIOAFSEL register to enable alternative function on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_ALT_FUNC_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        /*Set the corresponding bit in the GPIODEN register to enable digital on this pin */
        SET_BIT(*(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET),Port_PortChannels[Pin].pin_num);
        
        
        if((PORT_C_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)\
          ||(PIN_5_ID == Port_PortChannels[Pin].pin_num))&&(INSTANCE_1 == Port_PortChannels[Pin].pin_mode_instance))
        {
          /*Set the PMCxin the GPIOCTL register to use it as UART pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000002 << (Port_PortChannels[Pin].pin_num * 4) );
        }
        else if ((PORT_C_ID == Port_PortChannels[Pin].port_num)&&((PIN_4_ID == Port_PortChannels[Pin].pin_num)||(PIN_5_ID == Port_PortChannels[Pin].pin_num))\
          &&((INSTANCE_U1RTS == Port_PortChannels[Pin].pin_mode_instance)||(INSTANCE_U1CTS == Port_PortChannels[Pin].pin_mode_instance)))
        {
          /*Set the PMCxin the GPIOCTL register to use it as UART pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_PortChannels[Pin].pin_num * 4) );
        }
        
        else
        {
          /*Set the PMCxin the GPIOCTL register to use it as UART pin */
          *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000001 << (Port_PortChannels[Pin].pin_num * 4) );
        }
      }
      else
      {
        /*Do Nothing mode is not available for other pins*/
      }
      
      break;
      
    case PORT_PIN_INITIAL_MODE_NMI:
      
      if(((PORT_D_ID == Port_PortChannels[Pin].port_num)&&(PIN_7_ID == Port_PortChannels[Pin].pin_num))\
        ||((PORT_F_ID == Port_PortChannels[Pin].port_num)&&(PIN_0_ID == Port_PortChannels[Pin].pin_num)))                                                      
      {
        /*Set the PMCxin the GPIOCTL register to use it as NMI pin */
        *(volatile uint32*)((volatile uint8*) Port_Ptr + PORT_CTL_REG_OFFSET) |= (0x00000008 << (Port_PortChannels[Pin].pin_num * 4) );
      }
      else 
      {
        /*Do Nothing mode is not available for other pins*/
      }
      
      break;
    }
    
  }
  
  
}
#endif

/************************************************************************************
* Function Name : Port_ErrorState
* Inputs        : uint8 error_code
* Outputs       : boolean value (True or False)
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Returns True if an error was reported.
************************************************************************************/
boolean Port_ErrorState(uint8 error_code)
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


