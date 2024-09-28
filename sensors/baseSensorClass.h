#ifndef Sensor_H
#define Sensor_H

#include <MCP3008.h>


class Sensor
{
protected:
    const uint8_t m_channel;
    float m_voltage;
    float m_value;

    Sensor(const uint8_t& channel);

    void pollVoltage(MCP3008 *const mcp);

public:
    virtual ~Sensor();

    virtual void poll(MCP3008 *const mcp) = 0;
    virtual void printState() const = 0;
    
    float getValue() const;

protected:
    const uint16_t ADC_RESOLUTION = 1024;
    const float VREF = 5.0f; // Reference voltage for MCP3008
};

#endif