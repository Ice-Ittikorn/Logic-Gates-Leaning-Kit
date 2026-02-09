#pragma once
#include "esp_netif.h"

#define ESP_AP_SSID "ESP32_CONFIG"
#define ESP_AP_PASS "12345678"

void wifi_ap_start(void);

extern esp_ip4_addr_t wifi_ap_ip;