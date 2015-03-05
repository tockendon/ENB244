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

// Global variables
char GAME_CHOICE=0;
unsigned char GAME_CHANGER=1;
unsigned char GAME_MOVER=2;
unsigned int HUMAN_SCORE=0;
unsigned int AI_SCORE=0;
char END_GAME=0;

void game_menu(void) {
	unsigned char *ch_welcome = "Game Select:";
	unsigned char *ch_game1 = "BREAKOUT";
	unsigned char *ch_game2 = "PONG";
	unsigned char *ch_sel = ">";
	unsigned char *name = "T OCKENDON";
	unsigned char *id = "8650641";
	
	while(GAME_CHOICE == 0) {
	
		LCDClear();
		LCDPosition(7,0);
		LCDString(name);
		LCDPosition(18,1);
		LCDString(id);
		LCDPosition(0,3);
		LCDString(ch_welcome);
		LCDPosition(14,4);
		LCDString(ch_game1);
		LCDPosition(28,5);
		LCDString(ch_game2);
		LCDPosition(0,GAME_MOVER+2);
		LCDString(ch_sel);
		
		if(PIND == 0x7F) {			
			GAME_CHOICE = GAME_MOVER-1;
			
		} else if(PIND == 0xBF) {
			GAME_MOVER+=GAME_CHANGER;
			GAME_CHANGER*=-1;
		}
		_delay_ms(100);
	}
	HUMAN_SCORE=0;
	AI_SCORE=0;
	END_GAME=0;
	ROUND_COUNT=0;
	return;
}

void game_end(void) {
	unsigned char *win = "You Win! :D";
	unsigned char *lose = "You Lose :(";
}

void game_reset(void) {
	GAME_CHOICE=0;
	GAME_CHANGER=1;
	GAME_MOVER=2;
	HUMAN_SCORE=0;
	AI_SCORE=0;
	END_GAME=0;
	ROUND_COUNT=0;
}

void game_speed(void) {
	float speed = 0;
	speed = ADCRead();
	speed /=100;
	speed = floor(speed);
	speed = (speed*2)+3;
	_delay_ms(speed);
}

void game_led_flash(void) {
	while(PIND == 0xFF) {
		DDRD = 0x04;	
		PORTD = 0xFB;
		PORTD |= (1 << PIND2);
		_delay_ms(500);	
		PORTD &= ~(1 << PIND2);
		_delay_ms(500);
		PORTD = 0xFF;
		DDRD = 0x00;			
	}
		DDRD = 0x00;	
		PORTD = 0xFF;	
		LCDClear();
}

void game_goto_menu(void) {
	_delay_ms(40);
	if(PIND == 0x3F) {
		END_GAME = 4;
	}
	_delay_ms(40);
}

void game_pause(void) {
	
	unsigned char unpause = 0;
	
	if(PIND == 0xFE) {
		while(PIND == 0xFE) {};
		while((unpause == 0) && (END_GAME != 4)) {			
			if(PIND == 0x3F) {
				END_GAME = 4;
			}
			if(PIND == 0xFE) {
				unpause = 1;
			}
			_delay_ms(100);
		}
	}
}
