#ifndef Sensor_H
#define Sensor_H

#include <MCP3008.h>


class Sensor
{
private:
    //Const ptr to an MCP3008 object
    static MCP3008 *const s_mcp;

    //Cannot be declared as a const reference because the
    //readADC() method is not declared as const even though
    //it doesn't modify any of its class members.

    //MCP channel of sensor
    const uint8_t m_channel;

protected:
    float m_voltage;
    float m_value;

    Sensor(const uint8_t &channel);

    void pollVoltage();

public:
    virtual ~Sensor();

    virtual void poll() = 0;
    virtual void printState() const = 0;

    float getValue() const;

protected:
    static constexpr uint16_t ADC_RESOLUTION = 1024;
    static constexpr float VREF = ((2.0f / 3.0f) * 5.0f); //Reference voltage for MCP3008 ((2/3) * 5v = 3.3v)
};

#endif