#ifndef PhSensor_H
#define PhSensor_H

#include "baseSensorClass.h"


class phSensor : public Sensor
{
public:
    phSensor(const uint8_t& channel);
    ~phSensor() override;

    void poll(MCP3008 *const mcp) override;
    void printState() const override;

private:
    // Constant for pH calculation
    const float OFFSET = -5.0f;
};

#endif