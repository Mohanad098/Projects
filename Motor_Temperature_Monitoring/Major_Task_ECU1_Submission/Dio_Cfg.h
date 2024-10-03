 /******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Dio Driver
 *
 * Author: Team9 
 ******************************************************************************/

#ifndef DIO_CFG_H
#define DIO_CFG_H

/*
 * Module Version 1.0.0
 */
#define DIO_CFG_SW_MAJOR_VERSION              (1U)
#define DIO_CFG_SW_MINOR_VERSION              (0U)
#define DIO_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.3.1
 */
#define DIO_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define DIO_CFG_AR_RELEASE_MINOR_VERSION     (3U)
#define DIO_CFG_AR_RELEASE_PATCH_VERSION     (1U)

/* Pre-compile option for Development Error Detect */
#define DIO_DEV_ERROR_DETECT                (STD_ON)

/* Number of the configured Dio Channels */
#define DIO_CONFIGURED_CHANNLES              (5U)
   
   
/* Channel Index in the array of structures in Dio_lcfg.c */
#define DioConf_RED_LED_CHANNEL_ID_INDEX        (uint8)0x00
#define DioConf_BLUE_LED_CHANNEL_ID_INDEX       (uint8)0x01
#define DioConf_GREEN_LED_CHANNEL_ID_INDEX      (uint8)0x02   
#define DioConf_SW1_CHANNEL_ID_INDEX            (uint8)0x03
#define DioConf_SW2_CHANNEL_ID_INDEX            (uint8)0x04

/* DIO Configured Port ID's  */
#define DioConf_RED_LED_PORT_NUM                (Dio_PortType)5 /* PORTF */
#define DioConf_BLUE_LED_PORT_NUM               (Dio_PortType)5 /* PORTF */
#define DioConf_GREEN_LED_PORT_NUM              (Dio_PortType)5 /* PORTF */
#define DioConf_SW1_PORT_NUM                    (Dio_PortType)5 /* PORTF */
#define DioConf_SW2_PORT_NUM                    (Dio_PortType)5 /* PORTF */

/* DIO Configured Channel ID's */
#define DioConf_RED_LED_CHANNEL_NUM          (Dio_ChannelType)1 /* Pin 1 in PORTF */
#define DioConf_BLUE_LED_CHANNEL_NUM         (Dio_ChannelType)2 /* Pin 2 in PORTF */
#define DioConf_GREEN_LED_CHANNEL_NUM        (Dio_ChannelType)3 /* Pin 3 in PORTF */
#define DioConf_SW1_CHANNEL_NUM              (Dio_ChannelType)4 /* Pin 4 in PORTF */
#define DioConf_SW2_CHANNEL_NUM              (Dio_ChannelType)0 /* Pin 0 in PORTF */


#endif /* DIO_CFG_H */
