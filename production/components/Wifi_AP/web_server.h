#pragma once
#include "esp_http_server.h"

extern char g_wifi_ssid[32];
extern char g_wifi_pass[64];

void web_server_start(void);