#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdbool.h>
#include "Timer0.h"
#include "types.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "bitwise_operation.h"

#include "lcd.h"


extern unsigned char Timer_Sec1,Timer_Sec2,Timer_Min1,Timer_Min2;

int Timer_Ticks;

int Timer1_Ticks;
char Finish=0;

//char Sec11=5,Sec22=1,Min11=1,Min22=0;

extern char Start_Timer;



void Timer1A (uint16 time)
{
  TimerDisable(TIMER1_BASE,TIMER_A);
  TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER1_BASE,TIMER_A,((16000*time)-1));
  TIMER1_ICR_R=0x1;
  TimerEnable(TIMER1_BASE,TIMER_A);
  while((TIMER1_RIS_R & 0x1) ==0 );
  
}
/*void Timer1A_Handler(void)
{
Timer1_Ticks=1;

GPIO_PORTF_DATA_R ^= 0x02;
//TIMER0_ICR_R = 0x01;    // 6) clear TIMER0A timeout flag
TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
}*/
void Timer0A(uint16 time){ 
  //desired_time=time;
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  TIMER0_CTL_R = 0x0;    // 1) disable TIMER0A during configuration
  TIMER0_CFG_R = 0x0;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x02;   // 3) configure first bit for periodic mode, second bit for down-count settings (0x12 for count up)
  TIMER0_TAILR_R = (16000000*time)-1;    // 4) reload value
  TIMER0_ICR_R = 0x01;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x01;    // 7) arm timeout interrupt
  NVIC_EN0_R = 1<<19;           // 9) ector number 35, enable interrupt number 19 in NVIC
  TIMER0_CTL_R = 0x01;    // 10) enable TIMER0A
  
  
  
  
}

void Timer0A_Handler(void)
{
  Timer_Ticks=1;
  
  GPIO_PORTF_DATA_R ^= 0x08;
  //TIMER0_ICR_R = 0x01;    // 6) clear TIMER0A timeout flag
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
  
}

void Timer0A_countdown(void)
{
  
  LCD_Clear();
  LCD_Cursor(0, 0);
  LCD_Write_Integer(Timer_Min2);
  //SysCtlDelay_Ehab(200000);
  LCD_Cursor(0, 1);
  LCD_Write_Integer(Timer_Min1);
  //SysCtlDelay_Ehab(200000);
  LCD_Cursor(0, 2);
  LCD_Print(":");
  //SysCtlDelay_Ehab(200000);
  LCD_Cursor(0, 3);
  LCD_Write_Integer(Timer_Sec2);
  //SysCtlDelay_Ehab(200000);
  LCD_Cursor(0, 4);
  LCD_Write_Integer(Timer_Sec1);
  //SysCtlDelay_Ehab(3000000);
  Timer1A (1000);
  
  if(Timer_Ticks==1) //Every Second Increment
  {
    if(Timer_Sec1!=0)
    {
      Timer_Sec1--;
      //LCD_Cursor(0, 4);
      // LCD_Write_Integer(Timer_Sec1);
      //SysCtlDelay_Ehab(3000000);
      //Timer1A (1);
      
      //Finish=1;
    }
    
    
    if(Timer_Sec1==0 && Timer_Sec2!=0)
    {
      
      LCD_Cursor(0, 4);
      LCD_Write_Integer(Timer_Sec1);
      Timer1A (1000);
      Timer_Sec2--;
      Timer_Sec1=9;
      
      //Finish=1;
      
    }
    
    if(Timer_Sec2==0 && Timer_Min1!=0 && Timer_Sec1==0)
    {
      LCD_Cursor(0, 4);
      LCD_Write_Integer(Timer_Sec1);
      LCD_Cursor(0, 3);
      LCD_Write_Integer(Timer_Sec2);
      Timer1A (1000);
      Timer_Min1--;
      Timer_Sec2=5;
      Timer_Sec1=9;
      
      //Finish=1;
      
    }
    
    if(Timer_Sec2==0 && Timer_Min1==0 && Timer_Sec1==0 && Timer_Min2!=0)
    {
      LCD_Cursor(0, 4);
      LCD_Write_Integer(Timer_Sec1);
      LCD_Cursor(0, 3);
      LCD_Write_Integer(Timer_Sec2);  
      LCD_Cursor(0, 1);
      LCD_Write_Integer(Timer_Min1);
      Timer1A (1000);
      Timer_Min2--;
      Timer_Min1=9;
      Timer_Sec2=5;
      Timer_Sec1=9;
      
      //Finish=1;
      
    }
    if(Timer_Sec2==0 && Timer_Min1==0 && Timer_Sec1==0 && Timer_Min2==0 )
    {
      
      LCD_Cursor(0, 4);
      LCD_Write_Integer(Timer_Sec1);
      LCD_Cursor(0, 3);
      LCD_Write_Integer(Timer_Sec2);  
      LCD_Cursor(0, 2);
      LCD_Print(":");
      LCD_Cursor(0, 1);
      LCD_Write_Integer(Timer_Min1);
      Timer1A (1000);
      
      Start_Timer=1;
       //GPIO_PORTF_DATA_R ^= 0x04;//buzzer for now 
      //GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,0x01);
      
      //buzzer for now 
      //if(Finish==1)
      //{
        SET_BIT(GPIO_PORTA_DATA_R,5);
        //SysCtlDelay_Ehab(3000000);
        Timer1A(2000);
        CLEAR_BIT(GPIO_PORTA_DATA_R,5);
        //SysCtlDelay_Ehab(3000000);
        Timer1A(2000);
        
        
     // }
     // Finish=0;
       
    }
     
    
    Timer_Ticks=0;
  }
  
  
  
}
/*if(Timer_Sec2==0)
{
Timer_Sec2=0;
Timer_Sec1=0;
if(Timer_Min1>0)
{
Timer_Min1--;
  }
  }
if(Timer_Min1==0)
{
Timer_Sec2=0;
Timer_Sec1=0;
Timer_Min1=0;
if(Timer_Min2>0)
{
Timer_Min2--;
  }
  }
if(Timer_Min2==0)
{
//hykon EL BUZZER
Timer_Min2=0;
Timer_Min1=0;
Timer_Sec1=0;
Timer_Sec2=0;
  }

Timer_Ticks=0;
  }*/



//LCD_Cursor(0, 0);