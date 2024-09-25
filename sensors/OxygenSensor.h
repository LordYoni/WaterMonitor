#ifndef OxygenSensor_H
#define OxygenSensor_H

#include "Sensor.h"

class OxygenSensor : public Sensor
{
public:
    OxygenSensor(const uint8_t& pin);
    ~OxygenSensor() override;

    void printValue() override;
    void poll() override;
};

#endif