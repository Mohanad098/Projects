//WS_PROJECT
//CALC DONE 25/12
#include "Stopwatch_Func.h"
#include "calculator.h"
#include "keypad.h"
#include "lcd.h"
#include "Timer0.h"
#include "types.h"

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



////////BUZZER TAGROBA///////
#include "bitwise_operation.h"

//////PROJECT/////
int Mode=0;
unsigned char Sec1=0,Sec2=0,Min1=0,Min2=0,Hour1=0,Hour2=0;
unsigned char Timer_Sec1,Timer_Sec2,Timer_Min1,Timer_Min2;
//char x;


//static volatile void (*g_callBackPtrCalc)(void) = NULL_PTR;

//static volatile void (*g_callBackPtrClk)(void) = NULL_PTR;

//static volatile void (*g_callBackPtrTimer)(void) = NULL_PTR;


void GPIOA_Handler(void)
{
  
  if ( GPIO_PORTA_MIS_R & 0x08)// pinE5
  {
    Timer1A(200);
    if ( GPIO_PORTA_MIS_R & 0x08)// pinE5
    {
      
      
      Mode++;
      
      
      /* if (Mode==1)
      {
      if(g_callBackPtrCalc != NULL_PTR)
      {
      (*g_callBackPtrCalc)(); // another method to call the function using pointer to function g_callBackPtr();
    }
    }*/
      /*
      if(Mode==2)
      {
      
      if(g_callBackPtrClk != NULL_PTR)
      {
      (*g_callBackPtrClk)(); // another method to call the function using pointer to function g_callBackPtr();
    }
      
    }
    else if(Mode==3)
      {
      if(g_callBackPtrTimer != NULL_PTR)
      {
      (*g_callBackPtrTimer)(); // another method to call the function using pointer to function g_callBackPtr();
    }
      
      Mode=1;
    }*/
      if(Mode>3)
      {
        Mode=0;
      }
      GPIO_PORTA_ICR_R |= 0x08;
    }
    
  }
}
//void Calc_setCallBack(void(*a_ptr)(void))
//{
// Save the address of the Call back function in a global variable
//  g_callBackPtrCalc = a_ptr;
//}
//void  Clk_setCallBack(void(*a_ptr)(void))
//{
// Save the address of the Call back function in a global variable
// g_callBackPtrClk = a_ptr;
//}
//void Timer_setCallBack(void(*a_ptr)(void))
//{
// Save the address of the Call back function in a global variable
//  g_callBackPtrTimer = a_ptr;
//}


uint8 arr_calc[11]={0}; 

char Start_SW=1;
char Start_Timer=1;
char Enter_Timer=0;
extern uint8 key;

char arr_Timer[5]={0};


