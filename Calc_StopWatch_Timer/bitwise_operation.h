#ifndef BITWISE_OPERATION_
#define BITWISE_OPERATION_

/* Set a certain bit in any register */
#define SET_BIT(REG,BIT) (REG|=(1U<<BIT))

/* Clear a certain bit in any register */
#define CLEAR_BIT(REG,BIT) (REG&=(~(1U<<BIT)))

/* Toggle a certain bit in any register */
#define TOGGLE_BIT(REG,BIT) (REG^=(1U<<BIT))

#define GET_BIT(REG,BIT) ((REG&(1U<<BIT))>>BIT)

/* Check if a specific bit is set in any register and return true if yes */
#define BIT_IS_SET(REG,BIT) (REG&(1<<BIT))

/* Check if a specific bit is cleared in any register and return true if yes */
#define BIT_IS_CLEAR(REG,BIT) (!(REG&(1<<BIT)))

#endif