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
        ph_int2,
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

    uint8_t arr[sizeof(arrayIndex)];

    arr[start]      = START_BYTE;

    arr[temp_int]   = getInteger2   (te.getValue());
    arr[temp_dec]   = getDecimal    (te.getValue());

    arr[ph_int1]    = getInteger1   (ph.getValue());
    arr[ph_int2]    = getInteger2   (ph.getValue());
    arr[ph_dec]     = getDecimal    (ph.getValue());

    arr[tds_int1]   = getInteger1   (tds.getValue());
    arr[tds_int2]   = getInteger2   (tds.getValue());
    arr[tds_dec]    = getDecimal    (tds.getValue());

    arr[ec_int1]    = getInteger1   (ec.getValue());
    arr[ec_int2]    = getInteger2   (ec.getValue());
    arr[ec_dec]     = getDecimal    (ec.getValue());

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