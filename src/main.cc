#include "test_func.h"

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>

LOG_MODULE_REGISTER(main, CONFIG_USB_CDC_ACM_LOG_LEVEL);

static void login_init(void)
{
    if(!CONFIG_SHELL_CMD_ROOT[0])
    {
        shell_set_root_cmd("login");
    }
}

int main(void)
{
    int ret = 0;
    uint32_t dtr = 0;
    const struct device* shell_uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_shell_uart));
	const struct device *uart_dev = DEVICE_DT_GET_ONE(zephyr_cdc_acm_uart);

    LOG_INF("Hello World %s\n", CONFIG_BOARD);

    if(IS_ENABLED(CONFIG_USB_DEVICE_STACK))
    {
        ret = usb_enable(NULL);
        if(ret)
        {
            return 0;
        }
    }

    if(IS_ENABLED(CONFIG_SHELL_START_OBSUCURED))
    {
        login_init();
    }

    while(1)
    {
        printk("Hello World 2\n");
        LOG_INF("Hello World %s\n", CONFIG_BOARD);
        k_sleep(K_SECONDS(10));
    }

    return 0;
}
