
#include "keypad.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "lcd.h"

#include "tm4c123gh6pm.h"

#include "bitwise_operation.h"

uint8 key;
extern int Mode;
char z='\0';
void delay_us(int time)
{
  int i, j;
  for(i = 0 ; i < time; i++)
  {
    for(j = 0; j < 3; j++)
    {
    }
  }
  
}

/*void KeyPad_Init(void)
{
  //PORT_Configuration PTR={UNLOCKED,0xFF,0x0F,0xFF,0xFF};
  //DIO_init(PORTB,&PTR);//COL
  //DIO_init(PORTD,&PTR);//ROW
  //GPIO_PORTB_ODR_R=0xFF;
  //GPIO_PORTD_ODR_R=0xFF;
  
  //ROW
  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2);
  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4);
  GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5);
  
  //COL
  GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4);
  GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_5);
  GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);
  GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_7);
  
}
*/

//TARE2T SHA3RAWY AND YOMNA 
//make void and replace every return with key=
static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number)
{
	uint8 keypad_button = 0;
	switch(button_number)
	{
          
		case 1: keypad_button = 1;
				break;
		case 2: keypad_button = 2;
				break;
		case 3: keypad_button = 3;
				break;
		case 4: keypad_button = '+'; // ASCII Code of '+'
				break;
		case 5: keypad_button = 4;
				break;
		case 6: keypad_button = 5;
				break;
		case 7: keypad_button = 6;
				break;
		case 8: keypad_button = '-'; // ASCII Code of '-' 
				break;		
		case 9: keypad_button =  7;
				break;
		case 10: keypad_button = 8;
				break;
		case 11: keypad_button = 9;
				break;
		case 12: keypad_button = '/'; // ASCII Code of '/' 
				break;
		case 13: keypad_button = '*';  // ASCII of '*' 
				break;			
		case 14: keypad_button = 0;
				break;
		case 15: keypad_button = '#'; // ASCII Code of '=' 
				break;
		case 16: keypad_button = '=' ; // For changing Modes 
				break;
		default: keypad_button = button_number;
				//break;

/*
case 1: keypad_button = 7;
				break;
		case 2: keypad_button = 8;
				break;
		case 3: keypad_button = 9;
				break;
		case 4: keypad_button = '%'; // ASCII Code of %
				break;
		case 5: keypad_button = 4;
				break;
		case 6: keypad_button = 5;
				break;
		case 7: keypad_button = 6;
				break;
		case 8: keypad_button = '*'; // ASCII Code of '*' 
				break;		
		case 9: keypad_button = 1;
				break;
		case 10: keypad_button = 2;
				break;
		case 11: keypad_button = 3;
				break;
		case 12: keypad_button = '-'; // ASCII Code of '-' 
				break;
		case 13: keypad_button = 13;  // ASCII of Enter 
				break;			
		case 14: keypad_button = 0;
				break;
		case 15: keypad_button = '='; // ASCII Code of '=' 
				break;
		case 16: keypad_button = '+'; // ASCII Code of '+' 
				break;
		default: keypad_button = button_number;
				//break;
                */

	}
	return keypad_button;
} 




