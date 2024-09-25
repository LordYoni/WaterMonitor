#ifndef PressureSensor_H
#define PressureSensor_H

#include "Sensor.h"

class PressureSensor : public Sensor
{
public:
    PressureSensor(const uint8_t& pin);
    ~PressureSensor() override;

    void printValue() override;
    void poll() override;
};

#endif