import serial
import time
import sys


ser = serial.Serial('COM1', 9600, timeout=0)

reading_incomplete = True

while reading_incomplete:
    try:
            byte = ser.read()

            print(byte)

    except ser.SerialTimeoutException:
        print('Data could not be read')
    time.sleep(1)
