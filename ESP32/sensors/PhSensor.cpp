
#include "PhSensor.h"


PhSensor::PhSensor(MCP3008 *const mcp, const uint8_t& channel) : Sensor(mcp, channel) {}
PhSensor::~PhSensor() {}

// Function to calculate pH value
void PhSensor::poll()
{
    pollVoltage();

    // ??
    //const float voltage_scaled = m_voltage * VREF / 1024.0f;
    //m_value = 3.5f * voltage_scaled + OFFSET;
    
    m_value = 3.5f * m_voltage + OFFSET;
}

void PhSensor::printState() const
{
    Serial.print("pH: ");
    Serial.println(m_value);
}