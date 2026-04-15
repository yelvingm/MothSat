# MothSat
MothSat is a final project for EP 391L at Embry-Riddle Aeronautical University, which aims to construct a star-tracking, attitude control testbed using an ATmega644PA embedded system

To use a C file, create a new executable in Atmel Studio, copy the contents of the C file, and paste them in the solution explorer in Atmel.

main.c contains the entire data packet and sends it as a single string over UART.
Data packet consists of temperature between the two sides, angular displacement, and payload current and voltage.
