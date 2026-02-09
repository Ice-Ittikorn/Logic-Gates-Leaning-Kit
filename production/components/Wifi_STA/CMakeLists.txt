idf_component_register(
    SRCS "wifi_STA.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_event esp_netif
)