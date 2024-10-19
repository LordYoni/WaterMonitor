
#include "ConductivitySensor.h"


ConductivitySensor::ConductivitySensor(MCP3008 *const mcp, const uint8_t &channel, const TemperatureSensor &temperature)
    : Sensor(mcp, channel), m_temperature(temperature) {}

ConductivitySensor::~ConductivitySensor() {}

//Function to calculate conductivity
void ConductivitySensor::poll()
{
    pollVoltage();
    //Doesn't poll voltage from temperature

    const float ecValue = 100000.0f * m_voltage / RES2 / ECREF * k;
    m_value = ecValue / (1.0f + 0.02f * (m_temperature.getValue() - 25.0f));
}

void ConductivitySensor::printState() const
{
    Serial.print(F("EC Voltage: "));
    Serial.print(m_voltage);
    Serial.print(F(" mV, Conductivity: "));
    Serial.print(m_value);
    Serial.println(F(" uS/cm"));
}