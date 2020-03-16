#include <stdio.h>
#include "mbed.h"


// Blinking rate in milliseconds
#define BLINKING_RATE_MS                                                    500


int main(void) {
        DigitalOut led(LED1);

        while (true) {
                led = !led;
                thread_sleep_for(BLINKING_RATE_MS);
                printf(".\n");
        }
}

