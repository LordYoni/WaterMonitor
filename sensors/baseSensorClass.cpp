
#include "baseSensorClass.h"


Sensor::Sensor(const uint8_t& channel) : m_channel(channel) {}
Sensor::~Sensor() {}

void Sensor::pollVoltage(MCP3008 *const mcp)
{
    m_voltage = ((float)mcp->readADC(m_channel)) * VREF / (float)ADC_RESOLUTION;
}

float Sensor::getValue() const { return m_value; }