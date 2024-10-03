/******************************************************************************
*
* Module: Port
*
* File Name: Port_Cfg.h
*
* Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
*
* Author: Team9
******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
* Module Version 1.0.0
*/
#define PORT_CFG_SW_MAJOR_VERSION                (1U)
#define PORT_CFG_SW_MINOR_VERSION                (0U)
#define PORT_CFG_SW_PATCH_VERSION                (0U)

/*
* AUTOSAR Version 4.0.3
*/
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION        (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION        (3U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION        (1U)                        



/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                  (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                  (STD_ON)

/* Pre-compile option for Port Set Pin Mode API */
#define PORT_SET_PIN_MODE_API                  (STD_ON)

/* Pre-compile option for Port Set Pin Direction API */
#define PORT_SET_PIN_DIRECTION_API             (STD_ON)

/* Pre-compile option for Port Pin Direction Changeable*/
//#define PORT_PIN_DIRECTION_CHANGEABLE        (STD_ON)

/* Pre-compile option for Port Pin Mode Changeable*/
//#define PORT_PIN_MODE_CHANGEABLE             (STD_ON)

/* Number of the configured Port pins (without JTAG pins) */
#define PORT_CONFIGURED_CHANNLES                (39U)

/* Number of the configured Port pin modes in tiva C*/
#define PORT_CONFIGURED_CHANNLES_MODES          (11U)

/*Tiva C Ports*/
#define PORT_A_ID                               (0U)
#define PORT_B_ID                               (1U)
#define PORT_C_ID                               (2U)
#define PORT_D_ID                               (3U)
#define PORT_E_ID                               (4U)
#define PORT_F_ID                               (5U)
 
/*Tiva C Pins*/
#define PIN_0_ID                                (0U)
#define PIN_1_ID                                (1U)
#define PIN_2_ID                                (2U)
#define PIN_3_ID                                (3U)
#define PIN_4_ID                                (4U)
#define PIN_5_ID                                (5U)
#define PIN_6_ID                                (6U)
#define PIN_7_ID                                (7U)

/*Tiva C Port_Pin_Index used when calling 
 *Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction) function
 *As well as Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode ) function*/
#define PORT_A_PIN_0_INDEX                      (0U)
#define PORT_A_PIN_1_INDEX                      (1U)
#define PORT_A_PIN_2_INDEX                      (2U)
#define PORT_A_PIN_3_INDEX                      (3U)
#define PORT_A_PIN_4_INDEX                      (4U)
#define PORT_A_PIN_5_INDEX                      (5U)
#define PORT_A_PIN_6_INDEX                      (6U)
#define PORT_A_PIN_7_INDEX                      (7U)
   
#define PORT_B_PIN_0_INDEX                      (8U)
#define PORT_B_PIN_1_INDEX                      (9U)
#define PORT_B_PIN_2_INDEX                      (10U)
#define PORT_B_PIN_3_INDEX                      (11U)
#define PORT_B_PIN_4_INDEX                      (12U)
#define PORT_B_PIN_5_INDEX                      (13U)
#define PORT_B_PIN_6_INDEX                      (14U)
#define PORT_B_PIN_7_INDEX                      (15U)

/*JTAG PINS will not be configured PC[0:3] Tiva C has total 43 Pins but without JTAG pins 39pins*/

//#define PORT_C_PIN_0_INDEX                      (16U)
//#define PORT_C_PIN_1_INDEX                      (17U)
//#define PORT_C_PIN_2_INDEX                      (18U)
//#define PORT_C_PIN_3_INDEX                      (19U)


/*If you want JTAG PINS to be configured PC[0:3] valuse of the below #define will change*/
#define PORT_C_PIN_4_INDEX                      (16U)
#define PORT_C_PIN_5_INDEX                      (17U)
#define PORT_C_PIN_6_INDEX                      (18U)
#define PORT_C_PIN_7_INDEX                      (19U)
   
#define PORT_D_PIN_0_INDEX                      (20U)
#define PORT_D_PIN_1_INDEX                      (21U)
#define PORT_D_PIN_2_INDEX                      (22U)
#define PORT_D_PIN_3_INDEX                      (23U)
#define PORT_D_PIN_4_INDEX                      (24U)
#define PORT_D_PIN_5_INDEX                      (25U)
#define PORT_D_PIN_6_INDEX                      (26U)
#define PORT_D_PIN_7_INDEX                      (27U)
   
#define PORT_E_PIN_0_INDEX                      (28U)
#define PORT_E_PIN_1_INDEX                      (29U)
#define PORT_E_PIN_2_INDEX                      (30U)
#define PORT_E_PIN_3_INDEX                      (31U)
#define PORT_E_PIN_4_INDEX                      (32U)
#define PORT_E_PIN_5_INDEX                      (33U)
   
#define PORT_F_PIN_0_INDEX                      (34U)
#define PORT_F_PIN_1_INDEX                      (35U)
#define PORT_F_PIN_2_INDEX                      (36U)
#define PORT_F_PIN_3_INDEX                      (37U)
#define PORT_F_PIN_4_INDEX                      (38U)

/*Mode Instance*/
#define INSTANCE_0                              (0U)
#define INSTANCE_1                              (1U)
#define INSTANCE_2                              (2U)
#define INSTANCE_3                              (3U)
#define INSTANCE_4                              (4U)
#define INSTANCE_5                              (5U)
#define INSTANCE_6                              (6U)
#define INSTANCE_7                              (7U)
#define INSTANCE_U1RTS                          (8U)
#define INSTANCE_U1CTS                          (9U)



#endif /* DIO_CFG_H */
