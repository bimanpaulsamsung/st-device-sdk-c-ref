#include "cmsis_os2.h"
#include <stdio.h>
#include "mbed.h"
#include "WhdSoftAPInterface.h"

// Blinking rate in milliseconds
#define BLINKING_RATE_MS                                                    500

osThreadId_t  tid_phaseA;// Thread id of thread “phaseA”

void phaseA(void) {//Some application thread...some processing
        printf("IN phaseA()\n");

        DigitalOut led(LED1);

        while (true) {
                led = !led;
                thread_sleep_for(BLINKING_RATE_MS);
//                printf(".\n");
        }
}

void app_main(void) {
        Thread thread;
        thread.start(phaseA);

        printf("IN app_main()\n");
        osDelay(osWaitForever);
        //app_mainnever ready again
        while (1);
}

static int connect_to_ap(char *wifi_ssid, char *wifi_password,
                nsapi_security_t security = NSAPI_SECURITY_WPA_WPA2)
{
        WiFiInterface *wifi;
        wifi = WiFiInterface::get_default_instance();
        if (!wifi) {
            printf("ERROR: No WiFiInterface found.\n");
            return -1;
        }

        printf("\nConnecting to %s...\n", wifi_ssid);
        int ret = wifi->connect(wifi_ssid, wifi_password, security);
        if (ret != 0) {
            printf("\nConnection error: %d\n", ret);
            return -1;
        }

        printf("Success\n\n");
        printf("MAC: %s\n", wifi->get_mac_address());
        printf("IP: %s\n", wifi->get_ip_address());
        printf("Netmask: %s\n", wifi->get_netmask());
        printf("Gateway: %s\n", wifi->get_gateway());
        printf("RSSI: %d\n\n", wifi->get_rssi());
        return 0;
}

int main(void) {
        connect_to_ap("Psense", "psense019");

        Thread thread;
        thread.start(app_main);
        printf("IN main()\n");
        while(1);//will not execute unless above fails
}

