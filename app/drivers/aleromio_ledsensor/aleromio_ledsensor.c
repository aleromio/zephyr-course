#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "include/aleromio_ledsensor.h"

#define DT_DRV_COMPAT aleromio_ledsensor

/* The devicetree node identifier for the "app_led(app-led)" alias. */
#define LED_NODE DT_ALIAS(led0)
#define LED_OFF 0
#define LED_ON 1


static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(ledsensor, LOG_LEVEL_INF);

struct aleromio_data data;


static int sensor_channel_get_aleromio(const struct device *dev, 
                        enum sensor_channel chan, 
                        struct sensor_value *val){

    LOG_INF("Sensor LED_OFF !!!");
    if (gpio_pin_set_dt(&led, LED_OFF) < 0) return 0;

    return 0;
}

static int sensor_sample_fetch_aleromio(const struct device *dev, 
                        enum sensor_channel chan){

    LOG_INF("Sensor LED_ON !!!");
    if (gpio_pin_set_dt(&led, LED_ON) < 0) return 0;

    return 0;
}

int aleromio_set_parametro(const struct device *dev, int parametro)
{
    struct aleromio_data *data = dev->data;
    data->parametro = parametro;
    return 0;
}


static DEVICE_API(sensor, api_aleromio) = {
    .channel_get = sensor_channel_get_aleromio,
    .sample_fetch = sensor_sample_fetch_aleromio,
};


static int led_sensor_init(const struct device *dev){
    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    LOG_INF("Led Sensor Initialized !");   
    if (gpio_pin_set_dt(&led, LED_OFF) < 0) return 0;
    return 0;
}


DEVICE_DT_INST_DEFINE(0, led_sensor_init, NULL,  &data, NULL, POST_KERNEL, 80, &api_aleromio);