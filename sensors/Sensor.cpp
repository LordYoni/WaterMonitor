
#include <Arduino.h>
#include "Sensor.h"

Sensor::Sensor(const uint8_t& pin) : m_pin(pin) { pinMode(pin, INPUT); }
Sensor::~Sensor() {}

// Conversion de la tension (0~v1 V)
float Sensor::ConvertVoltage(const uint16_t& in, const float& v1) const
{
    return (in * (v1 / 1023.0f));
}