#pragma once
#include <stdbool.h>

extern char wifi_ssid[32];
extern char wifi_pass[64];
extern bool wifi_credential_received;

void web_server_start(void);