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

unsigned int GAME_SCORE;
char PAD3_POS;
int BALL_HIT;
int GAME_COUNT;

char BALL_POS_X,BALL_POS_Y,BALL_COUNT,BALL_ANGLE;

void breakout_welcome(unsigned char** LCD_MEM) {
	
	LCDClear();
	unsigned char *name = "T OCKENDON";
	unsigned char *id = "8650641";
	unsigned char *breakout = "BREAKOUT";

	// Start Screen
	LCDPosition(10,0);
	LCDString(name);
	LCDPosition(17,1);
	LCDString(id);
	LCDPosition(14,3);
	LCDString(breakout);
	HIGH_CHECK = 0;
	// Wait for button press
	while(PIND == 0xFF) {}
	GAME_SCORE=0;
}

void breakout_start(unsigned char** LCD_MEM, unsigned char** BOX_MEM) {

	unsigned char *level_1 = "Level 1";
	unsigned char *level_2 = "Level 2";
	unsigned char *level_3 = "Level 3";
	unsigned char *level_4 = "Level 4";
	unsigned char *level_5 = "Level 5";
	unsigned char *ready = "Ready!";
	unsigned char *set = "Set!";
	unsigned char *go = "Go!!!";	
	unsigned char *count1 = "3 - 2 - 1";
	unsigned char *count2 = "  - 2 - 1";
	unsigned char *count3 = "  -   - 1";
	
	// Set starting positions
	BALL_POS_X = 41;
	BALL_POS_Y = 23+ROUND_COUNT*4;
	BALL_COUNT = 0;
	BALL_ANGLE = 0;
	BALL_DIR_X = 0;
	BALL_DIR_Y = 1;
	GAME_COUNT = 0;	
	// Human
	PAD3_POS = 41;
	//pad3_pos[1] = 46;		
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
	switch(ROUND_COUNT) {
		case 0:
		LCDPosition(17,3);
		LCDString(level_1);
		break;
		case 1:
		LCDPosition(17,3);
		LCDString(level_2);
		break;
		case 2:
		LCDPosition(17,3);
		LCDString(level_3);
		break;
		case 3:
		LCDPosition(17,3);
		LCDString(level_4);
		break;
		case 4:
		LCDPosition(17,3);
		LCDString(level_5);
		break;				
	}				
	_delay_ms(1000);
	// 2
	LCDClear();
	LCDPosition(28,1);
	LCDString(set);
	LCDPosition(10,2);
	LCDString(count2);
	switch(ROUND_COUNT) {
		case 0:
		LCDPosition(17,3);
		LCDString(level_1);
		break;
		case 1:
		LCDPosition(17,3);
		LCDString(level_2);
		break;
		case 2:
		LCDPosition(17,3);
		LCDString(level_3);
		break;
		case 3:
		LCDPosition(17,3);
		LCDString(level_4);
		break;
		case 4:
		LCDPosition(17,3);
		LCDString(level_5);
		break;				
	}
	_delay_ms(1000);
	// 1
	LCDClear();
	LCDPosition(24,1);
	LCDString(go);
	LCDPosition(10,2);
	LCDString(count3);
	switch(ROUND_COUNT) {
		case 0:
		LCDPosition(17,3);
		LCDString(level_1);
		break;
		case 1:
		LCDPosition(17,3);
		LCDString(level_2);
		break;
		case 2:
		LCDPosition(17,3);
		LCDString(level_3);
		break;
		case 3:
		LCDPosition(17,3);
		LCDString(level_4);
		break;
		case 4:
		LCDPosition(17,3);
		LCDString(level_5);
		break;				
	}				
	_delay_ms(1000);
	// Go! 
	LCDClear();
	LCD_MEMset(LCD_MEM);	
	box_memset(BOX_MEM);
	pixel_write(BALL_POS_X, BALL_POS_Y, LCD_MEM);
	pad3_fill(LCD_MEM);
	BALL_HIT = 0;
	GAME_COUNT = 0;	

}

void box_memset(unsigned char** BOX_MEM) {	
	int i,j;
	for(i=0;i<6;i++){
		for(j=0;j<84;j++){
			BOX_MEM[i][j]= 0;
		}
	}
	return;
}

