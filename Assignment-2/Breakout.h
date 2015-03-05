/*	ENB244 Microcontroller Assignment 2
 *
 *	   Created: 20/10/2013
 *	   Student: Terrance Ockendon
 *	Student Id: n8560641
 *
 */ 
#ifndef Breakout_H_
#define Breakout_H_

extern unsigned int GAME_SCORE;
extern char PAD3_POS;
extern int BALL_HIT;
extern int GAME_COUNT;
// extern char BALL_DIR_X,BALL_DIR_Y;
// extern char BALL_POS_X,BALL_POS_Y,BALL_COUNT,BALL_ANGLE;
// extern char PAD_POS,PAD2_POS;
extern unsigned char HIGH_CHECK;
// extern unsigned int ROUND_COUNT;
// Pong Functions
void box_memset(unsigned char** BOX_MEM);

void breakout_welcome(unsigned char** LCD_MEM); 
void breakout_start(unsigned char** LCD_MEM, unsigned char** BOX_MEM);
void breakout_next_level(void);

void breakout_draw_pixel(unsigned char x, unsigned char y, unsigned char** BOX_MEM);
void breakout_clear_pixel(unsigned char x, unsigned char y, unsigned char** BOX_MEM);
void breakout_draw_box(unsigned char x,unsigned char y,unsigned char height,unsigned char width,unsigned char** BOX_MEM);
void breakout_clear_box(unsigned char x,unsigned char y,unsigned char height,unsigned char width,unsigned char** BOX_MEM);

void box_create(unsigned char** LCD_MEM, unsigned char** BOX_MEM);
void box_refresh(unsigned char** LCD_MEM, unsigned char** BOX_MEM);
void box_pixel_high(unsigned char x, unsigned char y, unsigned char** BOX_MEM);

void pad3_clr(unsigned char** LCD_MEM);
void pad3_move(void);
void pad3_fill(unsigned char** LCD_MEM);
void pad3_effect(unsigned char** LCD_MEM);

void breakout_ball_move(void);
void breakout_bounds( unsigned char** LCD_MEM, unsigned char** BOX_MEM);
void breakout_effect(unsigned char** LCD_MEM,unsigned char** BOXMem);
void breakout_count(void);
#endif /* Breakout_H_ */
