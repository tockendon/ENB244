/*	ENB244 Microcontroller Assignment 2
 *
 *	   Created: 20/10/2013
 *	   Student: Terrance Ockendon
 *	Student Id: n8560641
 *
 */
 
/*
 * LCD.c
 *
 * Created: 32/13/2015 12:34:56 AM
 *  Author: Michael
 */ 
#include "LCD.h"
#include <avr/io.h>
// Include our font pixel data
#include "ASCIIFont.h"
#include <stdlib.h>

void SPIInit(void) {
	// Set MOSI and SCK as outputs for the SPI module
	SET_OUTPUT(DDRB, MOSIPIN);
	SET_OUTPUT(DDRB, SCKPIN);
	SET_OUTPUT(DDRB, SCEPIN);
	
	// Enable SPI in Master mode, leave the clock speed as fclk/4 (Refer to datasheet for SPCR register)
	SPCR = (1<<SPE)|(1<<MSTR);
	
}
void SPIWriteByte(unsigned char byte) {
	// Write our byte to the SPI data register
	SPDR = byte;
	
	// Wait until the SPI module signals that transmission is finished
	while(!(SPSR & (1<<SPIF)));
}
void LCDWrite(unsigned char dc, unsigned char data) {
	// Set the DC pin based on the parameter 'dc' (Hint: use the OUTPUT_WRITE macro)
	OUTPUT_WRITE(PORTB, DCPIN, dc);
	// Pull the SCE/SS pin low to signal the LCD we have data
	OUTPUT_LOW(PORTB, SCEPIN);
	
	// Write the byte of data using SPI
	SPIWriteByte(data);
	
	// Pull SCE/SS high to signal the LCD we are done
	OUTPUT_HIGH(PORTB, SCEPIN);
}
// Initialise the LCD with our desired settings
void LCDInitialise(unsigned char contrast) {
	SPIInit();
	
	SET_OUTPUT(DDRB, DCPIN);
	SET_OUTPUT(DDRB, RSTPIN);
  
	OUTPUT_LOW(PORTB, RSTPIN);
	OUTPUT_HIGH(PORTB, RSTPIN);
  
	LCDWrite(LCD_C, 0x21 ); // Enable LCD extended command set
	LCDWrite(LCD_C, 0x80 | contrast ); // Set LCD Vop (Contrast)
	LCDWrite(LCD_C, 0x13 ); // LCD bias mode 1:48
  
	LCDWrite(LCD_C, 0x20 ); // Enable LCD basic command set
	LCDWrite(LCD_C, 0x0C ); // LCD in normal mode.
  
	LCDWrite(LCD_C, 0x40 ); // Reset row to 0
	LCDWrite(LCD_C, 0x80 ); // Reset column to 0
}
// Simply blank out the memory of the LCD to clear the screen
void LCDClear(void) {
	for (int i = 0; i < LCD_X * LCD_Y / 8; i++) {
		LCDWrite(LCD_D, 0x00);
	}
}
void LCDCharacter(unsigned char character) {
	/*
		Blank pixel column before the character, to increase readability
		Also, our font is only 5 pixels wide, since our LCD is 84 pixels wide
		we can fit twelve 7 pixel wide characters across.
	*/
	LCDWrite(LCD_D, 0x00);
	
	for (int index = 0; index < 5; index++) {
		// Write each of the 5 pixel rows to the LCD, we subtract 0x20 since
		// our table doesn't have the unprintable ASCII characters (...)
		LCDWrite(LCD_D, ASCII[character - 0x20][index]);
	}
	
	LCDWrite(LCD_D, 0x00);
}
// Write an interger number to the screen
void LCDNumber(unsigned int number, unsigned char y) {
//max number is 32000
	char buffer[100];
	unsigned char x;
	if(number < 10) {
		x=38;
	} else if(number < 100) {
		x=34;
	} else if(number < 1000) {
		x=31;
	} else if(number < 10000) {
		x=27;
	} else if(number < 32001) {
		x=23;
	}
	//int xcon;
		itoa (number, buffer, 10);
		unsigned char *ucbuffer = (unsigned char*)&buffer[0];
		LCDWrite(0, 0x80 | x);  // Column.	
		LCDWrite(0, 0x40 | y);  // Row.  ?		
		// Write a number
		LCDString(ucbuffer);
}
// Iterate over the string, sending each character one at a time
void LCDString(unsigned char *characters) {
	int i = 0;
	while (characters[i]!='\0'){
		LCDCharacter(characters[i]);
		i++;
		}
}
// Set the position in the LCD's memory that we want to begin writing from
void LCDPosition(unsigned char x, unsigned char y) {
	LCDWrite(0, 0x80 | x);  // Column.
	LCDWrite(0, 0x40 | y);  // Row.  ?
}
// Create LCD memory
void LCD_MEMset(unsigned char** LCD_MEM) {	
	int i,j;
	for(i=0;i<6;i++){
		for(j=0;j<84;j++){
			LCD_MEM[i][j]= 0;
		}
	}
	return;
}
