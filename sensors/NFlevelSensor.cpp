
#include <Arduino.h>
#include "Sensor.h"
#include "NFlevelSensor.h"

NFlevelSensor::NFlevelSensor(const uint8_t& pin) : Sensor(pin) {}
NFlevelSensor::~NFlevelSensor() {}


void NFlevelSensor::printValue()
{
    poll();
    Serial.print("Niveau (V): ");
    Serial.println(m_value);
}


// Fonction pour lire le capteur de niveau NF
void NFlevelSensor::poll()
{
    m_value = ConvertVoltage(analogRead(m_pin), 5.0f);
}
