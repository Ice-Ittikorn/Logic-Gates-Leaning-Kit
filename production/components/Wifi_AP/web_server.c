#include "web_server.h"

#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "string.h"

static const char *TAG = "WEB";

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

    size_t total = 0, used = 0;
    esp_spiffs_info(NULL, &total, &used);

    ESP_LOGI(TAG, "SPIFFS: used %d / %d", used, total);
}

/* ---------- GET / ---------- */
static esp_err_t index_handler(httpd_req_t *req)
{
    FILE *f = fopen("/spiffs/index.html", "r");
    if (!f) {
        ESP_LOGE(TAG, "File not found: /spiffs/index.html");
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    char buf[256];
    size_t len;
    while ((len = fread(buf, 1, sizeof(buf), f)) > 0) {
        httpd_resp_send_chunk(req, buf, len);
    }
    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* ---------- GET /style.css ---------- */
static esp_err_t css_handler(httpd_req_t *req)
{
    FILE *f = fopen("/spiffs/style.css", "r");
    if (!f) {
        httpd_resp_send_404(req);
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, "text/css");

    char buf[256];
    size_t len;
    while ((len = fread(buf, 1, sizeof(buf), f)) > 0) {
        httpd_resp_send_chunk(req, buf, len);
    }
    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

/* ---------- POST /wifi ---------- */
static esp_err_t wifi_post_handler(httpd_req_t *req)
{
    char buf[128];
    int len = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (len <= 0) return ESP_FAIL;
    buf[len] = 0;

    ESP_LOGI(TAG, "POST /wifi: %s", buf);

    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}

/* ---------- START SERVER ---------- */
void web_server_start(void)
{
    spiffs_init();

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    ESP_ERROR_CHECK(httpd_start(&server, &config));

    httpd_uri_t uri_index = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = index_handler
    };

    httpd_uri_t uri_css = {
        .uri = "/style.css",
        .method = HTTP_GET,
        .handler = css_handler
    };

    httpd_uri_t uri_wifi = {
        .uri = "/wifi",
        .method = HTTP_POST,
        .handler = wifi_post_handler
    };

    httpd_register_uri_handler(server, &uri_index);
    httpd_register_uri_handler(server, &uri_css);
    httpd_register_uri_handler(server, &uri_wifi);

    ESP_LOGI(TAG, "Web server started");
}