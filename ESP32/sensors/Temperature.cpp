
#include "Temperature.h"


TemperatureSensor::TemperatureSensor(MCP3008 *const mcp, const uint8_t &channel) : Sensor(mcp, channel) {}
TemperatureSensor::~TemperatureSensor() {}

//Function to calculate temperature from PT1000 sensor
void TemperatureSensor::poll()
{
    pollVoltage();

    const float Rpt1000 = (m_voltage / GDIFF + VR0) / I / G0;
    m_value = (Rpt1000 - 1000.0f) / 3.85f;
}

void TemperatureSensor::printState() const
{
    Serial.print(F("TemperatureSensor: "));
    Serial.print(m_value);
    Serial.println(F(" °C"));
}