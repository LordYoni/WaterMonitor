#ifndef PhSensor_H
#define PhSensor_H

#include "baseSensorClass.h"


class PhSensor : public Sensor
{
public:
    PhSensor(MCP3008 *const, const uint8_t& channel);
    ~PhSensor() override;

    void poll() override;
    void printState() const override;

private:
    // Constant for pH calculation
    const float OFFSET = -5.0f;
};

#endif