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
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"

static const char *TAG = "MAIN";

char lcd_buf[32]; //เก็บข้อความแสดงผลบนจอ LCD  

void setup(void)
{
    led_status_init();   
    Lcd_setup();        

    nvs_flash_init();                 // เตรียม NVS (ใช้เก็บ config WiFi และ calibration)
    esp_netif_init();                 // เริ่มต้น TCP/IP stack
    esp_event_loop_create_default();  // สร้าง default event loop สำหรับ WIFI/IP event

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT(); // โหลดค่า config มาตรฐานของ WiFi
    esp_wifi_init(&cfg);                                  // เริ่มต้น WiFi driver
}

void wifi_ap (void)
{
    wifi_ap_start();
    web_server_start();

    ESP_LOGI(TAG, "FORM SSID = %s", g_wifi_ssid);
    ESP_LOGI(TAG, "FORM PASS = %s", g_wifi_pass);

    sprintf(lcd_buf, "%-16s", "SSID : " ESP_AP_SSID); //จัดรูปแบบความยาว 16 ตัวอักษร
    Lcd_print(0, 0, lcd_buf);
    sprintf(lcd_buf, "%-16s", "Pass : " ESP_AP_PASS); 
    Lcd_print(0, 1, lcd_buf);

    while (strlen(g_wifi_ssid) == 0 || strlen(g_wifi_pass) == 0) {  // เช็คว่ามี g_wifi_ssid g_wifi_pass มายัง
        if (strcmp(WIFI_CLIENT_IP, "0.0.0.0") != 0) {
            Lcd_print(0, 0, " Please connect ");
            Lcd_print(0, 1, "IP : 192.168.4.1");
        } 
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    esp_wifi_stop();
}

void wifi_sta (void)
{
    wifi_setup(g_wifi_ssid,g_wifi_pass);
    
     while (1) {
        if (WIFI_STATUS == WIFI_STATUS_SUCCESS) {
            led_status_set(LED_GREEN);  
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}


void app_main(void)
{
    setup();
    wifi_ap();
    wifi_sta();
}


