
#include "PhSensor.h"


phSensor::phSensor(const MCP3008& mcp, const uint8_t& channel) : Sensor(mcp, channel) {}
phSensor::~phSensor() {}

// Function to calculate pH value
void phSensor::poll()
{
    pollVoltage();

    // ??
    //const float voltage_scaled = m_voltage * VREF / 1024.0f;
    //m_value = 3.5f * voltage_scaled + OFFSET;
    
    m_value = 3.5f * m_voltage + OFFSET;
}

void phSensor::printState() const
{
    Serial.print("pH: ");
    Serial.println(m_value);
}