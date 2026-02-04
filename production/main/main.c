#include "logic_gate.h"
#include "esp_log.h"
#include "wifi.h"

void app_main(void)
{
    ESP_LOGI("main", "Starting application");

    wifi_init_sta();

    ESP_LOGI("main", "WiFi connected, app ready");
}