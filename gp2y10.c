#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <wiringPiI2C.h>
#include <stdlib.h>
//based on measurements
#define OFFSET 5

int main(int argc, char **argv){
  int DEVICE_ADDRESS, data, fd;
  
  DEVICE_ADDRESS = 0x03;

  fd = wiringPiI2CSetup (DEVICE_ADDRESS);
  
  if (atoi(argv[1]) == 1 ) { 
	wiringPiI2CWrite (fd, atoi(argv[2]));
	wiringPiI2CRead (fd); 
	return 0; 
  }
  if (atoi(argv[1]) == 0) { 
//	data = wiringPiI2CRead (fd); // bug in the usitwislave library
	data = (wiringPiI2CRead(fd) << 8) + wiringPiI2CRead(fd);
	if(data != 0) data += OFFSET;
	printf("Voltage: %d \n", data);
 }
  return 0;
}
