
#include "PhSensor.h"


PhSensor::PhSensor(const uint8_t &channel) : Sensor(channel) {}
PhSensor::~PhSensor() {}

//Function to calculate pH value
void PhSensor::poll()
{
    pollVoltage();

    m_value = 3.5f * m_voltage + OFFSET;
}

void PhSensor::printState() const
{
    Serial.print(F("pH: "));
    Serial.println(m_value);
}