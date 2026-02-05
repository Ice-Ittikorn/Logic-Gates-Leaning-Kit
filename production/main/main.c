#include "logic_gate.h"
#include "wifi.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

    #define LED_R GPIO_NUM_25 
    #define LED_G GPIO_NUM_26    
    #define LED_B GPIO_NUM_27

void start_up(void)
{
    gpio_set_level(LED_R, 0); 
    gpio_set_level(LED_G, 0); 
    gpio_set_level(LED_B, 0);

}

void app_main(void)
{
    const char *ssid = "test";
    const char *pass = "12345678";

    wifi_setting(ssid, pass);
    
}

