/*	ENB244 Microcontroller Assignment 2
 *
 *	   Created: 20/10/2013
 *	   Student: Terrance Ockendon
 *	Student Id: n8560641
 *
 */ 
#include "LCD.h"
#include "Graphics.h"
#include "ASCIIFont.h"
#include <stdlib.h>
#include "Menu.h"

// Write a pixel using screen dimensions
void pixel_write(unsigned char x, unsigned char y, unsigned char** LCD_MEM) {
	unsigned char bit = 0x00;
	unsigned char y2 = 0x00;
	y2 = y/8;
	bit |= 1<<(y-y2*8);
	LCDWrite(0, 0x80 | x);  // Column.
	LCDWrite(0, 0x40 | y2);  // Row.  ?
	bit |= LCD_MEM[y2][x];
	LCD_MEM[y2][x] = bit;
	LCDWrite(LCD_D, bit);
	return;
}
// Clears a box of pixels
void pixel_clear(unsigned char x, unsigned char y, unsigned char** LCD_MEM) {
	
	unsigned char bit = 0x00;
	unsigned char y2 = 0x00;

	if (GAME_CHOICE==2) {
	
		y2 = y/8;
		bit |= 1<<(y%8+1);
		bit ^= 0x00;
		LCDWrite(0, 0x80 | x);  // Column.
		LCDWrite(0, 0x40 | y2);  // Row.  ?
		bit &= LCD_MEM[y2][x];
		LCD_MEM[y2][x] = bit;
		LCDWrite(LCD_D, bit);
		
	} else {
	
		y2 = y/8;
		bit |= 1<<(y%8);
		LCDWrite(0, 0x80 | x);  // Column.
		LCDWrite(0, 0x40 | y2);  // Row.  ?
		bit ^= LCD_MEM[y2][x];
		LCD_MEM[y2][x] = bit;
		LCDWrite(LCD_D, bit);
	}
	return;
} 
// Write a box using screen dimensions
void draw_box(unsigned char x,unsigned char y,unsigned char height,unsigned char width,unsigned char** LCD_MEM) {
	int i,j;
	for(j=y;j<(y+height);j++) {
		for(i=x;i<(x+width);i++) {
			pixel_write(i,j,LCD_MEM);	
		}
	}
}
// Clear a box using screen dimensions
void clear_box(unsigned char x,unsigned char y,unsigned char height,unsigned char width,unsigned char** LCD_MEM) {
	int i,j;
	for(j=y;j<(y+height);j++) {
		for(i=x;i<(x+width);i++) {
			pixel_clear(i,j,LCD_MEM);	
		}
	}
}