uint8 KeyPad_Read(void)
{
  
  uint8 rows=0,cols=0;
  //uint8 value=0;
  
  //COLS //el asas port e col PE0->PE3
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_1);
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2);
  GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_3);
  
  GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_0,LOGIC_HIGH);
  GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_1,LOGIC_HIGH);
  GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_2,LOGIC_HIGH);
  GPIOPinWrite(GPIO_PORTE_BASE,GPIO_PIN_3,LOGIC_HIGH);

    //ROWS //el asas port C col PC4->PC7
  GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);
  GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_5);
  GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6);
  GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_7);
  
  /*
  GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_4,LOGIC_HIGH);
  GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_5,LOGIC_HIGH);
  GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_6,LOGIC_HIGH);
  GPIOPinWrite(GPIO_PORTC_BASE,GPIO_PIN_7,LOGIC_HIGH);
*/

  GPIO_PORTC_PUR_R = 0xFF;
  GPIO_PORTC_ODR_R = 0xFF;
  
  
  
  //uint32 arr[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
  /* for(rows=0;rows<4;rows++)
  {
  switch(rows)
  {
case 0:
  GPIO_PORTA_DATA_R=0x0E;
  break;
case 1:
  GPIO_PORTA_DATA_R=0x0D;
  break;
case 2:
  GPIO_PORTA_DATA_R=0x0B;
  break;
case 3:
  GPIO_PORTA_DATA_R=0x07;
  break;
  default:
  GPIO_PORTA_DATA_R=0x0F;
  break;
}
  value=DIO_readPort(PORTB);
  value=value>>4;
  switch(value)
  {
case 0x0E:
  cols=0;
  key= arr[rows][cols];
  break;
case 0x0D:
  cols=1;
  key= arr[rows][cols];
  break;
case 0x0B:
  cols=2;
  key= arr[rows][cols];
  break;
case 0x07:
  cols=3;`
  key= arr[rows][cols];
  break;
  default:
  cols=1;
  break;
}
}
  rows=3;
  key= arr[rows][cols];*/
  
  while(Mode==1 || Mode==3)//Mode==1 || Mode==3
  {

    for( cols = 0; cols < 4; cols++)                        //columns traverse
    {
      //GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0 << rows );
      
      //GPIOPinWrite(GPIO_PORTE_BASE, (GPIO_PIN_0 << cols) , KEYPAD_BUTTON_PRESSED  );
      //GPIO_PORTE_DATA_R=0X01<<cols;
      //GPIO_PORTE_DATA_R =0xFF << cols;
      CLEAR_BIT(GPIO_PORTE_DATA_R,cols);
      for( rows = 0; rows < 4; rows++)                     //rows traverse
      {
        
        
        if(GPIOPinRead(GPIO_PORTC_BASE, (GPIO_PIN_4 << rows ) )== KEYPAD_BUTTON_PRESSED)// GET_BIT(GPIO_PORTC_DATA_R,rows) ==0
        {
          //key= arr[rows-GPIO_PIN_4][cols-GPIO_PIN_0];
          //key= ((rows*4)+cols+1); 
          //key= arr[rows-
          //key= arr[rows][cols];
          return KEYPAD_4x4_adjustKeyNumber(((rows*4)+cols+1));
          
        }
        
      }
      //GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0 << rows );
      //GPIOPinWrite(GPIO_PORTE_BASE, (GPIO_PIN_0 << cols) , KEYPAD_BUTTON_RELEASED  );
      //GPIO_PORTE_DATA_R =(0X01<<cols);
      SET_BIT(GPIO_PORTE_DATA_R,cols);
      //delay_us(10);
    }
    
    
    
  }
  
 return 100;
}
  /* keypad_getkey() function returns the value of key pressed by the user by traversing columns
and rows respectivley */
/*
  while(1)
  {
    for(int i = 0; i < 4; i++)    //Scan columns loop
    {
      GPIO_PORTC_DATA_R = (1U << i+4);
      delay_us(2);
      for(int j = 0; j < 4; j++)  //Scan rows
      {
        if((GPIO_PORTE_DATA_R & 0x1E) & (1U << j+1))
         // key= KEYPAD_4x4_adjustKeyNumber(((i*4)+j+1));
          key= arr[j][i];
      }
    }
  }
*/

/* Keypad_Init() configures PORTC and PORTE to scan keypad keys */
/* //INTERNET
void keypad_Init(void)
{
 SYSCTL_RCGCGPIO_R |= 0x14;        //Enable clock to PORTC and PORTE  

  
  while ((SYSCTL_RCGCGPIO_R & 0x14)==0);  //wait for clock to be setted
  
  GPIO_PORTC_CR_R  |= 0xF0;             //Allow settings for all pins of PORTC
  GPIO_PORTE_CR_R  |= 0x1E;             //Allow settings for all pins of PORTD
  GPIO_PORTE_DIR_R |= 0x00;             //PE1-PE4 are used with row and set them as digital input pins
  GPIO_PORTE_PDR_R |= 0x1E;             //Enable pull down resistor on PORTE
  GPIO_PORTC_DEN_R |= 0xF0;             //Set PORTC as digital pins
  GPIO_PORTE_DEN_R |= 0x1E;             //Set PORTE as digital pins
}
*/
