#include <LPC21xx.h>
#include "can.h"
#include "delay.h"
#include "lcd.h"
#include "defines.h"
//#include "can.h"
#include "can_defines.h"


#define LED_AL 0
CANF rxF;

// State variables
int isBlinking = 0;      // Flag to indicate if LEDs are blinking or not
int lastData = -1;       // Store last received data (initialize with an invalid value)

// Function to check if CAN message is ready
int rc_cnd(void) {
    return (READBIT(C1GSR, RBS_BIT)) ? 1 : 0;
}

// Function to start the LED blinking pattern based on the received data
void startBlinking(u8 data) {
    static u32 cnt = 0;   // Counter to control blinking cycles
    int i;

    if (data == 82) {  // Left to right blink (7 to 0)
        for (i = 7; i >= 0; i--) {
            IOPIN0 &= ~(1 << (LED_AL + i));  // Turn off LED
            delay_ms(100);                   // Delay between LED switches
        }
        IOPIN0 |= 255 << LED_AL;  // Turn all LEDs on
    }
    else if (data == 76) {  // Right to left blink (0 to 7)
        for (i = 0; i <= 7; i++) {
            IOPIN0 &= ~(1 << (LED_AL + i));  // Turn off LED
            delay_ms(100);                   // Delay between LED switches
        }
        IOPIN0 |= 255 << LED_AL;  // Turn all LEDs on
    }

    // Keep LEDs on after the blinking
    IOPIN0 |= 255 << LED_AL;
}

// Main loop
main() {
    s32 i;
    u32 cnt = 0;

    Init_CAN1();
    IODIR0 |= 255 << LED_AL;   // Set LED pins to output
    IOPIN0 |= 255 << LED_AL;   // Ensure LEDs are off initially

    rxF.Data1 = 0;
    
    while (1) {
        if (rc_cnd()) {  // If a CAN message is received
            CAN1_Rx(&rxF);

            if (rxF.ID == 2) {  // Only process messages with ID == 2
                // Check if data is different from last received value to start blinking
                if (rxF.Data1 != lastData) {
                    if (rxF.Data1 == 82 || rxF.Data1 == 76) {  // Only blink on Data1 == 82 or Data1 == 76
                        if (isBlinking) {
                            // Stop blinking if the same message comes again
                            isBlinking = 0;
                            IOPIN0 |= 255 << LED_AL;  // Turn off all LEDs
                        } else {
                            // Start blinking
                            isBlinking = 1;
                            startBlinking(rxF.Data1);  // Start LED blinking with the received data
                        }
                    }

                    // Update the last received data
                    lastData = rxF.Data1;
                }
            }
        }

        // Optional: You can add a small delay to prevent CPU overload
        delay_ms(50);  // Adjust as necessary
    }
}
