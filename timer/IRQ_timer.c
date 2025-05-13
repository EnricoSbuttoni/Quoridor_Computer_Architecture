/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "lpc17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../quoridor/quoridor.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern int mossat;
volatile int timer=20;
extern int vittoria1;
char t[3];
void TIMER0_IRQHandler (void)
{//l'handler si occupa di aggiornare e stampare il timer, se scade richiama la funzione mossa in modo "speciale" con il valore 1 e forza il cambio turno
	t[2]='\0';
	if(!vittoria1){
	t[1]= '0' + timer%10;
	if(timer==20){
		t[0]='2';
	}else if(timer>=10){
		t[0]='1';
	}
	else{
		t[0]='0';
	}
	GUI_Text(112, 252, (uint8_t *) "   ", White, White);
	GUI_Text(112, 252, (uint8_t *) t, Black, White);
	if(!timer){
		mossat=mossa(1);
		cambio_turno();
		timer=20;
	}else{
  timer--;}
  LPC_TIM0->IR = 1;}			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
