
#include <MCP3008.h>

#include "sensors/baseSensorClass.cpp"
#include "sensors/PhSensor.cpp"
#include "sensors/Temperature.cpp"
#include "sensors/tdsSensor.cpp"
#include "sensors/ConductivitySensor.cpp"
#include "sensors/Oxygen.cpp"


const uint8_t MCP_CS_PIN = 5;
const uint8_t MCP_CLOCK_PIN = 18;
const uint8_t MCP_MOSI_PIN = 23;
const uint8_t MCP_MISO_PIN = 19;

enum mcpChannel : uint8_t
{
    EC,
    TE,
    PH,
    TDSc,
    OX
};

void setup() { Serial.begin(9600); }

void loop()
{
    MCP3008         mcp (MCP_CLOCK_PIN, MCP_MOSI_PIN, MCP_MISO_PIN, MCP_CS_PIN);
    
    phSensor        ph  (&mcp, mcpChannel::PH);
    Temperature     te  (&mcp, mcpChannel::TE);
    TDS             tds (&mcp, mcpChannel::TDSc, te);
    Conductivity    ec  (&mcp, mcpChannel::EC, te);
    Oxygen          ox  (&mcp, mcpChannel::OX, te);

    // Read sensor values
    te.poll();
    ph.poll();
    tds.poll();
    ec.poll();
    ox.poll();

    // Output sensor data
    /*
    te.printState();
    ph.printState();
    tds.printState();
    ec.printState();
    ox.printState();
    */

    sendToXbee(ph, te, tds, ec, ox);

    delay(1000);
}
