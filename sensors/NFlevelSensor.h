#ifndef NFlevelSensor_H
#define NFlevelSensor_H

#include "Sensor.h"

class NFlevelSensor : public Sensor
{
public:
    NFlevelSensor(const uint8_t& pin);
    ~NFlevelSensor() override;

    void printValue() override;
    void poll() override;
};

#endif