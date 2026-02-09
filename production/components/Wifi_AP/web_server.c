#include "web_server.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "WEB";

char g_wifi_ssid[32] = {0};
char g_wifi_pass[64] = {0};

static esp_err_t send_file(httpd_req_t *req, const char *path, const char *type)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        ESP_LOGE(TAG, "File not found: %s", path);
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, type);
    char buf[256];
    size_t read;
    while ((read = fread(buf, 1, sizeof(buf), f)) > 0) {
        httpd_resp_send_chunk(req, buf, read);
    }
    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* GET / */
static esp_err_t root_get(httpd_req_t *req)
{
    ESP_LOGI(TAG, "GET /");
    return send_file(req, "/spiffs/from.html", "text/html");
}

/* GET /style.css */
static esp_err_t css_get(httpd_req_t *req)
{
    return send_file(req, "/spiffs/style.css", "text/css");
}

/* POST /Success */
static esp_err_t success_post(httpd_req_t *req)
{
    char buf[128];
    int len = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (len <= 0) return ESP_FAIL;
    buf[len] = 0;

    sscanf(buf, "ssid=%31[^&]&pass=%63s", g_wifi_ssid, g_wifi_pass);

    ESP_LOGI(TAG, "FORM SSID = %s", g_wifi_ssid);
    ESP_LOGI(TAG, "FORM PASS = %s", g_wifi_pass);

    return send_file(req, "/spiffs/success.html", "text/html");
}

static void spiffs_init(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .format_if_mount_failed = true,
        .max_files = 5
    };
    esp_vfs_spiffs_register(&conf);
}

void web_server_start(void)
{
    spiffs_init();

    httpd_handle_t server;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_start(&server, &config);

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/", .method = HTTP_GET, .handler = root_get});

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/style.css", .method = HTTP_GET, .handler = css_get});

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/Success", .method = HTTP_POST, .handler = success_post});

    ESP_LOGI(TAG, "Web server started");
}