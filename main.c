/*
 * Voltage Reading
 * Created: 3/21/2026 10:34:43 AM
 * Author : YELVINGM
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#define UART_UBRR_VALUE 12UL

void adc_init() {
	// Reads in 10-bits
	ADCSRA |= (1<<ADPS1) | (1<<ADPS0); // Set ADC prescalar to 8 which is 125kHz with a sample rate of 1MHz

	ADMUX |= (1<<REFS0); // Set ADC reference to AVCC

	// Right adjust ADC result to allow easy 10 bit reading
	ADMUX &= ~(1<<ADLAR);
	
	ADCSRA |= (1<<ADEN); // Enable ADC
}

void uart_init() {
	/*Set Baud rate*/
	UBRR0H = UART_UBRR_VALUE >> 8;
	UBRR0L = UART_UBRR_VALUE;
	/*Frame format: 8 data bits, no parity, 1 stop bit*/
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	/*Enable Transmit and Receive*/
	UCSR0B = (1<< RXEN0)|(1<<TXEN0);
	/*Need to set Double Speed (U2X=1) in order to get 9600 bps. Normal
	speed at 9600 bps causes too high of an error rate. See table in
	data sheet.*/
	UCSR0A = (1<<U2X0);
}

void write_uart(char c[]) {
	do {
		while((UCSR0A&(1<<UDRE0)) == 0); // Wait for Transmit Buffer to Empty
		UDR0 = *c;
		c++;
	}while(*c != '\0');
}

int analog(int channel) {
	ADMUX &= 0xE0;  
	ADMUX |= channel;
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC));
	int reading;
	reading = ADCL | (ADCH<<8);
	return reading;
}

int read_voltage(int channel) {
	int counts = analog(channel);
	int voltage = 5-(5*counts/1024);
	return voltage;
}

int read_angle(int channel) {
	// Read output of port B
	// Add angular displacement for each cycle
	// return angle
}

int read_temp(int channel) {
	// Read counts from thermistor
	// Convert to temperature
	// Return temperature
}

int read_light(int channel) {
	// Get counts from the current->voltage amplifier of the LED
	// Return counts
}

int main(void) {
	char buffer[20];
	sprintf(buffer, "\n\r");
	uart_init();
	adc_init();

	while(1) {
		int voltage_reading = read_voltage(0);
		// int temp_reading_1 = read_temp(1);
		// int temp_reading_2 = read_temp(2);
		// in angle_reading_1 = read_angle(3);
		sprintf(buffer, "\r\n%d V \t %d K \t %d K", voltage_reading);
		write_uart(buffer); 
		_delay_ms(250);
	}
}


