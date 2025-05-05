#include <stdio.h>
#include "types.h"
#include <lpc21xx.h>
#include "lcd_defines.h"
#include "defines.h"
#include "delay.h"
#include "lcd.h"
#include <math.h>

void writeLCD(u8 byte){

  IOCLR0=1<<LCD_RW;

  WRITEBYTE(IOPIN0,LCD_DATA,byte);

  IOSET0=1<<LCD_EN;

  delay_us(1);

  IOCLR0=1<<LCD_EN;

  delay_ms(2);

}

void cmdLCD(u8 cmd){

    IOCLR0=1<<LCD_RS;

 writeLCD(cmd);

}

void InitLCD(void){

  WRITEBYTE(IODIR0,LCD_DATA,0XFF);

  SETBIT(IODIR0,LCD_RS);

  SETBIT(IODIR0,LCD_RW);

  SETBIT(IODIR0,LCD_EN);

  delay_ms(15);

  cmdLCD(0x30);

  delay_ms(4);

  delay_us(100);

  cmdLCD(0x30);

  delay_us(100);

  cmdLCD(0X30);

  cmdLCD(MODE_8BIT_2LINE);

  cmdLCD(DSP_ON_CUR_BLINK);

  cmdLCD(CLEAR_LCD);

  cmdLCD(SHIFT_CUR_RIGHT);

}

void charLCD(u8 ascii){

  IOSET0=1<<LCD_RS;

  writeLCD(ascii);

}

void setcursor(u8 lineNo,u8 pos){

  if(lineNo==1)

  cmdLCD(GOTO_LINE1_POS0+pos);

  else if(lineNo==2)

  cmdLCD(GOTO_LINE2_POS0+pos);

}

void strLCD(s8 *str){

     while(*str){

     charLCD(*str++);

  }

}

void s32LCD(s32 num){

    s8 buff[10];

    if(num<0){

        charLCD('-');

        num=-num;

    }

    sprintf((char *)buff, "%d", num);

    strLCD(buff);

}
 void f32LCD(f32 fNum,u32 nDP)

{

	s32 iNum;

	if(fNum<0.0)

	{

		//display '-' on LCD

		charLCD('-');		

		//change the sign of the given number

		fNum = -fNum;	

	}

	//assign float number to integer variable	

	iNum = fNum;

	//display integer value on LCD

	u32LCD(iNum);

	//display '.' on LCD

	charLCD('.');

	//extract digit by digit after decimal point and display it on LCD up to given no.of digits after decimal point

	while(nDP)

	{

		fNum = (fNum-iNum)*10;

		iNum=fNum;

		charLCD(iNum+48);

		nDP--;

	}

}

/*
void f32LCD(float num, int point){

    int integerpart = (int)num;

    int floatpart = (int)((num-integerpart)*((int)(pow(10, point))));

    

    if(num<0){

        charLCD('-');

        integerpart=-integerpart;

        floatpart=-floatpart;

    }

    

    s32LCD(integerpart);

    charLCD('.');

    if(floatpart<0){

        floatpart=-floatpart;

    }

    s32LCD(floatpart);

} */
void u32LCD(u32 num)

{

	u8 a[10];

	s32 index=0;

	if(num==0)

	{

		//display 0 on lcd

		charLCD('0');

	}

	else

	{

		//extract digit by digit and store it into array

		while(num)

		{

			a[index] = (num%10) + 48;//48/0x30/'0'

			index++;

			num = num/10;

		}

		//display digit by digit on LCD 

		for(--index;index>=0;index--)

			charLCD(a[index]);	

	}

}

