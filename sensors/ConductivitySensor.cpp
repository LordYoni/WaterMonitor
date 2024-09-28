
#include "ConductivitySensor.h"


Conductivity::Conductivity(const uint8_t& channel) : Sensor(channel) {}
Conductivity::~Conductivity() {}

void Conductivity::poll(MCP3008 *const mcp) {};

// Function to calculate conductivity
void Conductivity::poll(MCP3008 *const mcp, const Temperature& temp)
{
    pollVoltage(mcp);
    //Doesn't poll voltage from temp

    const float ecValue = 100000.0f * m_voltage / RES2 / ECREF * k;
    m_value = ecValue / (1.0f + 0.02f * (temp.getValue() - 25.0f));
}

void Conductivity::printState() const
{
    Serial.print("EC Voltage: ");
    Serial.print(m_voltage);
    Serial.print(" mV, Conductivity: ");
    Serial.print(m_value);
    Serial.println(" uS/cm");
}