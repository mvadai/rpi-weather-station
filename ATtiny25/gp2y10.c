#define F_CPU 8000000L
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "usitwislave_devices.h"
#include "usitwislave.h"
#include "usitwislave.c"

// pin setup is for TWI / I2C
uint8_t uvh;
uint8_t uvl;
uint8_t dusth;
uint8_t dustl;
uint8_t state = 0;

void blink(int no){
	int i = 0;
	while( i < no ){
		PORTB |= (1 << PB1);
		_delay_ms(200);
		PORTB &= ~(1 << PB1);
		_delay_ms(200);
		i++;
	}
}

void ADC_input(uint8_t pin){
	ADMUX &= ~(1 << MUX3);
	ADMUX &= ~(1 << MUX2);
	ADMUX |= (1 << MUX1);
	if(pin == 3) ADMUX &= ~(1 << MUX0); // adc set to pb4
	if(pin == 2) ADMUX |= (1 << MUX0); // adc set to pb3
	_delay_us(120); // wait for 15 ADC cycles at 125kHz
}
void ADC_enable(uint8_t def_pin){
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // clock divided by 64 125 kHz
	ADCSRA |= (1 << ADATE); // setting auto trigger
	ADCSRB |= (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0); // free running mode - default
	ADMUX |= (0 << REFS0); // ref voltage is Vcc - default
	ADC_input(def_pin); // setting default pin
	ADMUX |= (0 << ADLAR); // using 10 bit resolution - default
	ADCSRA |= (1 << ADEN); // enable ADC

//	ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt 
	ADCSRA |= (1 << ADSC); 
}

void dust_measurement(){
	ADC_input(3);
	PORTB &= ~(1 << PB1);
	_delay_us(280);
	dustl = ADCL;
	dusth = ADCH;
	_delay_us(40);
	PORTB |= (1 << PB1);
	_delay_us(9680);
	state = 2;
}

void uv_measurement(){
	ADC_input(2);
	uvl = ADCL;
	uvh = ADCH;
	state = 4;
}

static void twi_callback(uint8_t input_buffer_length, const uint8_t *input_buffer,
				uint8_t *output_buffer_length, uint8_t *output_buffer)
{
	if(input_buffer_length > 0){
		if(*input_buffer == 1){
			dust_measurement();
		}
		if(*input_buffer == 2){
			uv_measurement();
		}
	}else{ // we're getting a write request from the master
		*output_buffer_length = 8;
		switch(state)
		{
			case(2):{ // dust data high byte
				*output_buffer = dusth;
				state = 1;
				break;
			}
			case(1):{ // dust data low byte
				*output_buffer = dustl;
				state = 0;
				break;
			}
			case(4):{ // uv data high byte
				*output_buffer = uvh;
				state = 3;
				break;
			}
			case(3):{ // uv data low byte
				*output_buffer = uvl;
				state = 0;
				break;
			}
			case(0):{ // sending device id
				*output_buffer = 3;
				break;
			}
		}
	}
}

int main(void) {
	DDRB |= (1 << PB1); // setup pin1 for output
	PORTB |= (1 << PB1); // set pin1 value to 1
	ADC_enable(3);
	usi_twi_slave(0x03, 1, *twi_callback, 0);
	return 0;
}
