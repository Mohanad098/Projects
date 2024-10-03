/******************************************************************************
*
* Module: ECU1_App
*
* File Name: ECU1_App.c
*
* Description: Source file for TM4C123GH6PM Microcontroller application layer.
*
* Author: Team 9
******************************************************************************/
#include "ECU1_App.h"
#include "OS.h"
#include "Button.h"
#include "CAN.h"
#include "Communication.h"


uint8 button_state=0;


uint8 ECU2_state=0;

/************************************************************************************
* Function Name : App_Init
* Inputs        : None
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Initilalizes the SPI as Master and UART for PC Communication and GPIO PORT and their tasks through OS 
************************************************************************************/
void App_Init(void)
{
  Comm_init();
  Comm_transmit_set();
  
  RTOS_Init();  
  RTOS_CreateTask(&Task_CFG);
}

/************************************************************************************
* Function Name : Task1_Button
* Inputs        : None
* Outputs       : None
* Reentrancy    : Non-reentrant
* Sync/Async    : Synchronous
* Description   : Responsible for reading the buttons states to determine the state to send to ECU2 (CW, Anti CW, RED, WHITE) 
************************************************************************************/
void Task1_Button (void)
{
  static uint8 button_counter	=0;     /* A variable that increments each time the button is pressed to delay a number of tick times after */
  uint8 switch1_prev	=0;		/* Stores the value of button 1 */
  uint8 switch1_current	=0;		/* Stores the value of button 2 */
  uint8 switch2_prev	=0;		/* Stores the value of button 1 after the button was pressed initially */
  uint8 switch2_current	=0;		/* Stores the value of button 2 after the button was pressed initially */
  
  
  if (button_counter ==0)
  {
    switch1_prev = Button1_State();
    switch2_prev = Button2_State();
    if(switch1_prev == 0 || switch2_prev ==0)
    {
      button_counter++;
    }
    else
    {
      /* Do nth */
    }
  }
  else if (button_counter == 1)
  {
    button_counter++;
  }
  else if (button_counter == 2)
  {
    switch1_current = Button1_State();
    switch2_current = Button2_State();
    if(switch1_prev == 0 && switch1_current == 0 && switch2_prev == 0 && switch2_current == 0)
    {
      button_state = 3;                                  /*Indicates that SW1 & SW2 is pressed so Reset state*/
      
    }
    else if ( switch1_prev == 0 && switch1_current == 0 )
    {
      button_state = 1;                                 /*Indicates that SW1 is pressed so Clockwise state*/
      
    }
    else if ( switch2_prev == 0 && switch2_current == 0 )
    {
      button_state = 2;                                 /*Indicates that SW2 is pressed so Counter Clockwise state*/
      
    }
    else
    {
      button_counter = 0;
    }
    
  }
}

/************************************************************************************
* Function Name : Task2_CAN
* Inputs        : None
* Outputs       : None
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Responsible for Sending msg to ECU2 according to the buttons states
************************************************************************************/
void Task2_CAN (void)
{
  sint8 Send_msg[8] = "S0";
  
  switch(button_state)
  {
  case 0: 
    Send_msg[0] = 'S';
    Send_msg[1] = '0';
    break;
    
  case 1: 
    Send_msg[0] = 'S';
    Send_msg[1] = '1';
    break;   
    
  case 2: 
    Send_msg[0] = 'S';
    Send_msg[1] = '2';
    break;   
    
  case 3: 
    Send_msg[0] = 'S';
    Send_msg[1] = '3';
    break;
    
  case 4: 
    Send_msg[0] = 'S';
    Send_msg[1] = '4';
    break;     
  }
  
  //CAN_Transmit_Data(Send_msg);
  Comm_transmit_data(Send_msg);
}

/************************************************************************************
* Function Name : Task3_IdleCheck
* Inputs        : None
* Outputs       : None
* Reentrancy    : Non-reentrant
* Sync/Async    : Synchronous
* Description   : It checks if the same state occurs on ECU2 for 10s Change state to IDLE (White)
************************************************************************************/
void Task3_IdleCheck (void)
{
  static uint8 counter = 0;
  static uint8 check = 0;
  
  if(counter == 0)
  {
    check = button_state;
  }
  
  if(check == button_state)
  {
    counter++;
  }
  else
  {
    counter = 1;
    check = button_state; 
  }
  
  if(counter == 5)
  {
    button_state = 4;
    counter=0;
  }
  
  
}