int main()
{
     // SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
     // GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);
        
        //Timer1A(200);
        //SET_BIT(GPIO_PORTA_DATA_R,5);
       // SysCtlDelay_Ehab(3000000);
       // CLEAR_BIT(GPIO_PORTA_DATA_R,5);
       


 
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Set bit5 of RCGCGPIO to enable clock to PORTF
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // Set bit5 of RCGCGPIO to enable clock to PORTD //////
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  
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
  
  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);
  
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
  
  
  GPIO_PORTF_PUR_R |= (1<<0)|(1<<4);
  
  GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);  ////
  GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3);
  
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0,GPIO_FALLING_EDGE);
  GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_4,GPIO_FALLING_EDGE);
  
  GPIOIntTypeSet(GPIO_PORTD_BASE, GPIO_PIN_3,GPIO_RISING_EDGE); ////
  
  GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_3,GPIO_RISING_EDGE);
  
  GPIOIntClear(GPIO_PORTF_BASE, INT_GPIOF_TM4C123);
  
  GPIOIntClear(GPIO_PORTD_BASE, INT_GPIOD_TM4C123);  ////
  
  GPIOIntClear(GPIO_PORTA_BASE, INT_GPIOA_TM4C123);  ////
  
  GPIO_PORTF_IM_R  |= (1<<4)|(1<<0);          // unmask interrupt 
  
  GPIO_PORTD_IM_R  |= (1<<3);          // unmask interrupt    ///
  
  GPIO_PORTA_IM_R  |= (1<<3); 
  
  IntDisable(INT_GPIOF_TM4C123);
  IntPrioritySet(INT_GPIOF_TM4C123,3);
  IntEnable(INT_GPIOF_TM4C123);
  
  IntDisable(INT_GPIOD_TM4C123);  ////
  IntPrioritySet(INT_GPIOD_TM4C123,3);//// 
  IntEnable(INT_GPIOD_TM4C123);  ////
  
  IntDisable(INT_GPIOA_TM4C123);  ////
  IntPrioritySet(INT_GPIOA_TM4C123,3);//// 
  IntEnable(INT_GPIOA_TM4C123);  ////
  
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0x0);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0x0);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0x0);
  
  //GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5,0x0);
  
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  
  SystickInt(1000);
  Timer0A(1);
  
  
  LCD_init();
  LCD_Clear();
  
  //keypad_Init();
  
  //Calc_setCallBack();
  //Clk_setCallBack(Clock);
  //Timer_setCallBack(Timer0A_countdwon);
  
  
  float32 result=0;
  
  while(1)
  { 
    LCD_Clear();
    
    while (Mode==0){
     // LCD_Clear();
      LCD_Print("Welcome");
      Timer1A(1000);
    }
    
    while(Mode==1)
    {    
      Start_SW=1;
      Start_Timer=1;
      
      
      LCD_Clear();
      LCD_Print("Calculator");
      Timer1A (1000);
      //LCD_Clear();
      char i=-1;
      LCD_Cursor(1, 0);
      do{
        
        
        i++;
        LCD_Cursor(1, i);
        
        arr_calc[i] = KeyPad_Read();
        if(arr_calc[i]!='+' && arr_calc[i]!='-' && arr_calc[i]!='/' && arr_calc[i]!='*' && arr_calc[i]!='=' && arr_calc[i]!=100)
        {
          
          LCD_Write_Integer(arr_calc[i]); 
        }
        
        else if(arr_calc[i]=='+')
        {
          
          LCD_Print("+");
        }
        else if(arr_calc[i]=='-')
        {
          LCD_Print("-");
        }
        else if(arr_calc[i]=='/')
        {
          LCD_Print("/");
        }
        else if(arr_calc[i]=='*')
        {
          LCD_Print("*");
        }
        else if(arr_calc[i]=='=')
        {
          LCD_Print("=");
        }
        
        Timer1A (1000);
        //LCD_Clear();
        
        
      }while(arr_calc[i]!='=' && Mode==1);
      //LCD_Clear();
      
      
      
      // for(int i=0;arr_calc[i]!='=';i++)
      // {
      // LCD_Write_Integer(arr_calc[i]);
      // Timer1A (1);
      // }
      
      
      if (Mode==1){
        //LCD_Print("Result is");
        result=calculator(arr_calc);
        LCD_Cursor(1, 11);
        
        
        //LCD_Write_Integer(result);
        //Timer1A (1000);
        //LCD_Clear();
        //LCD_Print("result is");
        //LCD_Cursor(0, 11);
        LCD_Write_Decimal(result);
        
        
        Timer1A (1000);
      }
      LCD_Clear();
    }
    
    
    while(Mode==2)
    {  
      Start_Timer=1;
      //Clock();
      //LCD_Print("omar");
      //SysCtlDelay(1000);
      //LCD_Clear();
      //SysCtlDelay(1000);
      //LCD_Clear();
      // Timer1A (1);
      //LCD_Print("Stopwatch");
      //Timer1A (1000);
      if(Start_SW==1)
      {
        LCD_Clear();
        LCD_Print("Stopwatch");
        Timer1A (1000);
        
        Sec1=0,Sec2=0,Min1=0,Min2=0,Hour1=0,Hour2=0;
        Start_SW=0;
      }
      Clock();
      
      
      
      //ModeChanger_setCallBack(Clock);
      
      //LCD_Print("Hello World!");          
      //SysCtlDelay_Ehab(3000000); //Delay (3000000 for 1 sec)
      //LCD_Clear(); 
      //SysCtlDelay_Ehab(3000000); //Delay
      
      
    }
    while(Mode==3)
    {      
      //TIMER_MODE
      
      LCD_Clear();
      //Timer1A (1);
      //LCD_Print("Timer");
      //Timer1A (1000);
      
      
      
      while(Enter_Timer!='=' && Mode==3 && Start_Timer==1)
      {
        
        LCD_Clear();
        LCD_Print("Timer");
        // Timer1A (1000);
        LCD_Cursor(1, 0);
        
        
        
        Timer_Min2=KeyPad_Read();
        if (Timer_Min2==100){
          break;
        }
        else{
          LCD_Write_Integer(Timer_Min2); 
          Timer1A (1000);
          
          
          Timer_Min1=KeyPad_Read();
          if(Timer_Min1==100)
          {
            break;
          }
          else
          {
            LCD_Write_Integer(Timer_Min1); 
            Timer1A (1000);
            
            
            Timer_Sec2=KeyPad_Read();
            if(Timer_Sec2==100)
            {
              break;
            }
            else 
            {
              LCD_Write_Integer(Timer_Sec2); 
              Timer1A (1000);
              
              
              Timer_Sec1=KeyPad_Read();
              
              if(Timer_Sec1==100)
              {
                break;
              }
              else {
                LCD_Write_Integer(Timer_Sec1); 
                Timer1A (1000);
                
                
                
                Enter_Timer=KeyPad_Read();;
                Start_Timer=0;
              }
            }
          }
        }
      }
      if (Timer_Min2==100 || Timer_Min1==100 ||Timer_Sec2==100 || Timer_Sec1==100 ){
        break;
      }
      else
      {
        Enter_Timer=0;
        Timer0A_countdown();
      }
      
      
      Start_SW=1;

      /*
      if(Start_Timer==1 && Enter_Timer!='=' )
      {
      LCD_Clear();
      LCD_Print("Timer");
      Timer1A (1000);
      LCD_Cursor(1, 0);
      
      Timer_Min2=KeyPad_Read();
      
      Timer1A (1000);
      //if(Timer_Min2 != 200)
      //{
      LCD_Write_Integer(Timer_Min2);
      //}
      Timer_Min1=KeyPad_Read();
      Timer1A (1000);
      //if(Timer_Min1 != 200)
      //{
      LCD_Write_Integer(Timer_Min1);
      //}
      LCD_Print(":");
      LCD_Cursor(1, 3);
      Timer_Sec2=KeyPad_Read();
      
      Timer1A (1000);
      //if(Timer_Sec2 != 200)
      //{
      LCD_Write_Integer(Timer_Sec2);
      //}
      
      Timer_Sec1=KeyPad_Read();
      
      Timer1A (1000);
      //if(Timer_Sec1 != 200)
      //{
      LCD_Write_Integer(Timer_Sec1);
      //}
      
      
      Enter_Timer=KeyPad_Read();
      Timer1A (1000);
      Start_Timer=0;
      
    }
      Enter_Timer=0;
      if(Timer_Min1 !=200 && Timer_Min2 != 200 && Timer_Sec1 != 200 && Timer_Sec2 != 200)
      {
      Timer0A_countdwon();
      Start_SW=1;
    }
      
      */
   }
    
    
    
  }
  
} 









