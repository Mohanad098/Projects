
#define SET_BIT(REG,BIT) (REG|=(1<<BIT))

#define CLEAR_BIT(REG,BIT) (REG&=(~(1<<BIT)))

#define GET_BIT(REG,BIT) ( ( REG & (1<<BIT) ) >> BIT )

#define TOGGLE_BIT(REG,BIT) (REG^=(1<<BIT))

#define BIT_IS_SET(REG,BIT) ( REG & (1<<BIT) )