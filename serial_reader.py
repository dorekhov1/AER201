import serial
import time
import sys

def update_progress(progress):
    barLength = 20 # Modify this to change the length of the progress bar
    status = ""
    if isinstance(progress, int):
        progress = float(progress)
    if not isinstance(progress, float):
        progress = 0
        status = "error: progress var must be float\r\n"
    if progress < 0:
        progress = 0
        status = "Halt...\r\n"
    if progress >= 1:
        progress = 1
        status = "Done...\r\n"
    block = int(round(barLength*progress))
    text = "\rPercent: [{0}] {1}% {2}".format( "="*block + " "*(barLength-block), progress*100, status)
    sys.stdout.write(text)
    sys.stdout.flush()


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

while reading_incomplete:
    try:
        if not line_num_read:
            first_byte = ser.read()
            if first_byte and first_byte != b'\x00':
                line_num = -1 + first_byte[0]
                print("Line number: ", line_num)
                line_num_read = True
        else:
            if line_num == 0:
                print("No logs to transfer.")
                reading_incomplete = False
            else:
                while lines_read != line_num:
                    line = ser.readline()
                    update_progress(lines_read/line_num)
                    if line:
                        all_lines += line
                        lines_read += len(line)

                update_progress(lines_read/line_num)
                print()

                while lines_processed != line_num:

                    operation_num = all_lines[lines_processed]

                    print_time("start", all_lines[lines_processed+1:lines_processed+8])
                    print_operations(operation_num, all_lines[lines_processed+8:lines_processed+11+3*(operation_num-1)])

                    print_taped(all_lines[lines_processed+11 + 3 * (operation_num - 1)],
                                all_lines[lines_processed+12 + 3 * (operation_num - 1)])

                    print_count(all_lines[lines_processed+13 + 3 * (operation_num - 1)],
                                all_lines[lines_processed+14 + 3 * (operation_num - 1)],
                                all_lines[lines_processed+15 + 3 * (operation_num - 1)])

                    print_time("completion",
                               all_lines[lines_processed+16+3*(operation_num-1):lines_processed+23+3*(operation_num-1)])

                    lines_processed += 23 + 3 * (operation_num - 1)
                    print()

                reading_incomplete = False

    except ser.SerialTimeoutException:
        print('Data could not be read')
    time.sleep(1)
