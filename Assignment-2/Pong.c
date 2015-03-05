/*	ENB244 Microcontroller Assignment 2
 *
 *	   Created: 20/10/2013
 *	   Student: Terrance Ockendon
 *	Student Id: n8560641
 *
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"
#include "LCD.h"
#include "Graphics.h"
#include "Menu.h"
#include "Pong.h"
#include "Breakout.h"

unsigned int ROUND_COUNT;
char BALL_DIR_X,BALL_DIR_Y;
char BALL_POS_X,BALL_POS_Y,BALL_COUNT,BALL_ANGLE;
char PAD_POS,PAD2_POS;
unsigned char HIGH_CHECK=0;
unsigned char BOUNCE_CHECK = 0;
unsigned int ROUND_COUNT=0;

void pong_welcome(unsigned char** LCD_MEM) {
	
	LCDClear();
	unsigned char *name = "T OCKENDON";
	unsigned char *id = "8650641";
	unsigned char *pong = "PONG";
	
	// Start Screen
	LCDPosition(10,0);
	LCDString(name);
	LCDPosition(17,1);
	LCDString(id);
	LCDPosition(28,3);
	LCDString(pong);	
	draw_box(0,33,14,2,LCD_MEM);
	draw_box(82,33,14,2,LCD_MEM);
	draw_box(41,39,3,3,LCD_MEM);
	// Wait for button press
	while(PIND == 0xFF) {}
	LCD_MEMset(LCD_MEM);
}

void pong_start(unsigned char** LCD_MEM) {
	LCDClear();

	unsigned char *ready = "Ready!";
	unsigned char *set = "Set!";
	unsigned char *go = "Go!!!";	
	unsigned char *count1 = "3 - 2 - 1";
	unsigned char *count2 = "  - 2 - 1";
	unsigned char *count3 = "  -   - 1";
	
	BALL_DIR_X=-1;
	BALL_DIR_Y=0;
	BALL_POS_X=42;
	BALL_POS_Y=23;
	BALL_COUNT=0;
	BALL_ANGLE=0;
	PAD_POS=23;
	PAD2_POS=23;

	// Start count down
	LCDClear();
	LCD_MEMset(LCD_MEM);	
	_delay_ms(500);
	// 3
	LCDClear();
	LCDPosition(21,1);
	LCDString(ready);
	LCDPosition(10,2);
	LCDString(count1);
	_delay_ms(1000);
	// 2
	LCDClear();
	LCDPosition(28,1);
	LCDString(set);
	LCDPosition(10,2);
	LCDString(count2);
	_delay_ms(1000);
	// 1
	LCDClear();
	LCDPosition(24,1);
	LCDString(go);
	LCDPosition(10,2);
	LCDString(count3);
	_delay_ms(1000);
	// Go!
	LCDClear();
}

void pong_next_level(unsigned char** LCD_MEM) {
	unsigned char *win = "You Win! :D";
	unsigned char *lose = "You Lose :(";
	unsigned char *score = "Score:";
	unsigned char *score_you = "You";
	unsigned char *score_ai = "AI";
	LCDClear();
	if(HUMAN_SCORE<3 && AI_SCORE<3) {
	// Win screen
		END_GAME = 0;
		LCDPosition(21,0);
		LCDString(score);
		LCDPosition(31,1);
		LCDString(score_you);
		LCDNumber(HUMAN_SCORE, 2);
		LCDPosition(35,3);
		LCDString(score_ai);
		LCDNumber(AI_SCORE, 4);		
	// Lose screen
	} else if(HUMAN_SCORE==3) {
		END_GAME=2;
		LCDPosition(3,0);
		LCDString(win);
		LCDPosition(31,1);
		LCDString(score_you);
		LCDNumber(HUMAN_SCORE, 2);
		LCDPosition(35,3);
		LCDString(score_ai);
		LCDNumber(AI_SCORE, 4);		
	// Changing levels screen	
	} else if(AI_SCORE==3) {
		END_GAME=2;
		LCDPosition(0,0);
		LCDString(lose);
		LCDPosition(31,1);
		LCDString(score_you);
		LCDNumber(HUMAN_SCORE, 2);
		LCDPosition(35,3);
		LCDString(score_ai);
		LCDNumber(AI_SCORE, 4);			
	} else {
		END_GAME = 0;
	}
	game_led_flash();
	LCD_MEMset(LCD_MEM);
}

void ball_move(void) {
		char x = 0;
		char y = 0;
		switch(BALL_ANGLE) {
			//move right @0 deg
			case 0:
				x++;				
				y=0;
			break;
			//move right @+30 deg
			case 30:			
				x++;				
				if(BALL_COUNT > 1) {
					--y;
					BALL_COUNT=0;
				} else {
					BALL_COUNT++;
				}					
			break;
			// move right @+45 deg			
			case 45:
				x++;
				y--;		
			break;
			// move right @+60 deg			
			case 60:
				y--;
				if(BALL_COUNT > 1) {
					++x;
					BALL_COUNT=0;
				} else {
					BALL_COUNT++;
				}
			break;				
			default:;				
		} 
		x*=BALL_DIR_X;
		y*=BALL_DIR_Y;
		BALL_POS_X+=x;
		BALL_POS_Y+=y;		
		return;
}

void ball_bounds(void) {
	int diff = 0;
	// change pong x direction if hit pong
	if(BALL_POS_X < 3) {
		diff = PAD_POS-BALL_POS_Y;
		switch(diff) {
		case -7: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=60; break;
		case -6: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=45; break;
		case -5: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=45; break;
		case -4: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
		case -3: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
		case -2: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
		case -1: BALL_DIR_Y = 0; BALL_DIR_X*=-1; BALL_ANGLE=0; break;
		case -0: BALL_DIR_Y = 0; BALL_DIR_X*=-1; BALL_ANGLE=0; break;
		case 1:	BALL_DIR_Y = 0; BALL_DIR_X*=-1; BALL_ANGLE=0; break;
		case 2: BALL_DIR_Y = 0; BALL_DIR_X*=-1; BALL_ANGLE=0; break;
		case 3: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
		case 4: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
		case 5: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
		case 6: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=45; break;
		case 7: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=45; break;
		case 8: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=60; break;
		default:;		
		}
	}
	if(BOUNCE_CHECK == 0) {
		if(BALL_POS_X > 79) {
			diff = PAD2_POS-BALL_POS_Y;
			switch(diff) {
			case -7: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=60; break;
			case -6: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=45; break;
			case -5: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=45; break;
			case -4: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
			case -3: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
			case -2: BALL_DIR_Y = -1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
			case -1: BALL_DIR_Y = 0; BALL_DIR_X*=-1; BALL_ANGLE=0; break;
			case -0: BALL_DIR_Y = 0; BALL_DIR_X*=-1; BALL_ANGLE=0; break;
			case 1:	BALL_DIR_Y = 0; BALL_DIR_X*=-1; BALL_ANGLE=0; break;
			case 2: BALL_DIR_Y = 0; BALL_DIR_X*=-1; BALL_ANGLE=0; break;
			case 3: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
			case 4: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
			case 5: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=30; break;
			case 6: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=45; break;
			case 7: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=45; break;
			case 8: BALL_DIR_Y = 1; BALL_DIR_X*=-1; BALL_ANGLE=60; break;
			default:;		
			}
			BOUNCE_CHECK=5;
		}
	} else {
		BOUNCE_CHECK--;
	}
	// change pong x direction if hit horizontal boundary
	if(BALL_POS_X < 1 ) {
		//BALL_DIR_X*=-1;
		AI_SCORE+=1;
		END_GAME = 1;
	} else if (BALL_POS_X > 82 ) {
		//BALL_DIR_X*=-1;
		HUMAN_SCORE+=1;
		END_GAME = 2;
	}
	// change pong y direction if hit vertical boundary
	if(BALL_POS_Y < 2 ) {
		BALL_DIR_Y*=-1;
	} else if (BALL_POS_Y > 45 ) {
		BALL_DIR_Y*=-1;
	}
}

void ball_fill(unsigned char** LCD_MEM) {
	draw_box(BALL_POS_X-1,BALL_POS_Y-1,3,3,LCD_MEM);
}

void ball_clear(unsigned char** LCD_MEM) {
	clear_box(BALL_POS_X-1,BALL_POS_Y-1,3,3,LCD_MEM);
}

void ball_effect(unsigned char** LCD_MEM) {
	ball_clear(LCD_MEM);
	ball_bounds(); 
	ball_move();
	ball_fill(LCD_MEM);
}

void pad_fill(unsigned char** LCD_MEM) {
	draw_box(0,PAD_POS-6,14,2,LCD_MEM);
}

void pad_clr(unsigned char** LCD_MEM) {
	clear_box(0,PAD_POS-6,14,2,LCD_MEM);
}

void pad2_fill(unsigned char** LCD_MEM){
	draw_box(82,PAD2_POS-6,14,2,LCD_MEM);
}

void pad2_clr(unsigned char** LCD_MEM) {
	clear_box(82,PAD2_POS-6,14,2,LCD_MEM);
}

void pad_move(void) {

	switch(PIND) {		
		// pin D7 low
		case 0x7F:		
			if(PAD_POS < 40) {
				PAD_POS++;
			}
		break;	
		// pin D6 low	
		case 0xBF:
			if(PAD_POS > 6) {
				PAD_POS--;
			}
		break;	
		default:;
	}		
}

void pad2_move(void) {
	if(BALL_POS_X > 63 ) {
		if((PAD2_POS) < BALL_POS_Y) {
			if(PAD2_POS < 40) {				
				PAD2_POS++;	
			}
		} else if((PAD2_POS)  > BALL_POS_Y) {
			if(PAD2_POS > 6) {
				PAD2_POS--;
			}
		} else if(PAD2_POS == BALL_POS_Y) {
		
		}			
	}
}

void pad_effect(unsigned char** LCD_MEM) {
	pad_clr(LCD_MEM);
	pad_move();
	pad_fill(LCD_MEM);
}

void pad2_effect(unsigned char** LCD_MEM) {
	pad2_clr(LCD_MEM);
	pad2_move();
	pad2_fill(LCD_MEM);
}

