#include <app/ota.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <esp_http_client.h>
#include <esp_crt_bundle.h>
#include <esp_app_desc.h>


#define URL_LATEST_VERSION "https://github.com/Carbon225/esp32-pwrctrl/releases/latest/download/firmware.version"
#define URL_FIRMWARE "https://github.com/Carbon225/esp32-pwrctrl/releases/latest/download/firmware.bin"
#define MAX_HTTP_OUTPUT_BUFFER 2048
#define OTA_PERIOD (1000 * 60 / portTICK_PERIOD_MS)


static const char TAG[] = "ota";


static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
        case HTTP_EVENT_ON_DATA:
            if (!esp_http_client_is_chunked_response(evt->client))
            {
                memcpy(evt->user_data, evt->data, evt->data_len);
            }
            break;
        default:
            break;
    }
    return ESP_OK;
}

static esp_err_t fetch_latest_version(char *version)
{
    esp_err_t err;
    char local_response_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0};

    esp_http_client_config_t config = {
        .url = URL_LATEST_VERSION,
        .event_handler = http_event_handler,
        .user_data = local_response_buffer,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };
    
    esp_http_client_handle_t client = esp_http_client_init(&config);

    err = esp_http_client_perform(client);
    if (err == ESP_OK)
    {
        char *end = local_response_buffer + strlen(local_response_buffer) - 1;
        while (end > local_response_buffer && isspace((unsigned char) *end)) end--;
        *(end + 1) = '\0';
        strcpy(version, local_response_buffer);
    }
    else
    {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);

    return err;
}

bool should_update(void)
{
    char latest_version[64] = {0};

    ESP_LOGI(TAG, "Fetching latest version");
    if (fetch_latest_version(latest_version) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to fetch latest version");
        return false;
    }
    ESP_LOGI(TAG, "Latest version: %s", latest_version);

    const esp_app_desc_t *app_desc = esp_app_get_description();
    ESP_LOGI(TAG, "Current version: %s", app_desc->version);

    if (strcmp(latest_version, app_desc->version) == 0)
    {
        ESP_LOGI(TAG, "No update available");
        return false;
    }
    else
    {
        ESP_LOGI(TAG, "Update available");
        return true;
    }
}

void ota_task(void *pv)
{
    for (;;)
    {
        ESP_LOGI(TAG, "Checking for updates");
        if (should_update())
        {
            ESP_LOGI(TAG, "Starting OTA");
            esp_https_ota_config_t config = {
                .url = URL_FIRMWARE,
                .crt_bundle_attach = esp_crt_bundle_attach,
            };
            esp_err_t err = esp_https_ota(&config);
            if (err == ESP_OK)
            {
                ESP_LOGI(TAG, "OTA successful, restarting");
                esp_restart();
            }
            else
            {
                ESP_LOGE(TAG, "OTA failed: %s", esp_err_to_name(err));
            }
        }
        vTaskDelay(OTA_PERIOD);
    }
}

void app_ota_start(void)
{
    xTaskCreate(&ota_task, "ota_task", 8192, NULL, 5, NULL);
}