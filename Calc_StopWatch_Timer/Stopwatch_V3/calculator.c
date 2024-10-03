//Included libraries
#include "calculator.h"
#include <stdio.h>
#include "math.h"

//#defines and global variables
#define ARRAY_SIZE 11
uint8 size=0;
uint8 operation=0;
uint8 number1_size=0;

uint16 calculator (uint8* array)
{
  uint8 x[ARRAY_SIZE],i;
  uint16 number1,number2;
  for (i=0;array[i]!='=';i++){
    x[i]=array[i];
//    delay(1000);
      if ((array[i]=='+')||(array[i]=='-')||(array[i]=='*')||(array[i]=='/')){
        operation=i;
      }
    }
  size=i;
  number1=FirstNumber(x);
  number2=SecondNumber(x);
  return Result(x,number1,number2);
}

static uint16 FirstNumber (uint8 * array)
{
  uint8 mul=1,y,number_1;
   for (y=0;(array[y]!='+')||(array[y]!='-')||(array[y]!='*')||(array[y]!='/');y++){
      mul=pow(10,operation-1-y);
      number_1+=(array[y]*mul);
    }
   number1_size=y;
   return number_1;
}

static uint16 SecondNumber (uint8 * array)
{
  uint8 mul=1,number_2;
  uint8 i=number1_size;
   while(i<size){
      mul=pow(10,size-1-operation);
      i++;
      number_2+=(array[i]*mul);
    }
   return number_2;
}

uint8 Result(uint8* array, uint16 Number1, uint16 Number2)
{
  uint16 result;
   switch (array[operation]){
      case '+':
        result=Number1+Number2;
      break;
      case '-':
        result=Number1-Number2;
      break;
      case '*':
        result=Number1*Number2;
      break;
      case '/':
        result=Number1/Number2;
      break;
    }
   return result;
}