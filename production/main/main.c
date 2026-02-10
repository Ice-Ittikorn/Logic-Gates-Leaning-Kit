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

static const char *TAG = "MAIN";

char lcd_buf[32]; //เก็บข้อความแสดงผลบนจอ LCD  

void setup (void)
{
    led_status_init();
    Lcd_setup();
}

void wifi_ap (void)
{
    wifi_ap_start();
    web_server_start();

    ESP_LOGI(TAG, "ESP32 AP SSID = %s", ESP_AP_SSID);
    ESP_LOGI(TAG, "ESP32 AP PASS = %s", ESP_AP_PASS);

    ESP_LOGI(TAG, "FORM SSID = %s", g_wifi_ssid);
    ESP_LOGI(TAG, "FORM PASS = %s", g_wifi_pass);

    sprintf(lcd_buf, "SSID:%s", ESP_AP_SSID);
    Lcd_print(0, 0, lcd_buf);
    sprintf(lcd_buf, "Pass:%s", ESP_AP_PASS);
    Lcd_print(0, 1, lcd_buf);
}

void wifi_sta (void)
{
    while (1){
        Lcd_print(0, 0, "CONNECTING .    ");
        Lcd_print(0, 1, "  Please  wait  ");
        led_status_set(LED_BLUE);
        vTaskDelay(pdMS_TO_TICKS(500));
        Lcd_print(0, 0, "CONNECTING . .  ");
        led_status_set(LED_OFF);
        vTaskDelay(pdMS_TO_TICKS(500));
        Lcd_print(0, 0, "CONNECTING . . .");
        led_status_set(LED_BLUE);
        vTaskDelay(pdMS_TO_TICKS(500));
        Lcd_print(0, 0, "CONNECTING      ");
        led_status_set(LED_OFF);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    // wifi_setup("ssid","password");

    // WIFI_STATUS_SUCCESS
    // WIFI_STATUS_WRONG_PASSWORD
    // WIFI_STATUS_NO_SSID
}

void app_main(void)
{
    setup();
    wifi_sta();
    //wifi_ap();
}