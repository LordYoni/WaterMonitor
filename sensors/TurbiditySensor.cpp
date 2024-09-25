
#include <Arduino.h>
#include "Sensor.h"
#include "TurbiditySensor.h"

TurbiditySensor::TurbiditySensor(const uint8_t& pin) : Sensor(pin) {}
TurbiditySensor::~TurbiditySensor() {}


void TurbiditySensor::printValue()
{
    poll();
    Serial.print("Turbidité (NTU): ");
    Serial.println(m_value);
}


// Fonction pour lire le capteur de turbidité
void TurbiditySensor::poll()
{
    const float in = ConvertVoltage(analogRead(m_pin), 4.5f);
    m_value = -1120.4f * pow(in, 2) + 5742.3f * in - 4352.9f;
}