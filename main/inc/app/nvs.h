#ifndef _PWRCTRL_APP_NVS_H_
#define _PWRCTRL_APP_NVS_H_


typedef enum
{
    APP_NVS_KEY_WIFI_SSID,
    APP_NVS_KEY_WIFI_PASSWORD,
    APP_NVS_KEY_FB_EMAIL,
    APP_NVS_KEY_FB_PASSWORD,
} app_nvs_key_t;


void app_nvs_init(void);

void app_nvs_config_get(app_nvs_key_t key, void *value);

void app_nvs_config_put(app_nvs_key_t key, const void *value);


#endif
