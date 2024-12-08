// digital_microphone.h

#ifndef DIGITAL_MICROPHONE_H
#define DIGITAL_MICROPHONE_H

#include <stdint.h>

class DigitalMicrophone {
public:
    DigitalMicrophone(uint32_t pin);
    bool read();

private:
    uint32_t gpio_pin;
};

#endif // DIGITAL_MICROPHONE_H