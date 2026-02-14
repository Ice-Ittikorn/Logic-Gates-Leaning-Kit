#include "wifi_sta.h"
#include <string.h>

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

volatile wifi_status_t WIFI_STATUS = WIFI_STATUS_DISCONNECTED;

static void wifi_event_handler(void *arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        WIFI_STATUS = WIFI_STATUS_CONNECTING;
        esp_wifi_connect();
    }

    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        wifi_event_sta_disconnected_t *disc =
            (wifi_event_sta_disconnected_t *)event_data;

        if (disc->reason == WIFI_REASON_AUTH_FAIL) {
            WIFI_STATUS = WIFI_STATUS_WRONG_PASSWORD;
        }
        else if (disc->reason == WIFI_REASON_NO_AP_FOUND) {
            WIFI_STATUS = WIFI_STATUS_NO_SSID;
        }
        else {
            WIFI_STATUS = WIFI_STATUS_DISCONNECTED;
        }
    }

    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        WIFI_STATUS = WIFI_STATUS_SUCCESS;
    }
}

void wifi_setup(const char *ssid, const char *password)
{
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                               wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
                               wifi_event_handler, NULL);

    esp_netif_create_default_wifi_sta();

    wifi_config_t wifi_config = {0};

    // copy ssid/password ลง buffer
    strncpy((char *)wifi_config.sta.ssid, ssid,
            sizeof(wifi_config.sta.ssid) - 1);
    strncpy((char *)wifi_config.sta.password, password,
            sizeof(wifi_config.sta.password) - 1);

    // แปลง + เป็น space (กรณีมาจาก web form)
    for (int i = 0; wifi_config.sta.ssid[i]; i++) {
        if (wifi_config.sta.ssid[i] == '+')
            wifi_config.sta.ssid[i] = ' ';
    }

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
}