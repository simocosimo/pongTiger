/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               TouchPanel.c
** Descriptions:            The TouchPanel application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "CollisionManager.h"

/* Private define ------------------------------------------------------------*/
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/*******************************************************************************
* Function Name  : drawRect
* Description    : Draw a Rect based on type
* Input          : - r: Rect, color: Color
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void drawRect(Rect r, uint16_t color) {
	
	int i = 0;
	int x = 0;
	switch(r.type) {
		case V_WALL:
			for(i = 0; i < r.width; i++) LCD_DrawLine(r.x + i, r.y, r.x + i, r.y + r.height, color); 
			break;
		
		case PADDLE_CENTER:
		case H_WALL:
			for(i = 0; i < r.height; i++) LCD_DrawLine(r.x, r.y + i, r.x + r.width, r.y + i, color);
			break;
		
		case BALL:
			for(i = 0; i < r.width; i++) {
				for(x = 0; x < r.height; x++) {
					LCD_SetPoint(r.x + i, r.y + x, color);
				}
			}
			break;
				
		default:
			break;
	}
	
}

/*******************************************************************************
* Function Name  : clearRect
* Description    : Clear the rect drawing and updates the position (if Rect is BALL)
* Input          : - r: Rect
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void clearRect(Rect *r) {
	drawRect(*r, Black);
	r->x += r->x_velocity;
	r->y += r->y_velocity;
}

/*******************************************************************************
* Function Name  : rangeIntersect
* Description    : Check for rect range intersections
* Input          : - min0: uint16_t, max0: uint16_t, min1: uint16_t, max1: uint16_t
* Output         : true (1) or false
* Return         : None
* Attention		 : None
*******************************************************************************/
uint8_t rangeIntersect(uint16_t min0, uint16_t max0, uint16_t min1, uint16_t max1) {
	return MAX(min0, max0) >= MIN(min1, max1) && MIN(min0, max0) <= MAX(min1, max1);
}

/*******************************************************************************
* Function Name  : rectIntersect
* Description    : Check for rect collision and change ball velocity
* Input          : - ball: Rect*, r: Rect
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
uint8_t rectCollision(Rect* ball, Rect r) {
	RectType coll = NO_COLL;
	if(rangeIntersect(ball->x, ball->x + ball->width, r.x, r.x + r.width) && 
			rangeIntersect(ball->y, ball->y + ball->height, r.y, r.y + r.height)) {
		coll = r.type;
	}
			
	switch(coll) {
		case V_WALL:
			ball->x_velocity *= -1;
			break;
		
		case H_WALL:
			ball->y_velocity *= -1;
			break;
		
		case PADDLE_CENTER:
			ball->y_velocity *= -1;
			if(score < 100) score += 5;
			else score += 10;
			break;
		
		case GAME_OVER:
			isGameOver = 1;
			break;
		
		default:
			break;
	}
	
	if(coll == NO_COLL) return 0;
	else return 1;
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
