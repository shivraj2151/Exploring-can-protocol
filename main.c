#include "delay.h"
#include "LCD_defines.h"
#include "LCD.h"
#include "adc.h"
#include "adc_defines.h"
#include "types.h"
#include "interrupt.h"
#include "can.h"
#include "can_defines.h"

	f32 result; 
	u32 fuel_per;

main(){
	CANF txF;
	Init_CAN1();
	InitLCD();
	Init_ADC();
	txF.ID=1;
	txF.BFV.RTR=0;
	txF.BFV.DLC=0x4;
	txF.Data1=1;
	txF.Data2=0;
	cmdLCD(0x0f);
	while(1){
		result = Read_ADC(0x01);
		f32LCD(result,4);
		//delay_s(1);
		fuel_per = ((result-0.28)/(2.42-0.28))*100;	  
		if(fuel_per>=100){
			fuel_per=100;
		}
		else if(fuel_per<=0){
			fuel_per=0;
		}
		cmdLCD(GOTO_LINE1_POS0);
		u32LCD(fuel_per);
		delay_s(1);
		cmdLCD(CLEAR_LCD); 
		txF.Data1=fuel_per;
		txF.Data2=0;
		CAN1_Tx(txF);		
		txF.Data1=0;
	}
}

