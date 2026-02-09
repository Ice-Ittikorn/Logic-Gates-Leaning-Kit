#include "led_status.h"
#include "driver/gpio.h"

/* GPIO DEFINE */
#define LED_R GPIO_NUM_19
#define LED_G GPIO_NUM_18
#define LED_B GPIO_NUM_23

static void led_off_all(void)
{
    gpio_set_level(LED_R, 0);
    gpio_set_level(LED_G, 0);
    gpio_set_level(LED_B, 0);
}

void led_status_init(void)
{
    gpio_reset_pin(LED_R);
    gpio_reset_pin(LED_G);
    gpio_reset_pin(LED_B);

    gpio_set_direction(LED_R, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_G, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_B, GPIO_MODE_OUTPUT);

    led_off_all();
}

void led_status_set(led_status status)
{
    led_off_all();  

    switch (status) {
        case LED_RED:
            gpio_set_level(LED_R, 1);
            break;

        case LED_GREEN:
            gpio_set_level(LED_G, 1);
            break;

        case LED_BLUE:
            gpio_set_level(LED_B, 1);
            break;

        case LED_OFF:
            break;
    }
}