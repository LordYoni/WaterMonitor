
#include <MCP3008.h>

#include "sensors/baseSensorClass.cpp"
#include "sensors/PhSensor.cpp"
#include "sensors/Temperature.cpp"
#include "sensors/tdsSensor.cpp"
#include "sensors/ConductivitySensor.cpp"

const uint8_t MCP_CS_PIN = 5;
const uint8_t MCP_CLOCK_PIN = 18;
const uint8_t MCP_MOSI_PIN = 23;
const uint8_t MCP_MISO_PIN = 19;

enum mcpChannel : uint8_t
{
    EC,
    TE,
    PH,
    TDSc
};

void setup() { Serial.begin(115200); }

void loop()
{
    MCP3008         mcp (MCP_CLOCK_PIN, MCP_MOSI_PIN, MCP_MISO_PIN, MCP_CS_PIN);
    
    phSensor        ph  (mcpChannel::PH);
    Temperature     te  (mcpChannel::TE);
    TDS             tds (mcpChannel::TDSc);
    Conductivity    ec  (mcpChannel::EC);

    // Read sensor values
    te.poll(&mcp);
    ph.poll(&mcp);
    tds.poll(&mcp, te);
    ec.poll(&mcp, te);

    // Output sensor data
    te.printState();
    ph.printState();
    tds.printState();
    ec.printState();

    delay(1000);
}
