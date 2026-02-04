#include "logic_gate.h"
#include "wifi.h"

void app_main(void)
{
    const char *ssid = "test";
    const char *pass = "12345678";

    wifi_setting(ssid, pass);
}