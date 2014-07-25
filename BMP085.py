# BMP 085 python class
# this class requires the MV_bmp058 c program

import subprocess
import re
import sys
import time
import settings

class BMP085:
    def __init__(self, altitude):
        self.altitude = altitude;

    def update(self):
        while True:
            output = subprocess.check_output(["./sensors/MV_bmp085", str(self.altitude)])
            data = re.search("Temperature:\s+([0-9.]+)", output)
            if not data:
                time.sleep(1)
                continue
            else:
                break

        self.temp = float(data.group(1))
        data = re.search("Pressure:\s+([0-9]+)", output)
        self.pressure = int(data.group(1))
        data = re.search("level:\s+([0-9]+)", output)
        self.pressure_sea = int(data.group(1))
