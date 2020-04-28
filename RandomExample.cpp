/*
 * RandomExample.cpp
 *
 *  Created on: 22-Apr-2020
 *      Author: biman
 */

#include "mbed.h"
#include <stdlib.h>
#include "us_ticker_api.h"

int main(void)
{
	while(1) {
		unsigned int t = us_ticker_read();
		printf("Seed Number: %u\n", t);
		srand(t);
		unsigned int r = rand();
		printf("Random Number: %u\n", r);
		thread_sleep_for(937);
	}
}
