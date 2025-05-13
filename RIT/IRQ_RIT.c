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
#include "../quoridor/quoridor.h"
#include "../GLCD/GLCD.h"
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


extern int mossat;
extern char led_value;
volatile int mode=0; //mode =0 mossa della pedina, 1 muro
int verso=1; //verso =0 orizzontale, 1 verticale
volatile int x_Mu=0; //posizione muro, inizialmente orizzontale
volatile int y_Mu=1; //posizione y_Mu
int prec=0;
extern int vittoria1;
volatile int down_0;
volatile int down_1;
volatile int down_2;
volatile int avvio=0;
extern int x1,y1,x2,y2;
void RIT_IRQHandler (void)

{					
	char t[1];
	int mex;
	static int select = 0;
	static int J_down_1 = 0;
	static int J_Up = 0;
	static int pointer = 0;
	int RES;
	static int J_left = 0;
	static int J_right = 0;
	
	if(avvio&&!vittoria1){
	
		//i joystick funzionano tutti nella stessa maniera, nel caso in cui mode sia 1 fanno i check  sui muri per controllare il loro piazzamento
		//nel caso del movimento della pedina controllano se la mossa è valida tramite la variabile mex
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
		/* Joytick J_Select pressed p1.25*/
		/* Joytick J_down_1 pressed p1.26 --> using J_down_1 due to emulator issues*/
		
		J_down_1++;
		switch(J_down_1){
			case 1:
				if(mode){
					if(verso){// verticale
						if(prova_muro_ver(1,1,x_Mu,y_Mu)){
							y_Mu=y_Mu+2;
						}
					}else{
					if(prova_muro_or(1,1,x_Mu,y_Mu)){
						y_Mu=y_Mu+2;
					}}
				}
				else{ //mossa pedina
				mex=mossa_valida(1);
				if(!mex){
				}else{
					if(mex==2){
					mossa_in(1,1);
				}else{
					mossa_in(1,0);}}}
				break;
			default:
				break;
		}
	}
	else{
			J_down_1=0;
	}
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick down_1 pressed p1.27 --> using J_left due to emulator issues*/
		
		J_left++;
			switch(J_left){
				case 1:
					if(mode){
					if(verso){// verticale
						if(prova_muro_ver(1,3,x_Mu,y_Mu)){
							x_Mu=x_Mu-2;
						}
					}else{
					if(prova_muro_or(1,3,x_Mu,y_Mu)){
						x_Mu=x_Mu-2;
					}}
				}
				else{
				mex=mossa_valida(3);
				if(!mex){
				}else{
					if(mex==2){
					mossa_in(3,1);
				}else{
					mossa_in(3,0);}}
				}
				break;
			default:
				break;
		}
	}
	else{
			J_left=0;
	}

	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){	
		/* Joytick Select pressed p1.25*/
		/* Joytick down_1 pressed p1.28 --> using J_right */
		
		J_right++;
		switch(J_right){
			case 1:
						
				if(mode){
		
					if(verso){// verticale
						if(prova_muro_ver(1,2,x_Mu,y_Mu)){
							x_Mu=x_Mu+2;
						}
					}else{
					if(prova_muro_or(1,2,x_Mu,y_Mu)){
						x_Mu=x_Mu+2;
					}}
				}
				else{
					mex=mossa_valida(2);
			if(!mex){
			}
			else{
				if(mex==2){
					mossa_in(2,1);
				}
				else{
				mossa_in(2,0);}}}
			
		break;
			default:
				break;
		}
	}
	else{
			J_right=0;
	}

	
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0 ){	
		/* Joytick Up pressed p1.29 --> using J_Up */
	// SUUUUUU 
		J_Up++;
		switch(J_Up){
			case 1:
				if(mode){
						if(verso){// verticale
						if(prova_muro_ver(1,0,x_Mu,y_Mu)){
							y_Mu=y_Mu-2;
						}
					}else{
					if(prova_muro_or(1,0,x_Mu,y_Mu)){
						y_Mu=y_Mu-2;
					}}
				}
				else{
			mex=mossa_valida(0);
			if(!mex){
			}
			else{
				if(mex==2){
					mossa_in(0,1);
				}
				else{
				mossa_in(0,0);}
			}
		}
				break;
				
			default:
				break;
		}
	}
	else{
			J_Up=0;
	}
	
	
if((LPC_GPIO1->FIOPIN & (1<<25)) == 0 ){	//il tasto select serve per confermare il piazzamento del muro
	//controlla attraverso le funzioni ricorsive se è raggiungibile la vittoria per entrambe le pedine
		

		select++;
		switch(select){
			case 1:
				if(mode) {
					if(muri_finiti()){
					if(funzione_ricorsiva1(x1,y1)&&funzione_ricorsiva2(x2,y2)){
						if(check(x_Mu,y_Mu)){
						mossat=mossa(0);
						reset_caselle();
						diminuisci_muri();
						mode=!mode;
					}
					}
					else{
					reset_caselle();}
				}}
				
				break;
				
			default:
				break;
		}
	}
	else{
			select=0;
	}
	/* button management */
	
	if(down_1!=0){ //passaggio dalla mode 0 (pedina) alla mode 1 (muro)
		if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){	/* KEY1 pressed */
			down_1++;				
			switch(down_1){
				case 2:
						mode=!mode;
						if(mode==1){
							colora_possibili(White);
							verso=1;
					if(verso){
						x_Mu=5;
						y_Mu=6;
						prova_muro_ver(0,1,x_Mu,y_Mu);
					}
					else{
						
					x_Mu=6;
					y_Mu=7;
					prova_muro_or(0,1,x_Mu,y_Mu);
					}
				}
				else{
					colora_possibili(Green);
					if(verso){
						disfaiVer(x_Mu,y_Mu);
					}else{
					disfaiOr(x_Mu,y_Mu);}
				}
				/* pay attention here: please see slides to understand value 2 */
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	if(down_2!=0){ 
		//rotazione del muro
		if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){	/* KEY2 pressed */
			down_2++;				
			switch(down_2){
				case 2:
						if(ruota(x_Mu,y_Mu)){
					if(verso){
							x_Mu=x_Mu+1;
							y_Mu=y_Mu-1;
							}
				else{		
							x_Mu=x_Mu-1;
							y_Mu=y_Mu+1;
			}
				verso=!verso;}
				/* pay attention here: please see slides to understand value 2 */
					break;
				default:
					break;
			}
		}
		else {	/* button released */
			down_2=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
}
	
	
	if(down_0!=0){ //avvio del gioco attraverso il cambiamento del flag avvio (tutto il rit è sotto un if di avvio)
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			down_0++;				
			switch(down_0){
				case 2:
					avvio=1;
					color_turno();
					enable_timer(0);
				colora_possibili(Green);
				default:
					break;
			}
		}
		else {	/* button released */
			down_0=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
		}
	}  
LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
