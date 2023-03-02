#include <app/nvs.h>

#include <nvs.h>
#include <nvs_flash.h>


#define NS_CONFIG "config"

#define KEY_WIFI_SSID "wifi_ssid"
#define KEY_WIFI_PASSWORD "wifi_password"
#define KEY_FB_EMAIL "fb_email"
#define KEY_FB_PASSWORD "fb_password"


void app_nvs_init(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

void app_nvs_config_get(app_nvs_key_t key, void *value)
{
    size_t len;
    nvs_handle_t handle;
    ESP_ERROR_CHECK(nvs_open(NS_CONFIG, NVS_READONLY, &handle));
    
    switch (key)
    {
        case APP_NVS_KEY_WIFI_SSID:
            len = 32;
            ESP_ERROR_CHECK(nvs_get_str(handle, KEY_WIFI_SSID, value, &len));
            break;
        case APP_NVS_KEY_WIFI_PASSWORD:
            len = 64;
            ESP_ERROR_CHECK(nvs_get_str(handle, KEY_WIFI_PASSWORD, value, &len));
            break;
        case APP_NVS_KEY_FB_EMAIL:
            len = 64;
            ESP_ERROR_CHECK(nvs_get_str(handle, KEY_FB_EMAIL, value, &len));
            break;
        case APP_NVS_KEY_FB_PASSWORD:
            len = 64;
            ESP_ERROR_CHECK(nvs_get_str(handle, KEY_FB_PASSWORD, value, &len));
            break;
    }

    nvs_close(handle);
}

void app_nvs_config_put(app_nvs_key_t key, const void *value)
{
    nvs_handle_t handle;
    ESP_ERROR_CHECK(nvs_open(NS_CONFIG, NVS_READWRITE, &handle));
    
    switch (key)
    {
        case APP_NVS_KEY_WIFI_SSID:
            ESP_ERROR_CHECK(nvs_set_str(handle, KEY_WIFI_SSID, value));
            break;
        case APP_NVS_KEY_WIFI_PASSWORD:
            ESP_ERROR_CHECK(nvs_set_str(handle, KEY_WIFI_PASSWORD, value));
            break;
        case APP_NVS_KEY_FB_EMAIL:
            ESP_ERROR_CHECK(nvs_set_str(handle, KEY_FB_EMAIL, value));
            break;
        case APP_NVS_KEY_FB_PASSWORD:
            ESP_ERROR_CHECK(nvs_set_str(handle, KEY_FB_PASSWORD, value));
            break;
    }
    
    ESP_ERROR_CHECK(nvs_commit(handle));
    nvs_close(handle);
}
