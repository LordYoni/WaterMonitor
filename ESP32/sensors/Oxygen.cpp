
#include "Oxygen.h"


OxygenSensor::OxygenSensor(MCP3008 *const mcp, const uint8_t &channel, const TemperatureSensor &temperature)
    : Sensor(mcp, channel), m_temperature(temperature) {}

OxygenSensor::~OxygenSensor() {}

//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 1

//Function to calculate oxygen
void OxygenSensor::poll()
{
    pollVoltage();
    //Doesn't poll voltage from temp


    uint16_t voltage_mv = m_voltage * 1000;
    uint8_t temperature_c = (uint8_t)(m_temperature.getValue());

    #if TWO_POINT_CALIBRATION == 0

        uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature_c - (uint32_t)CAL1_T * 35;
        m_value = (voltage_mv * DO_Table[temperature_c] / V_saturation) / 1000.0f;

    #else

        uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T) + CAL2_V;
        m_value = (voltage_mv * DO_Table[temperature_c] / V_saturation) / 1000.0f;

    #endif

}

void OxygenSensor::printState() const
{
    Serial.print(F("DO: "));
    Serial.print(m_value);
    Serial.println(F(" mg/L"));
}