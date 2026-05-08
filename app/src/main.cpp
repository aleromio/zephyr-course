#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "aleromio_ledsensor.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{

    static const struct  device* ledsensor_driver = DEVICE_DT_GET(DT_NODELABEL(ledsensor0));

    struct aleromio_data *data = (struct aleromio_data*)ledsensor_driver->data;

    while(1){
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    return 0;
}

