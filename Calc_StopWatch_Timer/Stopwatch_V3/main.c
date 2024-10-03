//WS_PROJECT
#include "Stopwatch_Func.h"
#include "calculator.h"
#include "keypad.h"
#include "lcd.h"
#include "Timer0.h"

#include <stdio.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "systick.h"
#include "tm4c123gh6pm.h"


//////PROJECT/////
int Mode=1;
int x;

int main()
{
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Set bit5 of RCGCGPIO to enable clock to PORTF
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // Set bit5 of RCGCGPIO to enable clock to PORTD //////
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  

  // PORTF0 has special function, need to unlock to modify 
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock commit register
  GPIO_PORTF_CR_R  = 0x01;           // make PORTF0 configurable 
  GPIO_PORTF_LOCK_R = 0;            // lock commit register 
  
  GPIO_PORTD_LOCK_R = 0x4C4F434B;   // unlock commit register  //////
  GPIO_PORTD_CR_R  = 0x80;           // make PORTF0 configurable  ////
  GPIO_PORTD_LOCK_R = 0;            // lock commit register  ////
  
  
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
  
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
  GPIO_PORTF_PUR_R |= (1<<0)|(1<<4);
  
  GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);  ////
  
  
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_FALLING_EDGE);
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_FALLING_EDGE);
  
  GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3,GPIO_RISING_EDGE); ////
  
  GPIOIntClear(GPIO_PORTF_BASE, INT_GPIOF_TM4C123);
  
  GPIOIntClear(GPIO_PORTD_BASE, INT_GPIOD_TM4C123);  ////
  
  GPIO_PORTF_IM_R  |= (1<<4)|(1<<0);          // unmask interrupt 
  
  GPIO_PORTD_IM_R  |= (1<<3);          // unmask interrupt    ///
  
  IntDisable(INT_GPIOF_TM4C123);
  IntPrioritySet(INT_GPIOF_TM4C123,3);
  IntEnable(INT_GPIOF_TM4C123);
  
  IntDisable(INT_GPIOD_TM4C123);  ////
  IntPrioritySet(INT_GPIOD_TM4C123,3);//// 
  IntEnable(INT_GPIOD_TM4C123);  ////
  
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0x0);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0x0);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0x0);


  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  
  SystickInt(1000);
  Timer0A(1);
  
  
  LCD_init();
  LCD_Clear();
  
  
  
  while(1)
  { 
    

    while(Mode==1)
    {      
      x=KeyPad_Read();
      //printf("%d",x);
      //calculator (uint8* array); //Array hyta5d mn el keypad
      //LCD_Cursor(0, 0);
      LCD_Write_Integer(x);
      SysCtlDelay_Ehab(3000000);
      
      
      //TESTING TIMER1 DELAY
      //GPIO_PORTF_DATA_R ^= 0x08;
      //Timer1A (1);
     // GPIO_PORTF_DATA_R ^= 0x08;
      //Timer1A (1);
      

    }
    while(Mode==2)
    {      
      //Clock();
      //LCD_Print("omar");
      //SysCtlDelay(1000);
      //LCD_Clear();
      //SysCtlDelay(1000);
      
      Clock();
      
      
      //LCD_Print("Hello World!");          
      //SysCtlDelay_Ehab(3000000); //Delay (3000000 for 1 sec)
      //LCD_Clear(); 
      //SysCtlDelay_Ehab(3000000); //Delay
      
      
    }
    while(Mode==3)
    {      
      //TIMER_MODE
      
      
      Timer0A_countdwon();
      
      
      
    }
    
    
    
    
  }
  
  return 0;
}








