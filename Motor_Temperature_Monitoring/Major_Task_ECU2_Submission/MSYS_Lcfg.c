/******************************************************************************
*
* Module: SYSTICK
*
* File Name: MYSYS_cfg.c
*
* Description: Configuration source file for TM4C123GH6PM Microcontroller - Systick driver .
*
* Author: Team 9
******************************************************************************/


#include "MSYST_Int.h"
#include "MSYS_cfg.h"

const SYS_Config Config =
{
  .CLKSource                    = SYSTEM_CLOCK,
  .Int_Enable                   = ENABLE,
  .PreloadValue                 = TICKS_TIME,
  .Systick_Interupt_Priority    = PRIORITY3
  
  
};