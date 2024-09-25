
#include <Arduino.h>
#include "Sensor.h"
#include "PressureSensor.h"

PressureSensor::PressureSensor(const uint8_t& pin) : Sensor(pin) {}
PressureSensor::~PressureSensor() {}


void PressureSensor::printValue()
{
    poll();
    Serial.print("Pression (psi): ");
    Serial.println(m_value);
}


// Fonction pour lire le capteur de pression
void PressureSensor::poll()
{
    const float pressureVoffset         = 0.5f;   // Offset pour le capteur de pression
    const float pressureSensitivity     = 0.02f;  // Sensibilité du capteur de pression (TODO: à vérifier)

    const float in = ConvertVoltage(analogRead(m_pin), 5.0f);
    m_value = (in - pressureVoffset) / pressureSensitivity;
}