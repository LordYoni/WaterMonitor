
#include "tdsSensor.h"


TDS::TDS(MCP3008 *const mcp, const uint8_t& channel, const Temperature& temp)
    : Sensor(mcp, channel), m_tempeature(temp) {}

TDS::~TDS() {}

// Function to calculate TDS value
void TDS::poll()
{
    pollVoltage();
    //Doesn't poll voltage from temp
    
    const float compensationCoefficient = 1.0f + 0.02f * (m_tempeature.getValue() - 25.0f);
    const float compensatedVoltage = m_voltage / compensationCoefficient;

    m_value = (133.42f * pow(compensatedVoltage, 3.0f) -
          255.86f * pow(compensatedVoltage, 2.0f) + 857.39f * compensatedVoltage) * 0.5f;
}

void TDS::printState() const
{
    Serial.print("TDS: ");
    Serial.print(m_value);
    Serial.println(" ppm");
}