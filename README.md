rpi-weather-station
===================

Prerequisites:
- for the client code on your Raspberry you'll have to have wiringPi I2C library installed: https://projects.drogon.net/raspberry-pi/wiringpi/i2c-library/
- for the ATtiny25 to communicate over I2C in a slave mode you'll need the files from here: https://github.com/eriksl/usitwislave


The workings are explained here: http://mihalysprojects.weebly.com/blog/making-a-weather-station-using-the-raspberry-pi-i2c

Example compilation for the Raspberry Pi:
gcc -Wall -std=gnu99 MV_bmp085.c -o MV_bmp085 -l wiringPi

The following post explains how to set the fuses on the ATtiny25 and how to program it to communicate with the Raspberry Pi over I2C:
http://mihalysprojects.weebly.com/blog/setting-up-i2c-between-an-attiny-tiny-avr-and-a-raspberry-pi

License: GPLv2 Mihaly Vadai 2014
