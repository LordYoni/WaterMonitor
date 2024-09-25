
#include <Arduino.h>
#include "Sensor.h"
#include "ConductivitySensor.h"

ConductivitySensor::ConductivitySensor(const uint8_t& pin) : Sensor(pin) {}
ConductivitySensor::~ConductivitySensor() {}


void ConductivitySensor::printValue()
{
    poll();
    Serial.print("Conductivité (uS/cm): ");
    Serial.println(m_value);
}


// Fonction pour lire le capteur de conductivité
void ConductivitySensor::poll()
{
    const float R1              = 1.0f;   // Résistance pour le capteur de conductivité
    const float conductivityVin = 5.0f;   // Tension d'alimentation pour le capteur de conductivité

    const float in = ConvertVoltage(analogRead(m_pin), 3.3f);
    m_value = (in * 1000.0f) / (R1 * conductivityVin - in);
}