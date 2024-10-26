#ifndef Temperature_H
#define Temperature_H

#include "baseSensorClass.h"


class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor(MCP3008 *const mcp, const uint8_t &channel);
    ~TemperatureSensor() override;

    void poll() override;
    void printState() const override;

private:
    //Constants for temperature calculations
    static constexpr float GDIFF   = 30.0f / 1.8f;
    static constexpr float VR0     = 0.223f;
    static constexpr float G0      = 2.0f;
    static constexpr float I       = 1.24f / 10000.0f;
};

#endif