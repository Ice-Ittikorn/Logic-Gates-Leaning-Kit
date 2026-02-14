#include "wifi_ap.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include <string.h>

static const char *TAG = "WIFI_AP";

esp_ip4_addr_t wifi_ap_ip;
esp_ip4_addr_t wifi_ap_client_ip;

char WIFI_CLIENT_IP[16] = "0.0.0.0";

/* ===== event handler ===== */
static void wifi_ap_event_handler(void *arg,esp_event_base_t event_base,int32_t event_id,void *event_data)
{
    if (event_base == IP_EVENT &&
        event_id == IP_EVENT_AP_STAIPASSIGNED) {

        ip_event_ap_staipassigned_t *event =
            (ip_event_ap_staipassigned_t *)event_data;

        wifi_ap_client_ip = event->ip;

        /* แปลง IP → string */
        sprintf(WIFI_CLIENT_IP,
                "%d.%d.%d.%d",
                IP2STR(&wifi_ap_client_ip));

        ESP_LOGI(TAG, "Client IP = %s",
                 WIFI_CLIENT_IP);
    }

    if (event_base == WIFI_EVENT &&
        event_id == WIFI_EVENT_AP_STADISCONNECTED) {

        strcpy(WIFI_CLIENT_IP, "0.0.0.0");
        wifi_ap_client_ip.addr = 0;
        ESP_LOGI(TAG, "Client disconnected");
    }
}

void wifi_ap_start(void)
{
    /* register events */
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                               &wifi_ap_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_AP_STAIPASSIGNED,
                               &wifi_ap_event_handler, NULL);

    /* create AP netif */
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();

    /* AP config */
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = ESP_AP_SSID,
            .ssid_len = strlen(ESP_AP_SSID),
            .password = ESP_AP_PASS,
            .channel = 1,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        }
    };

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    /* get AP IP */
    esp_netif_ip_info_t ip;
    esp_netif_get_ip_info(ap_netif, &ip);
    wifi_ap_ip = ip.ip;

    ESP_LOGI(TAG,
        "AP started SSID:%s PASS:%s IP:" IPSTR,
        ESP_AP_SSID, ESP_AP_PASS,
        IP2STR(&wifi_ap_ip));
}