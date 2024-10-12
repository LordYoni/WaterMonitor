#ifndef ConductivitySensor_H
#define ConductivitySensor_H

#include "baseSensorClass.h"
#include "Temperature.h"


class Conductivity : public Sensor
{
private:
    void poll() override;

public:
    Conductivity(const MCP3008& mcp, const uint8_t& channel);
    ~Conductivity() override;

    void poll(const Temperature& temp);
    void printState() const override;

private:
    // Constants for Conductivity calculations
    const float k = 1.0f;
    const float RES2 = 820.0f;
    const float ECREF = 200.0f;
};

#endif