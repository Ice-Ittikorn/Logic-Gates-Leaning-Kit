#include "web_server.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include <stdio.h>
#include <string.h>

static const char *TAG = "WEB";

/* ตัวแปรจริง (มีที่เดียว) */
char g_wifi_ssid[WIFI_SSID_LEN] = {0};
char g_wifi_pass[WIFI_PASS_LEN] = {0};

/* อ่านไฟล์จาก SPIFFS */
static esp_err_t send_file(httpd_req_t *req, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        ESP_LOGE(TAG, "File not found: %s", path);
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    char buf[256];
    size_t read_bytes;
    while ((read_bytes = fread(buf, 1, sizeof(buf), f)) > 0) {
        httpd_resp_send_chunk(req, buf, read_bytes);
    }

    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* GET / */
static esp_err_t root_get_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "GET /");
    return send_file(req, "/spiffs/from.html");
}

/* POST / */
static esp_err_t root_post_handler(httpd_req_t *req)
{
    char buf[128] = {0};
    int len = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (len <= 0) return ESP_FAIL;

    sscanf(buf,
           "ssid=%31[^&]&pass=%63s",
           g_wifi_ssid,
           g_wifi_pass);

    ESP_LOGI(TAG, "RECEIVED SSID = %s", g_wifi_ssid);
    ESP_LOGI(TAG, "RECEIVED PASS = %s", g_wifi_pass);

    httpd_resp_set_status(req, "303 See Other");
    httpd_resp_set_hdr(req, "Location", "/Success");
    httpd_resp_send(req, NULL, 0);
    return ESP_OK;
}

/* GET /Success */
static esp_err_t success_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "GET /Success");
    return send_file(req, "/spiffs/success.html");
}

void web_server_start(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    httpd_start(&server, &config);

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/", .method = HTTP_GET, .handler = root_get_handler });

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/", .method = HTTP_POST, .handler = root_post_handler });

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/Success", .method = HTTP_GET, .handler = success_handler });

    ESP_LOGI(TAG, "Web server started");
}