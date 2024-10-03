

#include "Stopwatch_Func.h"


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
#include "lcd.h"

unsigned char Sec1=0,Sec2=0,Min1=0,Min2=0,Hour1=0,Hour2=0;

int msTicks=0;

// SW1 is connected to PF4 pin, SW2 is connected to PF0. 
// Both of them trigger PORTF falling edge interrupt 
void GPIOF_Handler(void)
{
  //GPIO_PORTF_DATA_R ^= 0x04;
  
  if ( GPIO_PORTF_MIS_R & 0x10) // check if interrupt causes by PF4/SW1
  {   
    SysTickEnable();
    SysTickIntEnable();
    //GPIO_PORTF_DATA_R = 0x10;
    GPIO_PORTF_ICR_R |= 0x10; // clear the interrupt flag 


  } 
  else if ( GPIO_PORTF_MIS_R & 0x01) // check if interrupt causes by PF0/SW2 
  {  
    
    SysTickDisable();
    SysTickIntDisable();
    //GPIO_PORTF_DATA_R = 0x00;
  
    GPIO_PORTF_ICR_R |= 0x01; // clear the interrupt flag 
    
  }
}


void Systick_Handler(void)
{
  msTicks=1;
  GPIO_PORTF_DATA_R ^= 0x04;
}


void GPIOD_Handler(void)
{
  Sec1=0,Sec2=0,Min1=0,Min2=0,Hour1=0,Hour2=0;
  
  GPIO_PORTF_DATA_R |= 0x02;
  
  // GPIO_PORTF_DATA_R &= ~0x08;
  GPIO_PORTD_ICR_R |= 0x08; // clear the interrupt flag 
  
}


void Clock(void)
{
  if(msTicks==1) //Every Second Increment
  {
    Sec1++;

    if(Sec1==10)
    {
      Sec1=0;
      Sec2++;
    }
    if(Sec2==6)
    {
      Sec2=0;
      Sec1=0;
      Min1++;
    }
    if(Min1==10)
    {
      Min1=0;
      Min2++;
    }
    if(Min2==6)
    {
      Min2=0;
      Min1=0;
      Hour1++;
    }
    if(Hour1==10)
    {
      Hour1=0;
      Hour2++;
    }
    if(Hour2==10)
    {
      Sec1=0;
      Sec2=0;
      Min1=0;
      Min2=0;
      Hour1=0;
      Hour2=0;
    }
    msTicks=0;
  }
  
LCD_Cursor(0, 0);
  LCD_Write_Integer(Hour2);
   // SysCtlDelay_Ehab(200000);
    LCD_Cursor(0, 1);
  LCD_Write_Integer(Hour1);
    //SysCtlDelay_Ehab(200000);
LCD_Cursor(0, 2);
  LCD_Print(":");
    //SysCtlDelay_Ehab(200000);
LCD_Cursor(0, 3);
  LCD_Write_Integer(Min2);
    //SysCtlDelay_Ehab(200000);
LCD_Cursor(0, 4);
  LCD_Write_Integer(Min1);
    //SysCtlDelay_Ehab(200000);
 LCD_Cursor(0, 5);
  LCD_Print(":");
   // SysCtlDelay_Ehab(200000);
LCD_Cursor(0, 6);
  LCD_Write_Integer(Sec2);
   // SysCtlDelay_Ehab(200000);
LCD_Cursor(0, 7);
  LCD_Write_Integer(Sec1);
  SysCtlDelay_Ehab(3000000);

    //LCD_Cursor(0, 0);

}


void SystickInt(uint32_t time){
  time = (time * 16000);
  SysTickDisable();
  SysTickPeriodSet(time);
  SysTickIntDisable();
  NVIC_ST_CURRENT_R=0;

  
}

/* //DELAY_USING_SYSTICK
void delayms(uint32_t time){
  time = (time * (16000-1));
  SysTickDisable();
  SysTickPeriodSet(time);
  NVIC_ST_CURRENT_R=0;
  SysTickEnable();
  while((NVIC_ST_CTRL_R &(1<<16))==0){};  
}
*/

