import serial

timeout_sec = 3
input_size = 13

start_byte = 0xa0
end_byte = 0x0a


serial_in = serial.Serial('/dev/ttyUSB0',9600) #,serial.EIGHTBITS,serial.PARITY_NONE,serial.STOPBITS_ONE,timeout_sec)


serial_in.reset_output_buffer() #necesarry?

while 1:
    serial_in.reset_input_buffer()

    array_in = serial_in.read(input_size)

    if len(array_in) == input_size:

        if array_in[0] == start_byte and array_in[-1] == end_byte:
            clc_checksum = 0
        
            for i in range(1,input_size-2):
                clc_checksum += array_in[i]
            
            clc_checksum = clc_checksum % 0x100

            if clc_checksum == array_in[-2]:

                temperature = float(str(int.from_bytes([array_in[1]],byteorder='big', signed='true')) + "." + str(array_in[2]))
                ph = float(str(array_in[3]) + "." + str(array_in[4]))

                temp = str(array_in[5]*256 + array_in[6])

                tds = float(temp + "." + str(array_in[7]))

                temp = str(array_in[8]*256 + array_in[9])
                ec = float(temp + "." + str(array_in[10]))

                print("temp " + str(temperature))
                print("ph " + str(ph))
                print("tds " + str(tds))
                print("ec " + str(ec))
                print("ok")




                # TODO: send in database

            else:
                print("checksum error")

        else:
            print("Error start and/or end byte")