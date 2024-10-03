 /******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio_Lcfg.c
 *
 * Description: Link time Configuration Source file for TM4C123GH6PM Microcontroller - Dio Driver
 *
 * Author: Team9 
 ******************************************************************************/

#include "Dio.h"

/* Dio configuration Structure */
const Dio_ConfigType Dio_Configuration = {
                                             DioConf_RED_LED_PORT_NUM,DioConf_RED_LED_CHANNEL_NUM,
                                             
                                             DioConf_BLUE_LED_PORT_NUM,DioConf_BLUE_LED_CHANNEL_NUM,
                                             
				                             DioConf_GREEN_LED_PORT_NUM,DioConf_GREEN_LED_CHANNEL_NUM,
                                             
                                             DioConf_SW1_PORT_NUM,DioConf_SW1_CHANNEL_NUM,
                                             
                                             DioConf_SW2_PORT_NUM,DioConf_SW2_CHANNEL_NUM
                                             
                                               
                           
				         };