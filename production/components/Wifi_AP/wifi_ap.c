#include "wifi_ap.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "WIFI_AP";

/* IP ของ ESP32 (AP) */
esp_ip4_addr_t wifi_ap_ip;

void wifi_ap_start(const char *ssid, const char *password)
{
    /* netif + event loop */
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();

    /* wifi init */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    /* ใช้ RAM (ไม่แตะ NVS เพิ่ม) */
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

    wifi_config_t wifi_config = {0};

    strncpy((char *)wifi_config.ap.ssid, ssid,
            sizeof(wifi_config.ap.ssid));
    strncpy((char *)wifi_config.ap.password, password,
            sizeof(wifi_config.ap.password));

    wifi_config.ap.ssid_len = strlen(ssid);
    wifi_config.ap.channel = 1;
    wifi_config.ap.max_connection = 4;
    wifi_config.ap.authmode = WIFI_AUTH_WPA2_PSK;

    if (strlen(password) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    /* ดึง IP */
    esp_netif_ip_info_t ip_info;
    ESP_ERROR_CHECK(esp_netif_get_ip_info(ap_netif, &ip_info));
    wifi_ap_ip = ip_info.ip;

    ESP_LOGI(TAG, "AP started");
    ESP_LOGI(TAG, "SSID: %s", ssid);
    ESP_LOGI(TAG, "IP: " IPSTR, IP2STR(&wifi_ap_ip));
}