idf_component_register(
    SRCS
        wifi_ap.c
        web_server.c
    INCLUDE_DIRS .
    PRIV_REQUIRES
        esp_wifi
        esp_netif
        esp_event
        nvs_flash
        esp_http_server
        spiffs
)