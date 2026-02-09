#pragma once

#include "esp_netif.h"
#include "esp_netif_ip_addr.h"

extern esp_ip4_addr_t wifi_ap_ip;

void wifi_ap_start(const char *ssid, const char *password);