#include <app/monitor.h>
#include <app/config.h>

#include <driver/uart.h>


#define BAUD_RATE (115200)
#define RX_BUF_SIZE (1024)
#define PARSE_BUF_SIZE (64)


static void rx_task(void *pv)
{
    char buf[PARSE_BUF_SIZE];
    int pos = 0;
    for (;;)
    {
        int len = uart_read_bytes(APP_CONFIG_UART_NUM, buf + pos, 1, portMAX_DELAY);
        if (len > 0)
        {
            pos += len;
            if (pos >= PARSE_BUF_SIZE || buf[pos - 1] == '\n')
            {
                pos = 0;
            }
        }
    }
}

void app_monitor_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    ESP_ERROR_CHECK(uart_param_config(APP_CONFIG_UART_NUM, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(
        APP_CONFIG_UART_NUM,
        UART_PIN_NO_CHANGE,
        APP_CONFIG_UART_RX,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE));

    ESP_ERROR_CHECK(uart_driver_install(
        APP_CONFIG_UART_NUM,
        RX_BUF_SIZE,
        0,
        0,
        NULL,
        0));
}

void app_monitor_start(void)
{
    xTaskCreate(rx_task, "uart_rx_task", 2048, NULL, 10, NULL);
}
