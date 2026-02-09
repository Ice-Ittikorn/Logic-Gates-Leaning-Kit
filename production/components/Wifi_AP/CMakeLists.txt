idf_component_register(
    SRCS
        wifi_ap.c
        web_server.c
        spiffs_init.c     # 👈 เพิ่ม
    INCLUDE_DIRS "."
    REQUIRES
        esp_wifi
        esp_netif
        esp_http_server
        spiffs
        nvs_flash
)