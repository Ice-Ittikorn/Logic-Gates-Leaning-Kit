idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES logic_gate lcd_i2c led_status Wifi_STA Wifi_AP
)
