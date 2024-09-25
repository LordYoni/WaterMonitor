
#include <Arduino.h>
#include "Sensor.h"
#include "OxygenSensor.h"

OxygenSensor::OxygenSensor(const uint8_t& pin) : Sensor(pin) {}
OxygenSensor::~OxygenSensor() {}


void OxygenSensor::printValue()
{
    poll();
    Serial.print("Oxygène dissous (mg/L): ");
    Serial.println(m_value);
}

// Fonction pour lire le capteur d'oxygène dissous
void OxygenSensor::poll()
{
    const float oxygenCalibrationFactor = 1.0f;   // Facteur de calibration (TODO: à définir)
    const float oxygenVmax              = 3.3f;   // Tension maximale pour l'oxygène dissous
    const float oxygenVmin              = 0.0f;   // Tension minimale pour l'oxygène dissous

    const float in = ConvertVoltage(analogRead(m_pin), 3.3f);
    m_value = (in * 1000.0f) / (oxygenVmax - oxygenVmin) * oxygenCalibrationFactor;
}