void breakout_draw_pixel(unsigned char x, unsigned char y, unsigned char** BOX_MEM) {
	unsigned char bit = 0x00;
	unsigned char y2 = 0x00;
	y2 = y/8;
	bit |= 1<<(y-y2*8);
	bit |= BOX_MEM[y2][x];
	BOX_MEM[y2][x] = bit;
	return;
}

void breakout_clear_pixel(unsigned char x, unsigned char y, unsigned char** BOX_MEM) {
  unsigned char bit = 0x00;
  unsigned char y2 = 0x00;
	y2 = y/8;
	bit |= 1<<(y-y2*8);
  bit ^= BOX_MEM[y2][x];
  BOX_MEM[y2][x] = bit;
  return;
}

void breakout_draw_box(unsigned char x,unsigned char y,unsigned char height,unsigned char width,unsigned char** BOX_MEM) {
	int i,j;
	for(j=y;j<(y+height);j++) {
		for(i=x;i<(x+width);i++) {
			breakout_draw_pixel(i,j,BOX_MEM);	
		}
	}
	return;
}

void breakout_clear_box(unsigned char x,unsigned char y,unsigned char height,unsigned char width,unsigned char** BOX_MEM) {
	int i,j;
	for(j=y;j<(y+height);j++) {
		for(i=x;i<(x+width);i++) {
			breakout_clear_pixel(i,j,BOX_MEM);	
		}
	}
	return;
}

void box_create(unsigned char** LCD_MEM, unsigned char** BOX_MEM) {
	int i,j;
	ROUND_COUNT*=5;
	ROUND_COUNT+=12;
	for(i = 1; i < 74; i+=12 ) {
		for(j = 1; j < ROUND_COUNT; j+=5) {
			breakout_draw_box( i, j, 4, 10, BOX_MEM);
			draw_box( i, j, 4, 10, LCD_MEM);
		} 
	}
	ROUND_COUNT-=12;
	ROUND_COUNT/=5;	
	return;
}

void box_refresh(unsigned char** LCD_MEM, unsigned char** BOX_MEM) {
	unsigned char bit = 0x00;
	int i,j;
	for(i=0;i<6;i++){
		for(j=0;j<84;j++){
			LCD_MEM[i][j]=BOX_MEM[i][j];
			LCD_MEM[i][j] = bit;
			LCDWrite(0, 0x80 | j);  // Column.
			LCDWrite(0, 0x40 | i);  // Row.  ?
			LCDWrite(LCD_D, bit);
		}
	}
	return;
}

void box_pixel_high(unsigned char x, unsigned char y, unsigned char** BOX_MEM) {
	HIGH_CHECK=0;
	unsigned char bit = 0x00;
	unsigned char y2 = 0x00;
	if(BALL_POS_X<82&&BALL_POS_X>1) {
		y2 = y/8;
		bit |= 1<<(y-y2*8);
		bit &= (BOX_MEM[y2][x] & 1<<(y-y2*8));
		if(bit >0) {
			HIGH_CHECK = 1;
		}
	}
	return;
}

void pad3_clr(unsigned char** LCD_MEM) {
	clear_box(PAD3_POS-6,46,2,14,LCD_MEM);
}

void pad3_move(void) {
	switch(PIND) {		
		// pin D7 low
		case 0x7F:		
			if(PAD3_POS < 76) {
				PAD3_POS++;
			}
		break;	
		// pin D6 low	
		case 0xBF:
			if(PAD3_POS > 6) {
				PAD3_POS--;
			}
		break;	
		default:;
	}	
}

void pad3_fill(unsigned char** LCD_MEM) {
	draw_box(PAD3_POS-6,46,2,14,LCD_MEM);
}

void pad3_effect(unsigned char** LCD_MEM) {
	pad3_clr(LCD_MEM);
	pad3_move();
	pad3_fill(LCD_MEM);
}

