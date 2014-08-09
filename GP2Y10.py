# GP2Y10 python class
# this class requires the gp2y10 c program

import subprocess
import re
import sys
import time
import settings

class GP2Y10:

    def update(self):
        cnt = 0
        while cnt < 5:
            output = subprocess.check_output(["./sensors/gp2y10", "1", "1"])
            output = subprocess.check_output(["./sensors/gp2y10", "0"])
            data = re.search("Voltage:\s+([0-9]+)", output)
            if not data:
                time.sleep(1)
                self.dust = -1.0
                cnt += 1
                continue
            else:
                self.dust = ((float(data.group(1)))*5090/1024-500)*1/6000
                break
