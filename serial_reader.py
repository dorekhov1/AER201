import serial
import time


def print_time(s, rtc):
    print("Time of", s, ": ", format(rtc[6], '02x'), "/", format(rtc[5], '02x'), "/", format(rtc[4], '02x'), " ",
          format(rtc[2], '02x'), ":", format(rtc[1], '02x'), ":", format(rtc[0], '02x'))


def print_emergency(emergency):
    if emergency == 1:
        print("Operation was interrupted by emergency signal")


def print_taped(byte1, byte2):
    taped_bitmask = [bool(byte1 & (1 << n)) for n in range(8)] + [bool(byte2 & (1 << n)) for n in range(8)]

    taped_drawers = []
    for i in range(len(taped_bitmask)):
        if taped_bitmask[i]:
            taped_drawers.append(i)

    if len(taped_drawers) == 0:
        print("No drawers taped")
    else:
        print("Taped Drawers: " + ', '.join(taped_drawers))


def print_count(r, f, l):
    print("R pills counted: ", r)
    print("F pills counted: ", f)
    print("L pills counted: ", l)


def print_operations(num, data):
    for i in range(num):
        operation = data[3 * i:3 + 3 * i]
        print("Operation ", i + 1, ": ", DIETS[operation[1] - 1], "x", operation[2], " into ", operation[0])


ser = serial.Serial('COM1', 9600, timeout=0)

DIETS = ["R", "F", "L", "RF", "RL", "FL", "RRF", "RRL", "RFF", "RLL", "RFL", "FFL",
         "FLL", "RRFL", "RFFL", "RFLL", "RLLL", "FLLL"]

line_num_read = False
line_num = 0
lines_read = 0
lines_processed = 0

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

                while lines_processed != line_num:

                    operation_num = all_lines[lines_processed]

                    print_time("start", all_lines[lines_processed+1:lines_processed+8])
                    print_operations(operation_num, all_lines[lines_processed+8:lines_processed+11+3*(operation_num-1)])
                    print_emergency(all_lines[lines_processed+11 + 3 * (operation_num - 1)])

                    print_taped(all_lines[lines_processed+12 + 3 * (operation_num - 1)],
                                all_lines[lines_processed+13 + 3 * (operation_num - 1)])

                    print_count(all_lines[lines_processed+14 + 3 * (operation_num - 1)],
                                all_lines[lines_processed+15 + 3 * (operation_num - 1)],
                                all_lines[lines_processed+16 + 3 * (operation_num - 1)])

                    print_time("completion",
                               all_lines[lines_processed+17+3*(operation_num-1):lines_processed+24+3*(operation_num-1)])

                    lines_processed += 24 + 3 * (operation_num - 1)
                    print()

                reading_incomplete = False

    except ser.SerialTimeoutException:
        print('Data could not be read')
    time.sleep(1)
