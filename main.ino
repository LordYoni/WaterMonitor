#include <Adafruit_MCP3008.h>

// Pins and MCP3008 configuration
#define MCP3008_CS_PIN 5  // Chip Select pin for MCP3008 (D5)

#define EC_PIN 0          // MCP3008 channel 0 for EC
#define TE_PIN 1          // MCP3008 channel 1 for Temperature
#define PH_PIN 2          // MCP3008 channel 2 for pH
#define TDS_PIN 3         // MCP3008 channel 3 for TDS

#define VREF 5.0          // Reference voltage for MCP3008

Adafruit_MCP3008 mcp;     // Create an MCP3008 instance

// Variables for sensor readings
uint16_t EC_Voltage, TE_Voltage, PH_Voltage, TDS_Voltage;
float Conductivity, Temp, pHValue, TDSValue;

// Constants for Temperature and Conductivity calculations
float k = 1;
#define RES2 820.0
#define ECREF 200.0
#define GDIFF (30 / 1.8)
#define VR0 0.223
#define G0 2
#define I (1.24 / 10000)

// Constants for pH calculation
#define Offset -5.00

// Function to calculate temperature from PT1000 sensor
float get_temp(float voltage) {
  float Rpt1000 = (voltage / GDIFF + VR0) / I / G0;
  float temp = (Rpt1000 - 1000) / 3.85;
  return temp;
}

// Function to calculate conductivity
float get_us_cm(float voltage, float temp) {
  float ecValue = 100000 * voltage / RES2 / ECREF * k;
  float value = ecValue / (1.0 + 0.02 * (temp - 25.0));
  return value;
}

// Function to calculate pH value
float get_pH(float voltage) {
  float voltage_scaled = voltage * VREF / 1024.0;
  float pHValue = 3.5 * voltage_scaled + Offset;
  return pHValue;
}

// Function to calculate TDS value
float get_TDS(float voltage, float temperature) {
  float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);
  float compensatedVoltage = voltage / compensationCoefficient;
  return (133.42 * compensatedVoltage * compensatedVoltage * compensatedVoltage -
          255.86 * compensatedVoltage * compensatedVoltage + 857.39 * compensatedVoltage) * 0.5;
}

void setup() {
  Serial.begin(115200);
  if (!mcp.begin(MCP3008_CS_PIN)) {
    Serial.println("Failed to initialize MCP3008!");
    while (1);
  }
}

void loop() {
  // Read sensor values from MCP3008 channels
  EC_Voltage = mcp.readADC(EC_PIN) * VREF / 1024.0;
  TE_Voltage = mcp.readADC(TE_PIN) * VREF / 1024.0;
  PH_Voltage = mcp.readADC(PH_PIN) * VREF / 1024.0;
  TDS_Voltage = mcp.readADC(TDS_PIN) * VREF / 1024.0;

  // Calculate temperature and conductivity
  Temp = get_temp((float)TE_Voltage);
  Conductivity = get_us_cm(EC_Voltage, Temp);
  pHValue = get_pH(PH_Voltage);
  TDSValue = get_TDS(TDS_Voltage, Temp);

  // Output sensor data
  Serial.print("EC Voltage: ");
  Serial.print(EC_Voltage);
  Serial.print(" mV, Conductivity: ");
  Serial.print(Conductivity);
  Serial.print(" uS/cm, Temperature: ");
  Serial.print(Temp);
  Serial.print(" °C, pH: ");
  Serial.print(pHValue);
  Serial.print(", TDS: ");
  Serial.print(TDSValue);
  Serial.println(" ppm");

  delay(1000);  // Delay for 1 second
}
