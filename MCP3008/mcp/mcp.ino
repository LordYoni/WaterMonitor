#include <SPI.h>

uint16_t readMCP(const uint8_t& channel)
{
    
    const uint16_t strg = (0x8000 + ((channel & 0x07) << 12));
    uint16_t out;

    SPI.begin();
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

    SPI.transfer((uint8_t)1);

    out = SPI.transfer(strg);

    SPI.endTransaction();
    SPI.end();

    Serial.print(out, BIN);
    Serial.print("  ");
    Serial.print(out, HEX);
    Serial.print("  ");

    return out;
}

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  uint16_t valeur;
  valeur = readMCP(0);

  Serial.println(valeur);
  Serial.println();
  

  delay(1000);
}