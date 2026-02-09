#pragma once

typedef enum {
    WIFI_STATUS_CONNECTING = 0,
    WIFI_STATUS_SUCCESS,
    WIFI_STATUS_WRONG_PASSWORD,
    WIFI_STATUS_NO_SSID,
    WIFI_STATUS_DISCONNECTED,
    WIFI_STATUS_TIMEOUT
} wifi_status_t;

/* ตัวแปรสถานะกลาง */
extern volatile wifi_status_t WIFI_STATUS;

/* เรียกแค่ setup */
void wifi_setup(const char *ssid, const char *password);