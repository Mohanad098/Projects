//Included libraries
//CALC DONE 25/12
#include "calculator.h"
#include <stdio.h>
#include "math.h"

//#defines and global variables
#define ARRAY_SIZE 11
uint8 size=0;
uint8 operation=0;
uint8 number1_size=0;

float32 calculator (uint8* array)
{
  uint8 x[ARRAY_SIZE];
  uint16 number1,number2;
  for (uint8 i=0;array[i]!='=';i++){
    x[i]=array[i];
    //    delay(1000);
    if ((array[i]=='+')||(array[i]=='-')||(array[i]=='*')||(array[i]=='/')){
      operation=i;
    }
    size=i;
  }
  printf("\n");
  for (uint8 a=0;a<=size;a++){
    printf("%c",x[a]);
  }
  number1=FirstNumber(x);
  number2=SecondNumber(x);
  return Result(x,number1,number2);
}

static uint16 FirstNumber (uint8 * array)
{
  uint16 mul=1;
  uint16 number_1=0;
  for (uint8 y=0;y<operation;y++){
    mul=(uint16)(pow(10,operation-1-y));
    number_1+=((array[y])*mul);
    number1_size=y;
  }
  printf("\nfirst num is %d",number_1);
  return number_1;
}

static uint16 SecondNumber (uint8 * array)
{
  uint16 number_2=0;
  uint16 mul=1;
  uint8 i=operation+1;
  while(i<=size){
    mul=(uint16)(pow(10,size-1-(i-1)));
    number_2+=((array[i])*mul);
    i++;
  }
  printf("\nsec num is %d",number_2);
  return number_2;
}

float32 Result(uint8* array, uint16 Number1, uint16 Number2)
{
  float32 result;
  switch (array[operation]){
  case '+':
   // result=(float32)((float32)(Number1)+(float32)(Number2));
    result=Number1+Number2;
    break;
  case '-':
    result=Number1-Number2;
    break;
  case '*':
    result=Number1*Number2;
    break;
  case '/':
    result=(float32)((float32)(Number1)/(float32)(Number2));
    break;
  }
  return result;
}