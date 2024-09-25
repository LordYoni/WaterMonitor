#ifndef TurbiditySensor_H
#define TurbiditySensor_H

#include "Sensor.h"

class TurbiditySensor : public Sensor
{
public:
    TurbiditySensor(const uint8_t& pin);
    ~TurbiditySensor() override;

    void printValue() override;
    void poll() override;
};

#endif