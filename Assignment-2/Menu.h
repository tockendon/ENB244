/*	ENB244 Microcontroller Assignment 2
 *
 *	   Created: 20/10/2013
 *	   Student: Terrance Ockendon
 *	Student Id: n8560641
 *
 */ 
#ifndef Menu_H_
#define Menu_H_
// Global variables
extern char GAME_CHOICE;
extern unsigned char GAME_CHANGER;
extern unsigned char GAME_MOVER;
extern unsigned int HUMAN_SCORE;
extern unsigned int AI_SCORE;
extern char END_GAME;
// Menu functions
void game_menu(void);
void game_start(void);
void game_end(void);
void game_reset(void);
void game_speed(void);
void game_led_flash(void);
void game_goto_menu(void);
void game_pause(void);
#endif /* Menu_H_ */
