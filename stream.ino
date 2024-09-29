
#include "sensors/baseSensorClass.h"
#include "sensors/PhSensor.h"
#include "sensors/Temperature.h"
#include "sensors/tdsSensor.h"
#include "sensors/ConductivitySensor.h"

inline void sendToXbee
(
    const phSensor&     ph,
    const Temperature&  te,
    const TDS&          tds,
    const Conductivity& ec
)
{
    const uint8_t START_BYTE    = 0xa0;
    const uint8_t STOP_BYTE     = 0x0a;

    enum arrayIndex : uint8_t
    {
        start,
        temp_int,
        temp_dec,
        ph_int1,
        ph_dec,
        tds_int1,
        tds_int2,
        tds_dec,
        ec_int1,
        ec_int2,
        ec_dec,
        checksum,
        end
    };


    //1 start byte
    //te: 1 byte signed integer, 1 byte decimal
    //Ph: 1 byte unsigned integer, 1 byte decimal
    //tds 2 bytes unsigned integer, 1 byte decimal
    //ec: 2 bytes unsigned integer, 1 byte decimal
    //1 checksum byte
    //1 stop byte

    uint8_t arr[13];

    arr[start]      = START_BYTE;

    uint8_t arr_index = temp_int;

    writeToArray(arr, arr_index, te, 2);
    writeToArray(arr, arr_index, ph, 2);
    writeToArray(arr, arr_index, tds, 3);
    writeToArray(arr, arr_index, ec, 3);

    //calculate checksum
    uint8_t chk = 0;

    for(uint8_t i = arrayIndex::temp_int; i != arrayIndex::checksum; i++)
        chk += arr[i];
    
    arr[checksum] = chk;

    arr[end] = STOP_BYTE;

    //TODO: send
}

uint8_t getInteger1 (const float& inp)  { return uint8_t(inp / 100); }
uint8_t getInteger2 (const float& inp)  { return uint8_t(inp); }
uint8_t getDecimal  (const float& inp)  { return uint8_t(inp * 100); }

void writeToArray
(
    uint8_t *arr,
    uint8_t &index,
    const Sensor& sen,
    const uint8_t& size
)
{
    if (size == 3)
        arr[index++]    = getInteger1   (sen.getValue());

    arr[index++]    = getInteger2   (sen.getValue());
    arr[index++]    = getDecimal    (sen.getValue());
}