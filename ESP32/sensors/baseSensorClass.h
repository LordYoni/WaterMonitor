#ifndef Sensor_H
#define Sensor_H

#include <MCP3008.h>


class Sensor
{
private:
    //Cannot be declared as a const reference because the
    //readADC() method is not declared as const even though
    //it doesn't modify any of its class members.
    MCP3008 *const m_mcp;

    const uint8_t m_channel;

protected:
    float m_voltage;
    float m_value;

    Sensor(MCP3008 *const mcp, const uint8_t& channel);

    void pollVoltage();

public:
    virtual ~Sensor();

    virtual void poll() = 0;
    virtual void printState() const = 0;
    
    float getValue() const;

protected:
    const uint16_t ADC_RESOLUTION = 1024;
    const float VREF = ((2.0f / 3.0f) * 5.0f); // Reference voltage for MCP3008 ((2/3) * 5v = 3.3v)
};

#endif