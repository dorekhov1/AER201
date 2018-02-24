import serial
import time

ser = serial.Serial('COM1', 9600, timeout=0)

DIETS = ["R", "F", "L", "RF", "RL", "FL", "RRF", "RRL", "RFF", "RLL", "RFL", "FFL",
    "FLL", "RRFL", "RFFL", "RFLL", "RLLL", "FLLL"]

line_num_read = False
line_num = 0
lines_read = 0

all_lines = b''

reading_incomplete = True

while 1:
    try:
        if not line_num_read:
            first_byte = ser.read()
            if first_byte and first_byte != b'\x00':
                line_num = -1 + first_byte[0]
                line_num_read = True
        else:
            while lines_read != line_num:
                line = ser.readline()
                if line:
                    all_lines += line
                    lines_read += len(line)
            if reading_incomplete:

                operation_num = all_lines[0]

                rtc = all_lines[1:9]
                print("Time: ", format(rtc[6], '02x'), "/", format(rtc[5], '02x'), "/", format(rtc[4], '02x'), " ",
                      format(rtc[2], '02x'), ":", format(rtc[1], '02x'), ":", format(rtc[0], '02x'))

                for i in range(operation_num):
                    operation = all_lines[(8 + 3 * i):(11 + 3 * i)]
                    print("Operation ", i + 1, ": ", DIETS[operation[1] - 1], "x", operation[2], " into ", operation[0])

                print("Taped Drawers: ", all_lines[11 + 3 * (operation_num - 1)])

                reading_incomplete = False

    except ser.SerialTimeoutException:
        print('Data could not be read')
    time.sleep(1)