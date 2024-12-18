#ifndef INFRARED_SENSOR_H
#define INFRARED_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void infrared_sensor_init(uint32_t gpio_pin);
int infrared_sensor_read(uint32_t gpio_pin);

#ifdef __cplusplus
}
#endif

#endif // INFRARED_SENSOR_H