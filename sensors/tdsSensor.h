#ifndef tdsSensor_H
#define tdsSensor_H

#include "baseSensorClass.h"
#include "Temperature.h"


class TDS : public Sensor
{
private:
    void poll(MCP3008 *const mcp) override;

public:
    TDS(const uint8_t& channel);
    ~TDS() override;

    void poll(MCP3008 *const mcp, const Temperature& temp);
    void printState() const override;
};

#endif