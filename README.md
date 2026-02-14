local setup
```
source ~/store/Logic/v5.5.2/esp-idf/export.sh
```

Build
```
idf.py build
idf.py flash
idf.py spiffsgen
idf.py flash monitor
```


ESP_LOGI(TAG, "ESP32 AP SSID = %s", ESP_AP_SSID);
ESP_LOGI(TAG, "ESP32 AP PASS = %s", ESP_AP_PASS);

ESP_LOGI(TAG, "FORM Client = %s", WIFI_AP_CLIENT_IP);

ESP_LOGI(TAG, "FORM SSID = %s", g_wifi_ssid);
ESP_LOGI(TAG, "FORM PASS = %s", g_wifi_pass);


    

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