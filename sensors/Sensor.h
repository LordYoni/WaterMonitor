#ifndef Sensor_H
#define Sensor_H

class Sensor
{
protected:
    const uint8_t m_pin;
    float m_value;

    Sensor(const uint8_t& pin);

    float ConvertVoltage(const uint16_t& in, const float& v1) const;

public:
    virtual ~Sensor();

    virtual void poll() = 0;
    virtual void printValue() = 0;
};

#endif