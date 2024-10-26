#ifndef ConductivitySensor_H
#define ConductivitySensor_H

#include "baseSensorClass.h"
#include "Temperature.h"


class ConductivitySensor : public Sensor
{
private:
    const TemperatureSensor &m_temperature;

public:
    ConductivitySensor(const uint8_t &channel, const TemperatureSensor &temperature);
    ~ConductivitySensor() override;

    void poll();
    void printState() const override;

private:
    //Constants for Conductivity calculation
    static constexpr float k = 1.0f;
    static constexpr float RES2 = 820.0f;
    static constexpr float ECREF = 200.0f;
};

#endif