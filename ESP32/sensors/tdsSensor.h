#ifndef tdsSensor_H
#define tdsSensor_H

#include "baseSensorClass.h"
#include "Temperature.h"


class tdsSensor : public Sensor
{
private:
    const TemperatureSensor& m_tempeature;

public:
    tdsSensor(MCP3008 *const mcp, const uint8_t &channel, const TemperatureSensor &temperature);
    ~tdsSensor() override;

    void poll();
    void printState() const override;
};

#endif