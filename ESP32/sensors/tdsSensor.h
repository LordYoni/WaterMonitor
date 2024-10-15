#ifndef tdsSensor_H
#define tdsSensor_H

#include "baseSensorClass.h"
#include "Temperature.h"


class TDS : public Sensor
{
private:
    const Temperature& m_tempeature;

public:
    TDS(MCP3008 *const mcp, const uint8_t& channel, const Temperature& temp);
    ~TDS() override;

    void poll();
    void printState() const override;
};

#endif