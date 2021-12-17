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
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "CollisionManager.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif

// vertical walls
Rect wall1 = { 0, 0, 5, 280, V_WALL };
Rect wall2 = { MAX_X - 5, 0, 5, 280, V_WALL };
// horizontal walls
Rect wall3 = { 0, 0, MAX_X, 5, H_WALL };
//Rect wall4 = { 0, 280 - 4, MAX_X, 5, H_WALL };
// gameover object
Rect gameover = { 0, MAX_Y - 30, MAX_X, 29, GAME_OVER };

volatile uint16_t score = 0;
volatile uint8_t isGameOver = 0;

int main(void)
{
	int i = 0;
	char score_in_text[5] = "";
	
	// paddle and ball with angle and speed
	Rect ball = { MAX_X - 12, 160, 5, 5, BALL, 45.0, -1, 1};
	Rect paddle = { MAX_X/2 - 18, MAX_Y - 42, 36, 10, PADDLE_CENTER, 0, 1, 0 };
	Rect* walls[4] = { &wall1, &wall2, &wall3, &gameover };
	
  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
	LCD_Clear(Black);
	
	for(i = 0; i < 4; i++) {
		drawRect(*(walls[i]), Red);
	}
	drawRect(ball, Green);
	drawRect(paddle, Green);
	
	// help text
	sprintf(score_in_text, "%d", score);
	GUI_Text(10, 160, (uint8_t *) score_in_text, White, Black);
	
	//init_timer(0, 0x1312D0 ); 						/* 50ms * 25MHz = 1.25*10^6 = 0x1312D0 */
	//init_timer(0, 0x6108 ); 						  /* 1ms * 25MHz = 25*10^3 = 0x6108 */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 */
	//init_timer(0, 0xC8 ); 						    /* 8us * 25MHz = 200 ~= 0xC8 */
	
	//enable_timer(0);
	
  while (!isGameOver)	{
		if(!rectCollision(&ball, paddle)) {
			for(i = 0; i < 4; i++) {
				if(rectCollision(&ball, *(walls[i]))) {
					sprintf(score_in_text, "%d", score);
					GUI_Text(10, 160, (uint8_t *) score_in_text, White, Black);
					break;
				}
			}
		} else {
			sprintf(score_in_text, "%d", score);
			GUI_Text(10, 160, (uint8_t *) score_in_text, White, Black);
		}
		clearRect(&ball);
		drawRect(ball, Green);
  }
	
	clearRect(&ball);
	GUI_Text(10, 160, (uint8_t *) "    ", White, Black);
	GUI_Text(85, 160, (uint8_t *) "You Lose", White, Black);
	
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
