#include "web_server.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "esp_http_server.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "WEB";

char g_wifi_ssid[32] = {0};
char g_wifi_pass[64] = {0};

/* ========================================================= */
/* ส่งไฟล์จาก SPIFFS */
/* ========================================================= */
static esp_err_t send_file(httpd_req_t *req,
                           const char *path,
                           const char *type)
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

/* ========================================================= */
/* GET / */
/* ========================================================= */
static esp_err_t root_get(httpd_req_t *req)
{
    ESP_LOGI(TAG, "GET /");
    return send_file(req, "/spiffs/form.html", "text/html");
}

/* ========================================================= */
/* GET /style.css */
/* ========================================================= */
static esp_err_t css_get(httpd_req_t *req)
{
    return send_file(req, "/spiffs/style.css", "text/css");
}

/* ========================================================= */
/* GET /success.html */
/* ========================================================= */
static esp_err_t success_get(httpd_req_t *req)
{
    return send_file(req, "/spiffs/success.html", "text/html");
}

/* ========================================================= */
/* POST /Success  (รับ JSON รองรับภาษาไทย UTF-8) */
/* ========================================================= */
static esp_err_t success_post(httpd_req_t *req)
{
    char buf[256];

    int total_len = req->content_len;
    int received = 0;

    /* ป้องกัน overflow */
    if (total_len <= 0 || total_len >= sizeof(buf)) {
        ESP_LOGE(TAG, "Invalid content length");
        return ESP_FAIL;
    }

    /* อ่าน body ให้ครบ */
    while (received < total_len) {
        int len = httpd_req_recv(req,
                                 buf + received,
                                 total_len - received);
        if (len <= 0) {
            ESP_LOGE(TAG, "Failed to receive POST data");
            return ESP_FAIL;
        }
        received += len;
    }

    buf[received] = '\0';

    ESP_LOGI(TAG, "RAW JSON: %s", buf);

    /* เคลียร์ค่าก่อน */
    memset(g_wifi_ssid, 0, sizeof(g_wifi_ssid));
    memset(g_wifi_pass, 0, sizeof(g_wifi_pass));

    /* หา key */
    char *ssid_ptr = strstr(buf, "\"ssid\":\"");
    char *pass_ptr = strstr(buf, "\"pass\":\"");

    if (ssid_ptr) {
        ssid_ptr += 8;  // ข้าม "ssid":" 
        sscanf(ssid_ptr, "%31[^\"]", g_wifi_ssid);
    }

    if (pass_ptr) {
        pass_ptr += 8;  // ข้าม "pass":" 
        sscanf(pass_ptr, "%63[^\"]", g_wifi_pass);
    }

    ESP_LOGI(TAG, "FORM SSID = %s", g_wifi_ssid);
    ESP_LOGI(TAG, "FORM PASS = %s", g_wifi_pass);

    /* ส่ง OK กลับ (JS จะ redirect เอง) */
    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}

/* ========================================================= */
/* SPIFFS INIT */
/* ========================================================= */
static void spiffs_init(void)
{
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .format_if_mount_failed = true,
        .max_files = 5
    };

    esp_vfs_spiffs_register(&conf);
}

/* ========================================================= */
/* START WEB SERVER */
/* ========================================================= */
void web_server_start(void)
{
    spiffs_init();

    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_start(&server, &config);

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/",
        .method = HTTP_GET,
        .handler = root_get
    });

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/style.css",
        .method = HTTP_GET,
        .handler = css_get
    });

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/success.html",
        .method = HTTP_GET,
        .handler = success_get
    });

    httpd_register_uri_handler(server, &(httpd_uri_t){
        .uri = "/Success",
        .method = HTTP_POST,
        .handler = success_post
    });

    ESP_LOGI(TAG, "Web server started");
}