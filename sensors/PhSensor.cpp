#include <Arduino.h>
#include "Sensor.h"
#include "PhSensor.h"

PhSensor::PhSensor(const uint8_t& pin) : Sensor(pin) {}
PhSensor::~PhSensor() {}


// Fonction pour lire le capteur de pH
void PhSensor::printValue()
{
    poll();
    Serial.print("pH: ");
    Serial.println(m_value);
}


void PhSensor::poll()
{
    const float in = ConvertVoltage(analogRead(m_pin), 5.0f);
    m_value = 3.5f * in;
}