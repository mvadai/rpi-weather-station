# UV python class
# this class requires the gp2y10 c program

import subprocess
import re
import sys
import time
import settings

class UV:

    def update(self):
        while True:
            output = subprocess.check_output(["./sensors/gp2y10", "1", "2"])
            output = subprocess.check_output(["./sensors/gp2y10", "0"])
            data = re.search("Voltage:\s+([0-9]+)", output)
            if not data:
                time.sleep(1)
                continue
            else:
                break

        self.uv = round(0.01*(float(data.group(1)))*5090/1024, 1)
