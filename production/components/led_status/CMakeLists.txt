idf_component_register(
    SRCS "led_status.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_driver_gpio
    # PRIV_REQUIRES ใช้ใน component นี้ที่เดียว
)