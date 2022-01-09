/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Simone Cosimo
** Modified date:           27/12/2021
** Version:                 v3.0
** Descriptions:            Special project for ASE course 2021/22
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "CollisionManager.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

// vertical walls
Rect wall1 = { 0, 0, 5, 275, V_WALL };
Rect wall2 = { MAX_X - 4, 0, 5, 275, V_WALL };
// horizontal walls
Rect wall3 = { 0, 0, MAX_X, 5, H_WALL };
// gameover object
Rect gameover = { 0, MAX_Y - 30, MAX_X, 29, GAME_OVER };

volatile uint16_t score = 0;
volatile uint8_t isGameOver = 0;
volatile uint16_t bestScore = 100;
volatile uint8_t gameStarted = 0;
volatile uint8_t isPaused = 0;
volatile uint8_t toReset = 0;
volatile uint8_t toDraw = 0;
volatile uint8_t newPaddleX = MAX_X/2 - 18;

int main(void)
{
	int i = 0;
	int counter = 0;
	char score_in_text[5] = "";
	char best_score_text[22] = "";
	
	// paddle and ball with angle and speed
	Rect ball = { MAX_X - 12, 160, 5, 5, BALL, -2, 2};
	Rect paddle = { MAX_X/2 - 5, MAX_Y - 42, 10, 10, PADDLE_CENTER, 1, 0 };
	Rect paddle_left = { MAX_X/2 - 20, MAX_Y - 42, 15, 10, PADDLE_LEFT, 1, 0 };
	Rect paddle_right = { MAX_X/2 + 5, MAX_Y - 42, 15, 10, PADDLE_RIGHT, 1, 0 };
	Rect* walls[4] = { &wall1, &wall2, &wall3, &gameover };
	
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	BUTTON_init();
	ADC_init();
  LCD_Initialization();
	init_RIT(0x004C4B40);
	enable_RIT();
	init_timer(0, 0x0003D090);						// 10ms
	
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	while(1) {
		// reset stats
		counter = 0;
		score = 0;
		gameStarted = 0;
		isGameOver = 0;
		isPaused = 0;
		toReset = 0;
		toDraw = 0;
		
		ball.x = MAX_X - 12;
		ball.y = 160;
		ball.x_velocity = -1;
		ball.y_velocity = 1;
		paddle.x = MAX_X/2 - 5;
		paddle_left.x = MAX_X/2 - 20;
		paddle_right.x = MAX_X/2 + 5;
		
		LCD_Clear(Black);
		for(i = 0; i < 4; i++) {
			drawRect(*(walls[i]), Red);
		}
		drawRect(ball, Green);
		drawRect(paddle, Green);
		drawRect(paddle_left, Green);
		drawRect(paddle_right, Green);
		
		// score text
		sprintf(score_in_text, "%d", score);
		GUI_Text(10, 160, (uint8_t *) score_in_text, White, Black);
		
		sprintf(best_score_text, "%d", bestScore);
		GUI_Text(MAX_X - 35, 10, (uint8_t *) best_score_text, White, Black);
		
		// stalling until the button to start the game is pressed
		while(!gameStarted) {}
		
		while (!isGameOver)	{
			enable_timer(0);
			while(isPaused) {}
			if(toDraw) {
				disable_timer(0);
				reset_timer(0);
				if(!rectCollision(&ball, paddle) && !rectCollision(&ball, paddle_left) && !rectCollision(&ball, paddle_right)) {
					for(i = 0; i < 4; i++) {
						if(rectCollision(&ball, *(walls[i]))) {
							//sprintf(score_in_text, "%d", score);
							GUI_Text(10, 160, (uint8_t *) score_in_text, White, Black);
							GUI_Text(MAX_X - 35, 10, (uint8_t *) best_score_text, White, Black);
							break;
						}
					}
				} else {
					if(score < 100) score += 5;
					else score += 10;
					sprintf(score_in_text, "%d", score);
					GUI_Text(10, 160, (uint8_t *) score_in_text, White, Black);
				}
				clearRect(&ball, 1);
				drawRect(ball, Green);
				
				// update paddle position only if new x pos clears these constraints (deadzone)
				if(newPaddleX < (paddle.x - 5) || newPaddleX > (paddle.x + 5)) {
					if(newPaddleX < (paddle.x - 10) || newPaddleX > (paddle.x + 10)) {
						clearRect(&paddle, 0);
						clearRect(&paddle_right, 0);
						clearRect(&paddle_left, 0);
						paddle.x = newPaddleX;
						paddle_right.x = newPaddleX + 10;
						paddle_left.x = newPaddleX - 15;
						drawRect(paddle, Green);
						drawRect(paddle_left, Green);
						drawRect(paddle_right, Green);
					}
				}
				toDraw = 0;
			}
		}
		
		clearRect(&ball, 0);
		GUI_Text(10, 160, (uint8_t *) "    ", White, Black);
		GUI_Text(85, 144, (uint8_t *) "You Lose", White, Black);
		if(score > bestScore) {
			bestScore = score;
			sprintf(best_score_text, "New best score: %d", bestScore);
			GUI_Text(45, 160, (uint8_t *) best_score_text, White, Black);
		} else {
			sprintf(best_score_text, "Best score: %d", bestScore);
			GUI_Text(55, 160, (uint8_t *) best_score_text, White, Black);
		}
		
		toReset = 0;
		while(!toReset) {}
	}
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
