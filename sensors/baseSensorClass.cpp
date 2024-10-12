
#include "baseSensorClass.h"


Sensor::Sensor(MCP3008 *const mcp, const uint8_t& channel) : m_mcp(mcp), m_channel(channel) {}
Sensor::~Sensor() {}

void Sensor::pollVoltage()
{
    m_voltage = ((float)m_mcp->readADC(m_channel)) * VREF / (float)ADC_RESOLUTION;
}

float Sensor::getValue() const { return m_value; }