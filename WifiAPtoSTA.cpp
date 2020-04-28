#include "cmsis_os2.h"
#include <stdio.h>
#include "mbed.h"
#include "WhdSoftAPInterface.h"

// Blinking rate in milliseconds
#define BLINKING_RATE_MS                                                    500

osThreadId_t tid_phaseA;// Thread id of thread “phaseA”

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
//        while (1);
}

static const uint8_t channel = 1;

void *whd_event_handler_func(whd_interface_t ifp, const whd_event_header_t *event_header,
                                     const uint8_t *event_data, void *handler_user_data)
{
        printf("IN whd_event_handler_func\n");
}

static void start_ap(char *wifi_ssid, char *wifi_password,
                nsapi_security_t security = NSAPI_SECURITY_WPA_WPA2)
{
    WhdSoftAPInterface *_wifi;
    nsapi_error_t error_code;

    _wifi = WhdSoftAPInterface::get_default_instance();
    printf("Starting AP\n");

    _wifi->register_event_handler(whd_event_handler_func);

    _wifi->set_network("192.168.1.10", "255.255.255.0", "192.168.1.1");
    // AP Configure and start

    printf("SSID:%s, Password:%s, Security: %d\n", wifi_ssid, wifi_password, security);
    error_code = _wifi->start(wifi_ssid, wifi_password, security, channel,
                  true, NULL, false);
    MBED_ASSERT(error_code == NSAPI_ERROR_OK);

    printf("AP started successfully\n");
    osDelay(5000);





//    printf("Stopping AP\n");
//
//    error_code = _wifi->stop();
//    MBED_ASSERT(error_code == NSAPI_ERROR_OK);
//
//    printf("AP Stopped successfully\n");
//    osDelay(5000);
}


static void stop_ap()
{
    WhdSoftAPInterface *_wifi;
    nsapi_error_t error_code;

    _wifi = WhdSoftAPInterface::get_default_instance();
    printf("Stopping AP\n");

    error_code = _wifi->stop();
    MBED_ASSERT(error_code == NSAPI_ERROR_OK);

    printf("AP Stopped successfully\n");
    osDelay(5000);
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
            printf("\nConnection error: %d[0x%x]\n", ret, ret);
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
    Thread thread;
    printf("IN main()\n");
    thread.start(app_main);

        start_ap("AP-AP", "apapapap");
        stop_ap();
        connect_to_ap("Cisco57680", "9954915140");

        osDelay(osWaitForever);
//        while(1);//will not execute unless above fails
}

