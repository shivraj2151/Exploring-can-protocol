//can_rx_test.c
#include <lpc21xx.h>
#include "can.h"
#include "delay.h"
#include "defines.h"
int main()
{
	CANF rxF;
	Init_CAN1();
	
	while(1)
	{
 		 CAN1_Rx(&rxF);		
	 	WRITEBYTE(IOPIN0,0,rxF.Data1);
		 delay_ms(500);
  }		
}
