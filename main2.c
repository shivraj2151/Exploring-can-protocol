#include<LPC21xx.h>
#include "delay.h"
#include "LCD_defines.h"
#include "LCD.h"
#include "adc.h"
#include "adc_defines.h"
#include "types.h"
#include "interrupt.h"
#include "can.h"
#include "can_defines.h"
#include "ds18b20.h"
#include "rtc.h"
#include "rtc_defines.h"
#include "defines.h"
//#include "uart.h"

char cgram_lut[] = {0x0e,0x11,0x11,0x11,0x11,0x11,0x11,0x1f};
s32 hour,min,sec,date,month,year,day;
s32 temp;
u8 tp,tpd;
u32 flag1=0, flag2=0;
CANF rxF, txF;
int rc_cnd(void)
{
	return (READBIT(C1GSR,RBS_BIT))?1:0;
}
main(){

	InitLCD();
	Init_CAN1();
	RTC_Init();
	Enable_EINT0();
    Enable_EINT1();
  //  uart0_init(9600);  // Initialize UART0 at 9600 baud

	rxF.Data1=0;

	SetRTCTime(1,14,0);
	SetRTCDate(02,04,2025);
	SetRTCDay(3);


	while(1){

		//for reading, display the date and time
		GetRTCTime(&hour,&min,&sec);
		cmdLCD(0x80);
		DisplayRTCTime(hour,min,sec);
		GetRTCDate(&date,&month,&year);	
		cmdLCD(0xC0);
		DisplayRTCDate(date,month,year);
		GetRTCDay(&day);
		DisplayRTCDay(day);

		//for reading and display the temprature
		temp=ReadTemp(); 
		tp  = temp>>4;	 
		tpd=temp&0x08?0x35:0x30;
		cmdLCD(0x94);
 		strLCD("Temp =");
		s32LCD(tp);
		charLCD('.');
		charLCD(tpd);
		strLCD(" C");
	
		  if(flag1==1){
		  	  flag1=0;
		  }
		  if(flag2==1){
		  	  flag2=0;
		  }
		  
		//For reading the fuel percentage
		if(rc_cnd())
		{
		CAN1_Rx(&rxF);
			if(rxF.ID==1){
			cmdLCD(0xD4);
			strLCD("FUEL=");
			u32LCD(rxF.Data1);
			//delay_s(1);
			//cmdLCD(CLEAR_LCD);
		}
		}
	}
}

