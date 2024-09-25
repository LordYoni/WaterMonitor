#ifndef ConductivitySensor_H
#define ConductivitySensor_H

#include "Sensor.h"

class ConductivitySensor : public Sensor
{
public:
    ConductivitySensor(const uint8_t& pin);
    ~ConductivitySensor() override;

    void printValue() override;
    void poll() override;
};

#endif