import serial
import time
import sys

def update_progress(progress):
    barLength = 20
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


def print_time(rtc):
    print("Time of start: ", format(rtc[6], '02x'), "/", format(rtc[5], '02x'), "/", format(rtc[4], '02x'), " ",
          format(rtc[2], '02x'), ":", format(rtc[1], '02x'), ":", format(rtc[0], '02x'))


def print_runtime(time):
    print("Run Duration: ", time, "s")


def print_count(r, f, l):
    print("R pills counted: ", r)
    print("F pills counted: ", f)
    print("L pills counted: ", l)


def print_operations(num, data):
    for i in range(num):
        operation = data[3 * i:3 + 3 * i]
        dest = operation[0]
        if dest >= 100: print("Operation ", i + 1, ": ", DIETS[operation[1] - 1], "x", operation[2], " into ", dest - 100, "taped")
        else: print("Operation ", i + 1, ": ", DIETS[operation[1] - 1], "x", operation[2], " into ", dest)


ser = serial.Serial('COM1', 9600, timeout=0)  # open serial port on COM1

DIETS = ["R", "F", "L", "RF", "RL", "FL", "RRF", "RRL", "RFF", "RLL", "RFL", "FFL",
         "FLL", "RRFL", "RFFL", "RFLL", "RLLL", "FLLL"]

line_num_read = False  # flag to indicate if the total number of lines wa received
line_num = 0  # integer storing total number of lines
lines_read = 0  # integer storing the number of lines read
lines_processed = 0  # integer storing the number of lines processed

all_lines = b''  # variable storing all lines received

reading_incomplete = True  # flag to indicate if reading is complete

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

                    print_time(all_lines[lines_processed+1:lines_processed+8])
                    print_operations(operation_num, all_lines[lines_processed+8:lines_processed+11+3*(operation_num-1)])

                    print_count(all_lines[lines_processed+11 + 3 * (operation_num - 1)],
                                all_lines[lines_processed+12 + 3 * (operation_num - 1)],
                                all_lines[lines_processed+13 + 3 * (operation_num - 1)])

                    print_runtime(all_lines[lines_processed+14+3*(operation_num-1)])

                    lines_processed += 15 + 3 * (operation_num - 1)
                    print()

                reading_incomplete = False

    except ser.SerialTimeoutException:
        print('Data could not be read')
    time.sleep(1)
