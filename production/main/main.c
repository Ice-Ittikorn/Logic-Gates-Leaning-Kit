#include <stdio.h>
#include "logic_gate.h"
#include "esp_log.h"
#include "lcd_i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_status.h"
#include "wifi_sta.h"
#include "wifi_ap.h"
#include "web_server.h"
#include "nvs_flash.h"

void setup (void)
{
    led_status_init();
    Lcd_setup();
}



void wifi_sta (void)
{
    wifi_setup("ssid", "password");

    //WIFI_STATUS = WIFI_STATUS_SUCCESS เชื่อมต่อสำเร็จ
    //WIFI_STATUS = WIFI_STATUS_WRONG_PASSWORD  รหัสผ่านผิด
    //WIFI_STATUS = WIFI_STATUS_NO_SSID ไม่พบ SSID
}

#include "esp_log.h"
#include "nvs_flash.h"
#include "wifi_ap.h"
#include "web_server.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    /* NVS */
    ESP_ERROR_CHECK(nvs_flash_init());

    /* WiFi AP */
    wifi_ap_start();

    /* 🔥 SPIFFS MUST BE HERE */
    spiffs_init();

    /* Web Server */
    web_server_start();

    ESP_LOGI(TAG, "System ready");
}

void tt(void)
{
    setup();
    Lcd_print(0, 0, "Hello LogicGate!");
    Lcd_print(0, 1, " Welcome to ELK ");
    
    while (1) {
        led_status_set(LED_RED);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_status_set(LED_GREEN);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_status_set(LED_BLUE);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_status_set(LED_OFF);
    }
}