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
        sensor_channel_get(ledsensor_driver,SENSOR_CHAN_ALL, NULL);
        aleromio_set_parametro(ledsensor_driver, 1);
        LOG_INF("Driver Data Parameter: %d\n", data->parametro);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        sensor_sample_fetch(ledsensor_driver);
        aleromio_set_parametro(ledsensor_driver, 0);
        LOG_INF("Driver Data Parameter: %d\n", data->parametro);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    return 0;
}

