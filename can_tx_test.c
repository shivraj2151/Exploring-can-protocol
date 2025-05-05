//can_tx_test.c
#include "can.h"
#include "delay.h"
#include <string.h>
#include <stdlib.h>
int main()
{
	//CANF rxF;
	CANF txF;
	Init_CAN1();
	txF.ID=1;
	txF.BFV.RTR=0;
	txF.BFV.DLC=1;
	txF.Data1=1;
	txF.Data2=0;
	while(1)
	{
   	CAN1_Tx(txF);		
	 txF.Data1=((~txF.Data1)&1);	
	 delay_ms(500);
		
  }		
}
