#ifndef LED_STATUS_H
#define LED_STATUS_H

typedef enum {
    LED_OFF,
    LED_RED,
    LED_GREEN,
    LED_BLUE
} led_status;

void led_status_init(void);
void led_status_set(led_status status);

#endif