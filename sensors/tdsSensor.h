#ifndef tdsSensor_H
#define tdsSensor_H

#include "baseSensorClass.h"
#include "Temperature.h"


class TDS : public Sensor
{
private:
    void poll() override;

public:
    TDS(const MCP3008& mcp, const uint8_t& channel);
    ~TDS() override;

    void poll(const Temperature& temp);
    void printState() const override;
};

#endif