#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>



int main(void)
{

  //  static const struct  device* ledsensor_driver = DEVICE_DT_GET(DT_NODELABEL(ledsensor0));

    while(1){
        // sensor_channel_get(ledsensor_driver,SENSOR_CHAN_ALL, NULL);
        // k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
        // sensor_sample_fetch(ledsensor_driver);
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

    return 0;
}

