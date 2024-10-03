/*******************************************************************************
*                                                                              *
* Module: Port                                                                 *
*                                                                              *
* File Name: Port.h                                                            *
*                                                                              *
* Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.     *
*                                                                              *
* Author: Team9                                                                *  
********************************************************************************/

#ifndef PORT_H
#define PORT_H


/* Id for the company in the AUTOSAR */
#define PORT_VENDOR_ID    (1000U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
* Module Version 1.0.0
*/
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
* AUTOSAR Version 4.0.3
*/
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (3U)
#define PORT_AR_RELEASE_PATCH_VERSION   (1U)

/*
* Macros for Port Status
*/
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif

/* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"
/*******************************************************************************
*                           For MISRA rules                                    *        
********************************************************************************/
#define ZERO (0U)
#define UNLOCK (0x4C4F434BU)

/*******************************************************************************
*                      API Service Id Macros                                   *
********************************************************************************/
/* Service ID for Port Init */
#define PORT_INIT_SID                   (uint8)0x00

/* Service ID for Port Set Pin Direction */
#define PORT_SET_PIN_DIRECTION_SID      (uint8)0x01

/* Service ID for Port Refresh Port Direction */
#define PORT_REFRESH_PORT_DIRECTION_SID (uint8)0x02

/* Service ID for Port Get Version Info*/
#define PORT_GET_VERSION_INFO_SID      (uint8)0x03

/* Service ID for Port Set Pin Mode */
#define PORT_SET_PIN_MODE_SID          (uint8)0x04

/*******************************************************************************
*                      DET Error Codes                                         *
********************************************************************************/
/* DET code to report Invalid Port Pin ID.*/
#define PORT_E_PARAM_PIN               (uint8)0x0A

/* DET code to report Port Pin not configured as changeable.*/
#define PORT_E_DIRECTION_UNCHANGEABLE  (uint8)0x0B

/* DET code to repor Port_Init API service called with NULL pointer parameter.*/
#define PORT_E_PARAM_CONFIG            (uint8)0x0C

/* DET code to repor API Port_SetPinMode service called when mode passed not valid.*/
#define PORT_E_PARAM_INVALID_MODE      (uint8)0x0D

/* DET code to repor API Port_SetPinMode service called when mode is unchangeable.*/
#define PORT_E_MODE_UNCHANGEABLE       (uint8)0x0E

/* DET code to repor API service used without module initialization.*/
#define PORT_E_UNINIT                  (uint8)0x0F

/* DET code to repor API service shall return immediately without any further action.*/
#define PORT_E_PARAM_POINTER            (uint8)0x10


/*******************************************************************************
*                              Module Definitions                              *
********************************************************************************/

/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000

/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C

/*******************************************************************************
*                              Module Data Types                               *
********************************************************************************/
/* Description: Data type for the name of a port. */
typedef uint8 Port_PortType;
/* Description: Data type for the name of a port pin. */
typedef uint8 Port_PinType;
/* Description: Data type for different port pin modes. */
typedef uint8 Port_PinModeType;
typedef uint8 Port_PinModeInstanceType;
//typedef uint8 Port_PinMode;
/* Description: Data type for direction changeable in runtime.*/
typedef boolean Port_PinDirectionChangeable;
/* Description: Data type for pin mode changeable in runtime.*/
typedef boolean Port_PinModeChangeable;


/* Description: Enum to hold PIN direction */
typedef enum
{
  PORT_PIN_IN,
  PORT_PIN_OUT
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
  OFF,
  PULL_UP,
  PULL_DOWN
}Port_InternalResistor;

/* Description: Enum to hold port pin level value */
typedef enum
{
  PORT_PIN_LEVE_LOW,
  PORT_PIN_LEVEL_HIGH
}Port_PinLevelValue;

