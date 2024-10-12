#ifndef Temperature_H
#define Temperature_H

#include "baseSensorClass.h"


class Temperature : public Sensor
{
public:
    Temperature(const MCP3008& mcp, const uint8_t& channel);
    ~Temperature() override;

    void poll() override;
    void printState() const override;

private:
    // Constants for Temperature calculations
    const float GDIFF   = 30.0f / 1.8f;
    const float VR0     = 0.223f;
    const float G0      = 2.0f;
    const float I       = 1.24f / 10000.0f;
};

#endif