/*
 * lab14_yelvington.c
 *
 * Created: 4/1/2026 9:15:23 AM
 * Author : YELVINGM
 */ 
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#define UART_UBRR_VALUE 12UL
#define F_CPU 1000000UL

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

int main() {
	// Cole put the stepping sequences here and uncomment when you're done. The ones below don't work
	
	DDRB |= 0x0F; // Set Register B PINS 0-3 High, i.e. configure as output
	int wave_steps[4]={0x01, 0x04, 0x08, 0x02}; // Set the stepping sequence of the stepper motor 
	int full_steps[4]={0x03, 0x06, 0x0C, 0x09};
	int half_steps[8]={0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};
	int i=0; // Initialize variable i
	int cycles = 0;
	
	while(1) // Run the stepper motor forever
	{
	 PORTB &= 0xF0; // Turn off the stepper motor only
	 _delay_ms(50); // Delay the time between turning off and on the motor
	 PORTB |= wave_steps[i]; // Uncomment for Wave Mode
	 // PORTB |= full_steps[i]; // Uncomment for Full Step Mode
	 // PORTB |= half_steps[i]; // Uncomment for Half Step Mode
	 _delay_ms(50); // Delay the time between the step and shutting off the motor
	 i=(i+1)%4; // Setting the modulus so the stepping sequence repeats forever
	 
	 // Add to number of cycles  
	 cycles++;
	 
	 // Sends Angular Displacement over UART
	 char buffer[20];
	 sprintf(buffer, "\n\r");
	 uart_init();
	 int disp = 1.8*cycles;
	 sprintf(buffer, "\r\n%d degrees", disp);
	 write_uart(buffer);
	 _delay_ms(250);
	}
}