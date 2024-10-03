
#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "types.h"




/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* Keypad configurations for number of rows and columns */
#define KEYPAD_NUM_COLS                   4
#define KEYPAD_NUM_ROWS                   4

/* Keypad Port Configurations */
#define KEYPAD_ROW_PORT_ID                PORTC_ID
#define KEYPAD_FIRST_ROW_PIN_ID           PIN4_ID

#define KEYPAD_COL_PORT_ID                PORTE_ID
#define KEYPAD_FIRST_COL_PIN_ID           PIN0_ID

/* Keypad button logic configurations */
#define KEYPAD_BUTTON_PRESSED            LOGIC_LOW
#define KEYPAD_BUTTON_RELEASED           LOGIC_HIGH

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Get the Keypad pressed button
 */
void KeyPad_Init(void);
uint8 KEYPAD_getPressedKey(void);
uint8 KeyPad_Read(void);
void delay_us(int time);
  static uint8 KEYPAD_4x4_adjustKeyNumber(uint8 button_number);

#endif /* KEYPAD_H_ */
