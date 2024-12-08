// infrared_sensor.h

#ifndef INFRARED_SENSOR_H
#define INFRARED_SENSOR_H

#ifdef __cplusplus
extern "C" {
#endif

void infrared_sensor_init(void);
int infrared_sensor_read(void);

#ifdef __cplusplus
}
#endif

#endif // INFRARED_SENSOR_H