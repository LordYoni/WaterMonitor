
#include <MCP3008.h> //TODO: get rid of this library

#include "../sensors/baseSensorClass.cpp"
#include "../sensors/PhSensor.cpp"
#include "../sensors/Temperature.cpp"
#include "../sensors/tdsSensor.cpp"
#include "../sensors/ConductivitySensor.cpp"
#include "../sensors/Oxygen.cpp"

#include "../stream.cpp"

#define sendToSerialMonitor true

void setup() { Serial.begin(9600); }

void loop()
{

    enum MCP_PIN : uint8_t
    {
        CS      = 5,
        CLOCK   = 18,
        MOSI    = 23,
        MISO    = 19
    };

    enum MCP_CHANNEL : uint8_t
    {
        CONDUCTIVITY,
        TEMPERATURE,
        PH,
        TDS,
        OXYGEN
    };

    MCP3008             mcp (MCP_PIN::CLOCK, MCP_PIN::MOSI, MCP_PIN::MISO, MCP_PIN::CS);
    
    PhSensor            ph              (&mcp, MCP_CHANNEL::PH                         );
    TemperatureSensor   temperature     (&mcp, MCP_CHANNEL::TEMPERATURE                );
    tdsSensor           tds             (&mcp, MCP_CHANNEL::TDS,            temperature);
    ConductivitySensor  conductivity    (&mcp, MCP_CHANNEL::CONDUCTIVITY,   temperature);
    OxygenSensor        oxygen          (&mcp, MCP_CHANNEL::OXYGEN,         temperature);

    // Read sensor values
    temperature.poll();
    ph.poll();
    tds.poll();
    conductivity.poll();
    oxygen.poll();

    #if sendToSerialMonitor == TRUE

    // Output sensor data
    temperature.printState();
    ph.printState();
    tds.printState();
    conductivity.printState();
    oxygen.printState();

    #else

    sendToXbee(ph, temperature, tds, conductivity, oxygen);

    #endif

    delay(1000);
}
