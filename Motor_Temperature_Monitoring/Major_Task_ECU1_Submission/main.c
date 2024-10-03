#include "ECU1_App.h"
#include "OS.h"
#include "Port.h"

int main()
{
  Port_Init(&Port_Configuration);
  App_Init();
  Scheduler();
  return 0;
}