void breakout_ball_move(void) {
		char x = 0;
		char y = 0;
		switch(BALL_ANGLE) {
			//move right @0 deg
			case 0:
				y++;				
				x=0;
			break;
			//move right @+30 deg
			case 60:			
				x++;				
				if(BALL_COUNT > 1) {
					y++;
					BALL_COUNT=0;
				} else {
					BALL_COUNT++;
				}					
			break;
			// move right @+45 deg			
			case 45:
				x++;
				y++;		
			break;
			// move right @+60 deg			
			case 30:
				y++;
				if(BALL_COUNT > 1) {
					x++;
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
}

void breakout_bounds( unsigned char** LCD_MEM, unsigned char** BOX_MEM) {
	int diff = 0;

	int traj = 0;
	char x,y;
	unsigned char y2 = 0;
	unsigned char x2 = 0;
	// change pong x direction if hit pong
	if(BALL_POS_Y > 44) {
		diff = PAD3_POS-BALL_POS_X;
		switch(diff) {		 
		 case 8: BALL_DIR_X = -1; BALL_DIR_Y=-1; BALL_ANGLE=60; break;
		 case 7: BALL_DIR_X = -1; BALL_DIR_Y=-1; BALL_ANGLE=60; break;
		 case 6: BALL_DIR_X = -1; BALL_DIR_Y=-1; BALL_ANGLE=45; break;
		 case 5: BALL_DIR_X = -1; BALL_DIR_Y=-1; BALL_ANGLE=45; break;
		 case 4: BALL_DIR_X = -1; BALL_DIR_Y=-1; BALL_ANGLE=30; break;
		 case 3: BALL_DIR_X = -1; BALL_DIR_Y=-1; BALL_ANGLE=30; break;
		 case 2: BALL_DIR_X = -1; BALL_DIR_Y=-1; BALL_ANGLE=30; break;
		 case 1: BALL_DIR_X = 0; BALL_DIR_Y=-1; BALL_ANGLE=0; break;
		 case 0: BALL_DIR_X = 0; BALL_DIR_Y=-1; BALL_ANGLE=0; break;
		case -1: BALL_DIR_X = 0; BALL_DIR_Y=-1; BALL_ANGLE=0; break;
		case -2: BALL_DIR_X = 0; BALL_DIR_Y=-1; BALL_ANGLE=0; break;
		case -3: BALL_DIR_X = 1; BALL_DIR_Y=-1; BALL_ANGLE=30; break;
		case -4: BALL_DIR_X = 1; BALL_DIR_Y=-1; BALL_ANGLE=30; break;
		case -5: BALL_DIR_X = 1; BALL_DIR_Y=-1; BALL_ANGLE=30; break;
		case -6: BALL_DIR_X = 1; BALL_DIR_Y=-1; BALL_ANGLE=45; break;
		case -7: BALL_DIR_X = 1; BALL_DIR_Y=-1; BALL_ANGLE=45; break;
		case -8: BALL_DIR_X = 1; BALL_DIR_Y=-1; BALL_ANGLE=60; break;
		default:;		
		}
	}
	// change pong x direction if hit horizontal boundary
	if(BALL_POS_X < 2 ) {
		BALL_DIR_X*=-1;
	} else if (BALL_POS_X > 81 ) {
		BALL_DIR_X*=-1;
	}
	// change pong y direction if hit vertical boundary
	if(BALL_POS_Y < 3 ) {
		BALL_DIR_Y*=-1;
	} else if (BALL_POS_Y >= 46 ) {
		//BALL_DIR_Y*=-1;
		END_GAME = 1;
	}
	// think of traj like the quadrands of the unit circle
	if(BALL_DIR_X>0&&BALL_DIR_Y<0) {
		traj = 1;
	} else if(BALL_DIR_X<0&&BALL_DIR_Y<0) {
		traj = 2;
	} else if(BALL_DIR_X<0&&BALL_DIR_Y>0) {
		traj = 3;
	} else if(BALL_DIR_X>0&&BALL_DIR_Y>0) {
		traj = 4;
	} else if(BALL_DIR_X!=0&&BALL_DIR_Y==0) {
		traj = 5;		
	} else if(BALL_DIR_X==0&&BALL_DIR_Y!=0) {
		traj = 6;
		
	}
	// box breaker check and rebound
	x=BALL_POS_X;
	y=BALL_POS_Y;	
	box_pixel_high(x, y, BOX_MEM);
	if(HIGH_CHECK > 0) {
		// remove respective box
		x2 = ((BALL_POS_X)/12)*12+1;
		y2 = ((BALL_POS_Y)/5)*5+1;
		//while(PIND != 0xFE) {}
		clear_box(x2, y2, 4, 10, LCD_MEM);
		breakout_clear_box(x2, y2, 4, 10, BOX_MEM);		
		// deflect ball depending on angle
		switch(traj) {
			case 1:
				BALL_DIR_X*=1;
				BALL_DIR_Y*=1;
			break;
			case 2:
				BALL_DIR_X*=-1;
				BALL_DIR_Y*=1;
			break;
			case 3:
				BALL_DIR_X*=-1;
				BALL_DIR_Y*=-1;
			break;
			case 4:
				BALL_DIR_X*=1;
				BALL_DIR_Y*=-1;
			break;
			case 5:
				BALL_DIR_X*=-1;
			break;
			case 6:
				BALL_DIR_Y*=-1;
			break;
			default:;
		}
		GAME_SCORE++;
		BALL_HIT++;
		if(BALL_HIT>((ROUND_COUNT-1)*7)+20) {
			END_GAME=2;
		}
	}			
}

void breakout_effect(unsigned char** LCD_MEM,unsigned char** BOX_MEM) {
	pixel_clear(BALL_POS_X,BALL_POS_Y, LCD_MEM);
	breakout_ball_move();
	breakout_bounds(LCD_MEM, BOX_MEM); 	
	pixel_write(BALL_POS_X, BALL_POS_Y, LCD_MEM);
}

void breakout_count(void) {
	if(PIND == 0xFE) {
		END_GAME=2;
	}
	_delay_ms(30);
	if(GAME_COUNT > 321) {
		if(GAME_SCORE > 0) {
			GAME_SCORE--;
		}
		GAME_COUNT=0;
	} else {
		GAME_COUNT++;
	}
}

void breakout_next_level(void) {

	unsigned char *level = "Loading Next";
	unsigned char *level2 = "Level";
	unsigned char *score = "Score:";
	unsigned char *win = "You Win! :D";
	unsigned char *lose = "You Lose :(";
	ROUND_COUNT++;
	
	if(ROUND_COUNT > 4) {
	// Win screen
		if(END_GAME == 2) {
			LCDClear();
			GAME_SCORE+=(ROUND_COUNT)*7;
			GAME_SCORE+=36;
			LCDClear();
			LCDPosition(3,1);
			LCDString(win);
			LCDPosition(21,2);
			LCDString(score);
			LCDNumber(GAME_SCORE, 3);
			_delay_ms(2000);
		} else if(END_GAME == 1) {		
			LCDClear();
			LCDPosition(3,1);
			LCDString(lose);
			LCDPosition(21,2);
			LCDString(score);
			LCDNumber(GAME_SCORE, 3);
			_delay_ms(2000);
			ROUND_COUNT=5;
			_delay_ms(2000);
		}	
		game_led_flash();	
	// Lose screen			
	} else if(END_GAME == 2) {
	// Changing levels screen	
		LCDClear();
		GAME_SCORE+=(ROUND_COUNT-1)*7;
		GAME_SCORE+=36;
		//ROUND_COUNT++;
		LCDClear();
		LCDPosition(0,0);
		LCDString(level);
		LCDPosition(24,1);
		LCDString(level2);
		LCDPosition(21,3);
		LCDString(score);
		LCDNumber(GAME_SCORE, 4);
		_delay_ms(1000);
		END_GAME = 0;		
	} 
	if(END_GAME == 1) {		
		LCDClear();
		LCDPosition(3,1);
		LCDString(lose);
		LCDPosition(21,2);
		LCDString(score);
		LCDNumber(GAME_SCORE, 3);
		ROUND_COUNT=5;
		_delay_ms(2000);
		game_led_flash();
	}
}
