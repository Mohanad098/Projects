/******************************************************************************
*
* Module: ECU2_App
*
* File Name: ECU1_App.c
*
* Description: Source file for TM4C123GH6PM Microcontroller application layer.
*
* Author: Team 9
******************************************************************************/
#include "ECU2_App.h"
#include "LED.h"
#include "OS.h"
#include "Communication.h"



volatile uint8 button_state = 0;


volatile uint8 ECU2_state = 0;

/************************************************************************************
* Function Name : App_Init
* Inputs        : None
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Initilalizes the SPI as Slave and GPIO Port for LEDs then creates their tasks through OS  
************************************************************************************/
void App_Init(void)
{

  
 /*Init CAN */
  Comm_init();  
  Comm_receive_set();
 
    
  RTOS_Init();  
  RTOS_CreateTask(&Task_CFG);
                  
  
}

/************************************************************************************
* Function Name : Task1_LEDS
* Inputs        : None
* Outputs       : None
* Reentrancy    : Non-reentrant
* Sync/Async    : Synchronous
* Description   : Responsible for the state and sequence of the LED (CW, Anti CW, RED ONLY, WHITE ONLY) 
************************************************************************************/
void Task1_LEDS (void)
{
  static uint16 led_counter = 1;
  
  switch (ECU2_state)
  {
  case 0:       TURN_OFF_LEDS();
  break;
  /*Clockwise */
  case 1:       
    if (led_counter == 1)
    {
      RED_LED(); /* Turn on the Red LED and disable the others*/
    }
    else if ( led_counter == 2)
    {
     GREEN_LED(); /* Turn on the Green LED and disable the others */
    }
    else if ( led_counter == 3)
    {
      BLUE_LED(); /* Turn on the Blue LED and disable the others */
    }    
    led_counter++;
    if (led_counter == 4)
    {
      led_counter = 1;
    }
    break;
    /* Counter Clockwise */
  case 2:
    if (led_counter == 1)
    {
      RED_LED(); /* Turn on the Red LED and disable the others*/
    }
    else if ( led_counter == 2)
    {
      GREEN_LED(); /* Turn on the Green LED and disable the others */
    }
    else if ( led_counter == 3)
    {
      BLUE_LED(); /* Turn on the Green LED and disable the others */
    }    
    led_counter--;
    if (led_counter == 0)
    {
      led_counter = 3;
    }    
    break;
    
    /* Reset */
  case 3:
    RED_LED(); /* Turn on the Red LED and disable the others*/
    break;
    
    /* Idle State */
  case 4:
    WHITE_LED(); /* Turn on the white LED and disable the others*/     
    break;
    
  default: break;
  }
  
}

/************************************************************************************
* Function Name : Task2_CAN
* Inputs        : None
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Receives the msg from Master SPI and checks which character is received to be used for changing / maintaining states
************************************************************************************/

void Task2_CAN (void)
{
  sint8 msg[8];
  static uint8 Received_msg;
  Comm_receive_data(msg);
  
  switch(msg[1])
  {
  case '0': 
    Received_msg = 0;
    break;
    
  case '1': 
    Received_msg = 1;
    break;   
    
  case '2': 
    Received_msg = 2;
    break;   
    
  case '3': 
    Received_msg = 3;
    break;
    
  case '4': 
    Received_msg = 4;
    break;    
    
  default:
    break;
  }
  
  if(Received_msg == 60 || Received_msg == 'Z')
  {
    /*do nothing*/
  }

    ECU2_state = Received_msg;

}



