/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../timer/timer.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int key1 = 0;
volatile int key2 = 0;
volatile int int0 = 0;
extern uint8_t gameStarted;
extern uint8_t isPaused;
extern uint8_t toReset;

void RIT_IRQHandler (void)
{					
			
	/* ADC management */
	ADC_start_conversion();		
	
	// start game
	if(key1 > 1) {
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0) {	// KEY1 pressed
			switch(key1) {
				case 2:
					// key 1 function
					gameStarted = 1;
					break;
				default:
					break;
			}
		} else {
			key1 = 0;
			NVIC_EnableIRQ(EINT1_IRQn);							// Enable interrupt
			LPC_PINCON->PINSEL4		|= (1 << 22);			// Pin selection
		}
	} else {
		if(key1 == 1)
			key1++;
	}
	
	// pause/resume
	if(key2 > 1) {
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0) {	// KEY2 pressed
			switch(key2) {
				case 2:
					// key 2 function
					if(!isPaused) {
						isPaused = 1;
					} else {
						isPaused = 0;
					}
					break;
					
				default:
					break;
			}
		} else {
			key2 = 0;
			NVIC_EnableIRQ(EINT2_IRQn);							// Enable interrupt
			LPC_PINCON->PINSEL4		|= (1 << 24);			// Pin selection
		}
	} else {
		if(key2 == 1)
			key2++;
	}
	
	// restart
	if(int0 > 1) {
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0) {	// KEY2 pressed
			switch(int0) {
				case 2:
					// key 2 function
					toReset = 1;
					break;
				default:
					break;
			}
		} else {
			int0 = 0;
			NVIC_EnableIRQ(EINT0_IRQn);							// Enable interrupt
			LPC_PINCON->PINSEL4		|= (1 << 20);			// Pin selection
		}
	} else {
		if(int0 == 1)
			int0++;
	}
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
