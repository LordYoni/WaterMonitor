
#include "Temperature.h"


Temperature::Temperature(MCP3008 *const mcp, const uint8_t& channel) : Sensor(mcp, channel) {}
Temperature::~Temperature() {}

// Function to calculate temperature from PT1000 sensor
void Temperature::poll()
{
    pollVoltage();

    const float Rpt1000 = (m_voltage / GDIFF + VR0) / I / G0;
    m_value = (Rpt1000 - 1000.0f) / 3.85f;
}

void Temperature::printState() const
{
    Serial.print("Temperature: ");
    Serial.print(m_value);
    Serial.println(" °C");
}