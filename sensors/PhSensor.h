#ifndef PhSensor_H
#define PhSensor_H

#include "baseSensorClass.h"


class phSensor : public Sensor
{
public:
    phSensor(const MCP3008& mcp, const uint8_t& channel);
    ~phSensor() override;

    void poll() override;
    void printState() const override;

private:
    // Constant for pH calculation
    const float OFFSET = -5.0f;
};

#endif