/*uart0_driver.c*/
#include <LPC21xx.h>
#include "types.h"
#define THRE ((U0LSR>>5)&1)
#define RDR ((U0LSR>>0)&1)
#define ENTER_KEY '\r'

void uart0_init(u32 baud){
    PINSEL0 |= 0x5;   //P0.0-->TxD0  & P0.1--> RxD0
    U0LCR = 0x83;     //DLAB=1, 
    switch(baud){
        case 9600   : U0DLL=97;
                      break;
        case 57600  : U0DLL=16;
                      break;
        case 115200 : U0DLL=8;
                      break;
        case 921600 : U0DLL=1;
                      break;
        default     : U0DLL=97;    
    }
    U0LCR = 0x03;     //DLAB=0;
}


void uart0_tx(u8 byte){
    U0THR = byte;                //We are putting data(byte) into transmitter buffer
    while(THRE==0);              //THRE is a flag, we have defined at the top of the file, 5th bit of U0LSR, if THRE==0 means there is no data in U0THR right now
}

u8 uart0_rx(void){
    while(RDR==0);               //RDR is a flag, we have defined at the top of the file, 1st bit of U0LSR, if RDR==0 means there is no data in U0RDR right now 
    return U0RBR;
}

void uart0_tx_integer(s32 num){
    s8 digit, arr[10], i=0;
    if(num==0){
        uart0_tx(48);
        return;
    }
    if(num<0){
        uart0_tx('-');
        num=-num;
    }
    while(num){
        digit=num%10;
        arr[i]=digit;
        num/=10;
        i++;
    }
    for(--i; i>=0; i--){
        uart0_tx(arr[i]+48);
    }
}

