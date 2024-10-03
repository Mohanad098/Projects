/******************************************************************************
 *
 * Module: CAN
 *
 * File Name: CAN_Lcfg.c
 *
 * Description: Link time Configuration Source file for TM4C123GH6PM Microcontroller - CAN Driver
 *
 * Author: Team 9 
 ******************************************************************************/
#include "CAN.h"

/* Structure used with CAN_Init API */
const Can_Config_type Can_configuration = { CAN0 , TRANS_MSG_ID, BR_1MBPS, OBJ_NUM_1 ,DATA_8_BYTE };