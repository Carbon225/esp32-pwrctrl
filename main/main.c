#include <app/nvs.h>
#include <app/fb.h>
#include <app/monitor.h>
#include <app/control.h>


void app_main(void)
{
    app_nvs_init();

    app_fb_init();
    app_monitor_init();
    app_control_init();

    app_fb_start();
    app_monitor_start();
    app_control_start();
}
