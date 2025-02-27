import serial
import mysql.connector as MC
from dotenv import load_dotenv
import os
from enum import IntEnum, auto

load_dotenv()

def insert_data(tds, ph, oxygen, conductivity, temperature):
    query = """
        INSERT INTO Data (TDS, pH, Oxygen, Conductivity, Temperature)
        VALUES (%s, %s, %s, %s, %s)
    """
    args = (tds, ph, oxygen, conductivity, temperature)

    try:
        conn = MC.connect(
            host=os.getenv('DB_HOST'),
            database=os.getenv('DB_NAME'),
            user=os.getenv('DB_USER'),
            password=os.getenv('DB_PASSWORD')
        )
        cursor = conn.cursor()
        cursor.execute(query, args)
        conn.commit()
        print("Data inserted successfully")
    except MC.Error as err:
        print("Error while inserting data:", err)
    finally:
        cursor.close()
        conn.close()


def add_comma(number):
    if number < 10:
        return ".0" + str(number)
    else:
        return "." + str(number)


class ArrayIndex(IntEnum):
    start = 0

    temperature_LSB = auto()
    temperature_dec = auto()

    pH_LSB = auto()
    pH_dec = auto()

    TDS_MSB = auto()
    TDS_LSB = auto()
    TDS_dec = auto()

    conductivity_MSB = auto()
    conductivity_LSB = auto()
    conductivity_dec = auto()

    oxygen_MSB = auto()
    oxygen_LSB = auto()
    oxygen_dec = auto()

    checksum = auto()
    end = auto()


expected_input_size = len(ArrayIndex)
start_byte  = 0xa0
end_byte    = 0x0a

serial_timeout_sec = 3

Xbee = serial.Serial('/dev/ttyUSB0', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE, serial_timeout_sec)

while True:
    Xbee.reset_input_buffer()

    array_in = Xbee.read(expected_input_size)

    if len(array_in) == expected_input_size:
        if array_in[ArrayIndex.start.value] == start_byte and array_in[ArrayIndex.end.value] == end_byte:

            #Calculate checksum
            checksum = 0

            for i in range(ArrayIndex.start.value + 1, ArrayIndex.checksum.value):
                checksum += array_in[i]

            checksum = checksum % 0x100

            if checksum == array_in[ArrayIndex.checksum.value]:

                #Retrieve data from array

                if array_in[ArrayIndex.temperature_LSB.value] != 0xff:

                    #Handle negative temperature
                    temp = array_in[ArrayIndex.temperature_LSB.value]

                    temp = ('-' + str((temp ^ 0xff) + 1)) if temp & 0x80 else str(temp)

                    temperature     = float(temp + add_comma( array_in[ArrayIndex.temperature_dec.value]))

                else:
                    temperature     = 0.0



                ph = (float(str(array_in[ArrayIndex.pH_LSB.value]) + add_comma(array_in[ArrayIndex.pH_dec.value]))) if array_in[ArrayIndex.pH_LSB.value] != 0xff else 0.0



                if array_in[ArrayIndex.TDS_MSB.value] != 0xff:
                    temp            = str(array_in[ArrayIndex.TDS_MSB.value] * 256 + array_in[ArrayIndex.TDS_LSB.value])
                    tds             = float(temp + add_comma(array_in[ArrayIndex.TDS_dec.value]))

                else:
                    tds             = 0.0



                if array_in[ArrayIndex.conductivity_MSB.value] != 0xff:
                    temp            = str(array_in[ArrayIndex.conductivity_MSB.value] * 256 + array_in[ArrayIndex.conductivity_LSB.value])
                    conductivity    = float(temp + add_comma(array_in[ArrayIndex.conductivity_dec.value]))

                else:
                    conductivity    = 0.0



                if array_in[ArrayIndex.oxygen_MSB.value] != 0xff:
                    temp            = str(array_in[ArrayIndex.oxygen_MSB.value] * 256 + array_in[ArrayIndex.oxygen_LSB.value])
                    oxygen          = float(temp + add_comma(array_in[ArrayIndex.oxygen_dec.value]))

                else:
                    oxygen = 0.0



                for i in range(ArrayIndex.start.value + 1, ArrayIndex.checksum.value):
                    print(hex(array_in[i]))

                print()

                print("Temperature: "   + str(temperature))
                print("pH: "            + str(ph))
                print("TDS: "           + str(tds))
                print("Conductivity: "  + str(conductivity))
                print("Oxygen: "        + str(oxygen))

                insert_data(tds, ph, oxygen, conductivity, temperature)

            else:
                print("Checksum error")

        else:
            print("Error: start and/or end byte mismatch")
