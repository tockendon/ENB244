/*	ENB244 Microcontroller Assignment 2
 *
 *	   Created: 20/10/2013
 *	   Student: Terrance Ockendon
 *	Student Id: n8560641
 *
 */ 
#ifndef Graphics_H_
#define Graphics_H_

void pixel_write(unsigned char x, unsigned char y, unsigned char** LCD_MEM);
void pixel_clear(unsigned char x, unsigned char y, unsigned char** LCD_MEM);
void draw_box(unsigned char x,unsigned char y,unsigned char height,unsigned char width,unsigned char** LCD_MEM);
void clear_box(unsigned char x,unsigned char y,unsigned char height,unsigned char width,unsigned char** LCD_MEM);
#endif /* Breakout_H_ */
