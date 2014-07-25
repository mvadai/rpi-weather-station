# Written by: Mihaly Vadai
# simple class for the mightyohm Geiger counter over serial connection

import serial
import settings
import string

# As per default the counter sends 4 parameters
# cps: counts per second
# cpm: counts per minute
# dose: dose in uSv/h
# mode: SLOW|FAST|INST
# for description see: http://mightyohm.com/blog/products/geiger-counter/usage-instructions/
# over a 9600 8N1 serial protocol

class MIGEIGER:
    def __init__(self):
        self.port = settings.SERIAL

#    def __init__(self, port):
#        pass

    def update(self):
        ser = serial.Serial(self.port, 9600, timeout=1)
        line = ser.readline()
        ser.close()
        data = string.split(line, ',')
        try:
            self.cps = int(data[1])
            self.cpm = int(data[3])
            self.dose = float(data[5])
            self.mode = data[6][1:][:4]
        except:
            self.cps = 0
            self.cpm = 0
            self.dose = 0
            self.mode = 'NONE'
