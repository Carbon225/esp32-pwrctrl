#include <app/nvs.h>
#include <app/fb.h>
#include <app/monitor.h>
#include <app/control.h>
#include <app/ota.h>
#include <app/wifi.h>


void app_main(void)
{
    app_nvs_init();
    app_wifi_init();

    app_fb_init();
    app_monitor_init();
    app_control_init();

    app_wifi_start();
    app_fb_start();
    app_monitor_start();
    app_control_start();
    app_ota_start();
}
