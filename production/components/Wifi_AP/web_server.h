#pragma once
#include <stdbool.h>

extern bool wifi_credential_received;
extern char wifi_ssid[32];
extern char wifi_pass[64];

void web_server_start(void);