#ifndef ALEROMIO_LEDSENSOR_H
#define ALEROMIO_LEDSENSOR_H

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h> 

#ifdef __cplusplus
extern "C" {
#endif

struct aleromio_data {
    int parametro;

};

// Changes device->data value
int aleromio_set_parametro(const struct device *dev, int parametro);

#ifdef __cplusplus
}
#endif

#endif // ALEROMIO_LEDSENSOR_H