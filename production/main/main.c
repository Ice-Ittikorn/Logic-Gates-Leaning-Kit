#include "logic_gate.h"
#include "esp_log.h"

static const char *TAG = "PROD";

void app_main(void)
{
    int a = 1;
    int b = 0;

    ESP_LOGI(TAG, "AND = %d", logic_and(a, b));
    ESP_LOGI(TAG, "OR  = %d", logic_or(a, b));
    ESP_LOGI(TAG, "NOT = %d", logic_not(a));
}