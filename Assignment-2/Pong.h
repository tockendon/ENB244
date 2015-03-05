/*	ENB244 Microcontroller Assignment 2
 *
 *	   Created: 20/10/2013
 *	   Student: Terrance Ockendon
 *	Student Id: n8560641
 *
 */ 
#ifndef Pong_H_
#define Pong_H_
// Global variables
extern char BALL_DIR_X,BALL_DIR_Y;
extern char BALL_POS_X,BALL_POS_Y,BALL_COUNT,BALL_ANGLE;
extern char PAD_POS,PAD2_POS;
extern unsigned char HIGH_CHECK;
extern unsigned char BOUNCE_CHECK;
extern unsigned int ROUND_COUNT;
// Pong Functions
void pong_welcome(unsigned char** LCD_MEM);
void pong_start(unsigned char** LCD_MEM);
void pong_next_level(unsigned char** LCD_MEM);

void ball_move(void);
void ball_bounds(void);
void ball_fill(unsigned char** LCD_MEM);
void ball_clear(unsigned char** LCD_MEM);
void ball_effect(unsigned char** LCD_MEM);

void pad_fill(unsigned char** LCD_MEM);
void pad_clr(unsigned char** LCD_MEM);
void pad_move(void);
void pad_effect(unsigned char** LCD_MEM);

void pad2_fill(unsigned char** LCD_MEM);
void pad2_clr(unsigned char** LCD_MEM);
void pad2_move(void);
void pad2_effect(unsigned char** LCD_MEM);


#endif /* Pong_H_ */