/* Description: Enum to hold port pin initial modes */
typedef enum 
{ 
  PORT_PIN_INITIAL_MODE_DIO,                    
  PORT_PIN_INITIAL_MODE_ADC,
  PORT_PIN_INITIAL_MODE_CAN,
  PORT_PIN_INITIAL_MODE_DIO_GPT,
  PORT_PIN_INITIAL_MODE_DIO_WDG,
  PORT_PIN_INITIAL_MODE_USB,
  PORT_PIN_INITIAL_MODE_I2C,
  PORT_PIN_INITIAL_MODE_PWM,
  PORT_PIN_INITIAL_MODE_SSI,
  PORT_PIN_INITIAL_MODE_UART,
  PORT_PIN_INITIAL_MODE_NMI
    
}Port_PinInitialMode;

/* Description: Enum to hold port pin modes */
typedef enum 
{ 
  PORT_PIN_MODE_DIO,                    
  PORT_PIN_MODE_ADC,
  PORT_PIN_MODE_CAN,
  PORT_PIN_MODE_DIO_GPT,
  PORT_PIN_MODE_DIO_WDG,
  PORT_PIN_MODE_USB,
  PORT_PIN_MODE_I2C,
  PORT_PIN_MODE_PWM,
  PORT_PIN_MODE_SSI,
  PORT_PIN_MODE_UART,
  PORT_PIN_MODE_NMI
    
}Port_PinMode;

/* Description: Structure to configure each individual PIN:
*      1. the PORT Which the pin belongs to. 0, 1, 2, 3, 4 or 5
*      2. the number of the pin in the PORT.
*      3. the direction of pin --> INPUT or OUTPUT
*      4. the internal resistor --> Disable, Pull up or Pull down
*      5. the pin mode type
*      6. the pin mode instance -->0 : 9
*      7. the pin level value --> low level ,High level 
*      8. the pin direction changeable in runtime --> ON, OFF
*      9. the pin mode changeable in runtime --> ON, OFF
*/
typedef struct 
{
  Port_PortType port_num; 
  Port_PinType pin_num;
  Port_PinDirectionType pin_direction;
  Port_InternalResistor pin_resistor;
  Port_PinMode pin_mode;
  Port_PinModeInstanceType pin_mode_instance;
  Port_PinLevelValue pin_initial_value; 
  Port_PinDirectionChangeable pin_change_direction;
  Port_PinModeChangeable pin_change_mode;
  
}Port_ConfigChannel;

/* Data Structure required for initializing the Port Driver */
typedef struct Port_ConfigType
{
  Port_ConfigChannel Channels[PORT_CONFIGURED_CHANNLES];
} Port_ConfigType;

#define PTR_IS_NULL             1
/*******************************************************************************
*                      Function Prototypes                                     *
********************************************************************************/

/*******************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
********************************************************************************/
void Port_Init( const Port_ConfigType* ConfigPtr );

/*******************************************************************************
* Service Name: Port_SetPinDirection
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number & Direction - Port Pin Direction:
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction
********************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction );
#endif

/*******************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
********************************************************************************/
void Port_RefreshPortDirection( void );

/*******************************************************************************
* Service Name: Port_GetVersionInfo
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non-reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfo - Pointer to where to store the version information of this module.
* Return value: None
* Description: Returns the version information of this module:
*                                                             - Module Id
*                                                             - Vendor Id
*                                                             - Vendor specific version numbers
********************************************************************************/
/* Function for Port Get Version Info API */
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
#endif

/*******************************************************************************
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin - Port Pin ID number & Mode - New Port Pin mode to be set on port pin.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin mode.
********************************************************************************/
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode );
#endif

/************************************************************************************
* Function Name : Port_ErrorState
* Inputs        : uint8 error_code
* Outputs       : boolean value (True or False)
* Reentrancy    : reentrant
* Sync/Async    : Synchronous
* Description   : Returns True if an error was reported.
************************************************************************************/
boolean Port_ErrorState(uint8 error_code);

/*******************************************************************************
*                       External Variables                                     *
********************************************************************************/

/* Extern PB structures to be used by Dio and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H */
