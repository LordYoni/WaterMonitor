
#include <MCP3008.h> //TODO: get rid of this library

#include "../sensors/baseSensorClass.cpp"
#include "../sensors/PhSensor.cpp"
#include "../sensors/Temperature.cpp"
#include "../sensors/tdsSensor.cpp"
#include "../sensors/ConductivitySensor.cpp"
#include "../sensors/Oxygen.cpp"

#include "../stream.cpp"

//Defines whether we want to output data to Xbee or
//in a readable format in the serial monitor
#define sendToSerialMonitor true

enum MCP_PIN : uint8_t
{
    MCP_CS      = 5,
    MCP_CLOCK   = 18,
    MCP_MOSI    = 23,
    MCP_MISO    = 19
};

static MCP3008 mcp(MCP_CLOCK, MCP_MOSI, MCP_MISO, MCP_CS);

MCP3008 *const Sensor::s_mcp = &mcp;

void setup() { Serial.begin(9600); }

void loop()
{
    enum MCP_CHANNEL : uint8_t
    {
        CONDUCTIVITY,
        TEMPERATURE,
        PH,
        TDS,
        OXYGEN
    };

    PhSensor            ph              (MCP_CHANNEL::PH                       );
    TemperatureSensor   temperature     (MCP_CHANNEL::TEMPERATURE              );
    tdsSensor           tds             (MCP_CHANNEL::TDS,          temperature);
    ConductivitySensor  conductivity    (MCP_CHANNEL::CONDUCTIVITY, temperature);
    OxygenSensor        oxygen          (MCP_CHANNEL::OXYGEN,       temperature);

    //Read sensor values
    temperature.poll();
    ph.poll();
    tds.poll();
    conductivity.poll();
    oxygen.poll();

    #if sendToSerialMonitor == true

        //Output sensor data to serial monitor
        temperature.printState();
        ph.printState();
        tds.printState();
        conductivity.printState();
        oxygen.printState();

        Serial.println();

    #else

        sendToXbee(ph, temperature, tds, conductivity, oxygen);

    #endif

    delay(1000);
}
