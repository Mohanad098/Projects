/*
 * LCD.h
 *  Author:
 *      Ahmet Eren ODACI
 *
 *  Credit:
 *      Lokman GÖKDERE
 *      http://www.mcu-turkey.com/stellaris-launchpad-16x2-lcd/
 */
//CALC DONE 25/12
#ifndef LCD_H_
#define LCD_H_
#include "types.h"

#define LCDPORT         GPIO_PORTB_BASE
#define LCDPORTENABLE   SYSCTL_PERIPH_GPIOB
#define RS              GPIO_PIN_0
#define E               GPIO_PIN_1
#define D4              GPIO_PIN_4
#define D5              GPIO_PIN_5
#define D6              GPIO_PIN_6
#define D7              GPIO_PIN_7


void LCD_init(void);                                //LCD initialization
void LCD_Command(unsigned char c);                  //Send command
void LCD_Show(unsigned char d);                     //Show a char

void LCD_Clear(void);                               //Clear the screen
void LCD_Print(char *s);                  //Print 2 lines
void LCD_PrintLn(char i, char *s);                  //Print specific line
void LCD_PrintJustify(char i, char *s, char *d);    //Print specific line floated left and floated right text
void LCD_Cursor(char x, char y);                    //Set cursor
void SysCtlDelay_Ehab(int n);
void LCD_Write_Integer(unsigned long data);
void LCD_Write_Decimal(float32 data);


#endif /* LCD_H_ */
