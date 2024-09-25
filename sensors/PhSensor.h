#ifndef PhSensor_H
#define PhSensor_H

#include "Sensor.h"

class PhSensor : public Sensor
{
public:
    PhSensor(const uint8_t& pin);
    ~PhSensor() override;

    void printValue() override;
    void poll() override;
};

#endif