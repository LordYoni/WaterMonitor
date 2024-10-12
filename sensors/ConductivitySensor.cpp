
#include "ConductivitySensor.h"


Conductivity::Conductivity(MCP3008 *const mcp, const uint8_t& channel, const Temperature& temp)
    : Sensor(mcp, channel), m_temperature(temp) {}

Conductivity::~Conductivity() {}

// Function to calculate conductivity
void Conductivity::poll()
{
    pollVoltage();
    //Doesn't poll voltage from temp

    const float ecValue = 100000.0f * m_voltage / RES2 / ECREF * k;
    m_value = ecValue / (1.0f + 0.02f * (m_temperature.getValue() - 25.0f));
}

void Conductivity::printState() const
{
    Serial.print("EC Voltage: ");
    Serial.print(m_voltage);
    Serial.print(" mV, Conductivity: ");
    Serial.print(m_value);
    Serial.println(" uS/cm");
}