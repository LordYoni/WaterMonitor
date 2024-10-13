import serial
import mysql.connector as MC

def add_comma(arr):
    if arr < 10:
        return str(".0" + str(arr))
    else:
        return str("." + str(arr))

# Configuration of serial communication parameters
timeout_sec = 3
input_size = 16

# Definition of start and end bytes
start_byte = 0xa0
end_byte = 0x0a

# Initialization of the serial connection
serial_in = serial.Serial('/dev/ttyUSB0', 9600)  # ,serial.EIGHTBITS,serial.PARITY_NONE,serial.STOPBITS_ONE,timeout_sec)

# Resetting the output buffer (necessary?)
serial_in.reset_output_buffer()

def insert_data(tds, ph, oxygen, conductivity, temperature):
    query = """
        INSERT INTO Data (TDS, pH, Oxygen, Conductivity, Temperature)
        VALUES (%s, %s, %s, %s, %s)
    """
    args = (tds, ph, oxygen, conductivity, temperature)

    try:
        conn = MC.connect(host='localhost', database='WaterData', user='dbaccess', password='')
        cursor = conn.cursor()
        cursor.execute(query, args)
        conn.commit()
        print("Data inserted successfully")
    except MC.Error as err:
        print("Error while inserting data:", err)
    finally:
        cursor.close()
        conn.close()
        
while True:
    # Resetting the input buffer
    serial_in.reset_input_buffer()

    # Reading data from the serial port
    array_in = serial_in.read(input_size)

    # Checking the size of the read data
    if len(array_in) == input_size:

        # Checking the start and end bytes
        if array_in[0] == start_byte and array_in[-1] == end_byte:

            # Calculating the checksum
            clc_checksum = 0
            
            for i in range(1, input_size - 2):
                clc_checksum += array_in[i]

            clc_checksum = clc_checksum % 0x100

            # Verifying the checksum
            if clc_checksum == array_in[-2]:
                # Converting data to usable values
                temperature = float(str(int.from_bytes([array_in[1]], byteorder='big', signed=True)) + add_comma(array_in[2]))
                ph = float(str(array_in[3]) + add_comma(array_in[4]))

                temp = str(array_in[5] * 256 + array_in[6])
                tds = float(temp + add_comma(array_in[7]))

                temp = str(array_in[8] * 256 + array_in[9])
                ec = float(temp + add_comma(array_in[10]))

                temp = str(array_in[11] * 256 + array_in[12])
                ox = float(temp + add_comma(array_in[13]))

                for i in range(1, input_size - 1):
                    print(hex(array_in[i]))

                print("Temperature: " + str(temperature))
                print("pH: " + str(ph))
                print("TDS: " + str(tds))
                print("EC: " + str(ec))
                print("OX: " + str(ox))

                print("Data received successfully\n")

                insert_data(tds, ph, ox, ec, temperature)

            else:
                print("Checksum error")

        else:
            print("Error: start and/or end byte mismatch")
