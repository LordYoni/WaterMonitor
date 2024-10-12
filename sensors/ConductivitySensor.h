#ifndef ConductivitySensor_H
#define ConductivitySensor_H

#include "baseSensorClass.h"
#include "Temperature.h"


class Conductivity : public Sensor
{
private:
    const Temperature& m_temperature;

public:
    Conductivity(MCP3008 *const mcp, const uint8_t& channel, const Temperature& temp);
    ~Conductivity() override;

    void poll();
    void printState() const override;

private:
    // Constants for Conductivity calculations
    const float k = 1.0f;
    const float RES2 = 820.0f;
    const float ECREF = 200.0f;
};

#endif