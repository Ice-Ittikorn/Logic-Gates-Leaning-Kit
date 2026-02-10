#pragma once
#include "esp_netif.h"

#define ESP_AP_SSID "ESP_0001"
#define ESP_AP_PASS "12345678"

void wifi_ap_start(void);

extern esp_ip4_addr_t wifi_ap_ip;          // IP ของ AP
extern esp_ip4_addr_t wifi_ap_client_ip;   // IP ของ client

extern char WIFI_CLIENT_IP[16];  //IP client ในรูป string