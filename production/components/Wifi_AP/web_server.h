#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_SSID_LEN 32
#define WIFI_PASS_LEN 64

/* ค่าที่รับจาก HTML */
extern char g_wifi_ssid[WIFI_SSID_LEN];
extern char g_wifi_pass[WIFI_PASS_LEN];

void web_server_start(void);

#ifdef __cplusplus
}
#endif