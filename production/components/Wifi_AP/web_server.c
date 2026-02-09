#include "web_server.h"

#include "esp_http_server.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "WEB";

bool wifi_credential_received = false;
char wifi_ssid[32] = {0};
char wifi_pass[64] = {0};

static esp_err_t root_get_handler(httpd_req_t *req)
{
    const char *html =
        "<html><body>"
        "<h2>ESP32 WiFi Setup</h2>"
        "<form method='POST' action='/wifi'>"
        "SSID:<br><input name='ssid'><br>"
        "Password:<br><input name='pass' type='password'><br><br>"
        "<input type='submit'>"
        "</form>"
        "</body></html>";

    httpd_resp_send(req, html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t wifi_post_handler(httpd_req_t *req)
{
    char buf[128];
    int len = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (len <= 0) return ESP_FAIL;

    buf[len] = 0;

    sscanf(buf, "ssid=%31[^&]&pass=%63s", wifi_ssid, wifi_pass);
    wifi_credential_received = true;

    ESP_LOGI(TAG, "Received SSID=%s PASS=%s", wifi_ssid, wifi_pass);

    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}

void web_server_start(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(httpd_start(&server, &config));

    httpd_uri_t root = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_get_handler
    };

    httpd_uri_t wifi = {
        .uri = "/wifi",
        .method = HTTP_POST,
        .handler = wifi_post_handler
    };

    httpd_register_uri_handler(server, &root);
    httpd_register_uri_handler(server, &wifi);

    ESP_LOGI(TAG, "Web server started");
}