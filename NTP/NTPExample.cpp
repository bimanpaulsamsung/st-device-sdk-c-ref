#include "mbed.h"
#include "cmsis_os2.h"
#include "lwip/apps/sntp.h"

#define NTP_DEFULT_NIST_SERVER_ADDRESS "2.pool.ntp.org"
#define NTP_DEFULT_NIST_SERVER_PORT 123
#define IOT_INFO(fmt, args...) printf("%s(%d) > " fmt "\n", __FUNCTION__, __LINE__, ##args)

WiFiInterface *wifi;

static void _initialize_sntp(void)
{
	IOT_INFO("Initializing SNTP");
	sntp_setoperatingmode(SNTP_OPMODE_POLL);
	sntp_setservername(0, "pool.ntp.org");
	sntp_setservername(1, "1.kr.pool.ntp.org");
	sntp_setservername(2, "1.asia.pool.ntp.org");
	sntp_setservername(3, "us.pool.ntp.org");
	sntp_setservername(4, "1.cn.pool.ntp.org");
	sntp_setservername(5, "1.hk.pool.ntp.org");
	sntp_setservername(6, "europe.pool.ntp.org");
	sntp_setservername(7, "time1.google.com");

	sntp_init();
}

static void _obtain_time(void) {
	time_t now = 0;
	struct tm timeinfo = { 0 };
	int retry = 0;
	const int retry_count = 10;

	time(&now);
	localtime_r(&now, &timeinfo);
	IOT_INFO("DATE: (%02d-%02d-%04d %02d:%02d:%02d)", timeinfo.tm_mday,
			timeinfo.tm_mon+1, timeinfo.tm_year + 1900,
			timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

	_initialize_sntp();



	while (timeinfo.tm_year < (2016 - 1900) && ++retry < retry_count) {
		IOT_INFO("Waiting for system time to be set... (%d/%d)", retry, retry_count);
		osDelay(2000);
		time(&now);
		localtime_r(&now, &timeinfo);
		IOT_INFO("DATE: (%02d-%02d-%04d %02d:%02d:%02d)", timeinfo.tm_mday,
				timeinfo.tm_mon+1, timeinfo.tm_year + 1900,
				timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
	}

	if (retry < 10) {
		IOT_INFO("[WIFI] system time updated by %ll", now);
	}
}







static int connect_to_ap(char *wifi_ssid, char *wifi_password,
                nsapi_security_t security = NSAPI_SECURITY_WPA_WPA2)
{

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


int main() {
	connect_to_ap("Cisco57680", "9954915140");

    printf("NTP Client example (using Ethernet)\r\n");

    printf("Client IP Address is %s\r\n", wifi->get_ip_address());

    _obtain_time();


}
