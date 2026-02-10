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

    ESP_LOGI(TAG, "FORM SSID = %s", g_wifi_ssid);
    ESP_LOGI(TAG, "FORM PASS = %s", g_wifi_pass);

    sprintf(lcd_buf, "%-16s", "SSID : " ESP_AP_SSID); //จัดรูปแบบความยาว 16 ตัวอักษร
    Lcd_print(0, 0, lcd_buf);
    sprintf(lcd_buf, "%-16s", "Pass : " ESP_AP_PASS); 
    Lcd_print(0, 1, lcd_buf);

    if (strcmp(WIFI_CLIENT_IP, "0.0.0.0") != 0) {
        //เช็คส่าถูกมั้ยดดยการใส่ log
        Lcd_print(0, 0, " Please connect ");
        Lcd_print(0, 1, "IP : 192.168.4.1");
    } // เพิ่ม loop เช็คการเชื่อมต่อของ client เบรกตอนได้  ESP_AP_SSID และ ESP_AP_PASS 
}

void wifi_sta (void)
{
    wifi_setup("ssid","password");
    // WIFI_STATUS_SUCCESS
    // WIFI_STATUS_WRONG_PASSWORD
    // WIFI_STATUS_NO_SSID
}


void app_main(void)
{
    setup();
    wifi_ap();
}


void connect_text( char *x)
{
    const char *connec_txt[] = {"CONNECTING .    ","CONNECTING . .  ","CONNECTING . . .","CONNECTING      "};
    while (1) {
        for (int i = 0; i < 4; i++) {
            Lcd_print(0, 0, connec_txt[i]);
            Lcd_print(0, 1, "  Please  wait  ");

            led_status_set(i % 2 ? LED_OFF : LED_BLUE);
            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }
}