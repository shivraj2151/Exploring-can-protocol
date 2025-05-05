#include <LPC21xx.h>
#include"can.h"
#include "delay.h"
#include "lcd.h"
#define LED_AL 0
   	CANF rxF;
int rc_cnd(void)
{
	return (READBIT(C1GSR,RBS_BIT))?1:0;
}

main()
{
	s32 i;
	u32 cnt=0;

	Init_CAN1();
    IODIR0|=255<<LED_AL;
    IOPIN0|=255<<LED_AL;
	
	rxF.Data1=0;
        while(1)
        {
               if(rc_cnd())
			   {
				CAN1_Rx(&rxF);
                if(rxF.ID==2)
                {
                        if(rxF.Data1==76)
                        {
							cnt=5;
							while(cnt)
							{
                                for(i=0;i<=7;i++)
                                {
                                        IOPIN0&=~(1<<LED_AL+i);
                                        delay_ms(100);
                                }
								IOPIN0|=255<<LED_AL;
								cnt--;
							}
								
                        }
						
						else if(rxF.Data1==82)
                		{
							cnt=5;

							while(cnt)
							{
                        		for(i=7;i>=0;i--)
                        		{
                                	IOPIN0&=~(1<<LED_AL+i);
                                	delay_ms(100);
                        		}
								IOPIN0|=255<<LED_AL;
								cnt--;
							}
                		}
           			}
					IOPIN0|=255<<LED_AL;
        	}
			}
}
