#include "web_server.h"

#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include <string.h>

static const char *TAG = "WEB";

char wifi_ssid[32];
char wifi_pass[64];
bool wifi_credential_received = false;

/* ---------- SPIFFS ---------- */
static void spiffs_init(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };

    ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));
    ESP_LOGI(TAG, "SPIFFS mounted");
}

/* ---------- File sender ---------- */
static esp_err_t send_file(httpd_req_t *req, const char *path, const char *type)
{
    FILE *f = fopen(path, "r");
    if (!f) {
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, type);

    char buf[256];
    size_t len;
    while ((len = fread(buf, 1, sizeof(buf), f)) > 0) {
        httpd_resp_send_chunk(req, buf, len);
    }

    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* ---------- Handlers ---------- */
static esp_err_t index_get(httpd_req_t *req)
{
    return send_file(req, "/spiffs/index.html", "text/html");
}

static esp_err_t css_get(httpd_req_t *req)
{
    return send_file(req, "/spiffs/style.css", "text/css");
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

/* ---------- Start server ---------- */
void web_server_start(void)
{
    spiffs_init();

    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(httpd_start(&server, &cfg));

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri="/", .method=HTTP_GET, .handler=index_get});

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri="/style.css", .method=HTTP_GET, .handler=css_get});

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri="/wifi", .method=HTTP_POST, .handler=wifi_post});

    ESP_LOGI(TAG, "Web server started");
}