#include "web_server.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "WEB";

char wifi_ssid[32];
char wifi_pass[64];
bool wifi_credential_received = false;

/* HTML */
static const char *html_page =
"<!DOCTYPE html>"
"<html><head>"
"<link rel='stylesheet' href='/style.css'>"
"</head><body>"
"<h2>ESP32 WiFi Setup</h2>"
"<form method='POST' action='/wifi'>"
"SSID:<br><input name='ssid'><br>"
"Password:<br><input name='pass' type='password'><br><br>"
"<button type='submit'>Save</button>"
"</form>"
"</body></html>";

/* CSS */
static const char *css_page =
"body{font-family:sans-serif;background:#111;color:#0f0;text-align:center;}"
"input{padding:6px;margin:4px;}";

static esp_err_t root_get(httpd_req_t *req)
{
    httpd_resp_send(req, html_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t css_get(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/css");
    httpd_resp_send(req, css_page, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t wifi_post(httpd_req_t *req)
{
    char buf[128];
    int len = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (len <= 0) return ESP_FAIL;

    buf[len] = 0;

    char *s = strstr(buf, "ssid=");
    char *p = strstr(buf, "pass=");

    if (s && p) {
        sscanf(s, "ssid=%31[^&]", wifi_ssid);
        sscanf(p, "pass=%63s", wifi_pass);
        wifi_credential_received = true;

        ESP_LOGI(TAG, "SSID=%s PASS=%s", wifi_ssid, wifi_pass);
    }

    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}

void web_server_start(void)
{
    httpd_handle_t server = NULL;
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();

    ESP_ERROR_CHECK(httpd_start(&server, &cfg));

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri="/", .method=HTTP_GET, .handler=root_get});

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri="/style.css", .method=HTTP_GET, .handler=css_get});

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri="/wifi", .method=HTTP_POST, .handler=wifi_post});

    ESP_LOGI(TAG, "Web server started");
}