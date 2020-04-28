#include <stdbool.h>
#include <stdio.h>
#include "mbed.h"
#include "cmsis_os2.h"
#include "us_ticker_api.h"

InterruptIn button(USER_BUTTON);
DigitalOut led(LED1);

static int timedown;

void highLevel()
{
	led = 1;
	int usec = us_ticker_read();

	printf("Press Duration: %d\n", usec - timedown);
	if (usec - timedown > 3000000)
		printf("LONG PRESS\n");
}

void lowLevel()
{
	led = 0;
	timedown = us_ticker_read();
}

void flip() {
    led = !led;
    printf("Button Rise: %d\n", led.read());
}

int main() {
    button.rise(mbed_event_queue()->event(highLevel));
    button.fall(mbed_event_queue()->event(lowLevel));
    while(1) {           // wait around, interrupts will interrupt this!
    	thread_sleep_for(0.25);
    }
}
