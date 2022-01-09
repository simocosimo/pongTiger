/****************************************Copyright (c)****************************************************
**                                      
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               CollisionManager.h
** Descriptions:            CollisionManager for rectangular shapes
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Simone Cosimo
** Created date:            2021-12-17
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

#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

/* Includes ------------------------------------------------------------------*/
#include <math.h>
#include <stdint.h>
#include "GLCD/GLCD.h" 

/* External variables ---------------------------------------------------------*/
extern volatile uint16_t score;
extern volatile uint8_t isGameOver;

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	V_WALL, H_WALL, BALL, PADDLE_LEFT, PADDLE_CENTER, PADDLE_RIGHT, GAME_OVER, NO_COLL
} RectType;

typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	RectType type;
	
	// variables only for the ball
	float_t x_velocity;
	float_t y_velocity;
} Rect;

/* Private function prototypes -----------------------------------------------*/	
void drawRect(Rect r, uint16_t color);
void clearRect(Rect* r, uint8_t update);
uint8_t rangeIntersect(uint16_t min0, uint16_t max0, uint16_t min1, uint16_t max1);
uint8_t rectCollision(Rect* ball, Rect r);

#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/


