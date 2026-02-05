idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES wifi PRIV_REQUIRES esp_driver_gpio logic_gate
)