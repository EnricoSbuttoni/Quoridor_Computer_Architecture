#include "LPC17xx.h"
#include "stdio.h"
#include "string.h"
#include "../GLCD/GLCD.h" 
#include "../TouchPanel/TouchPanel.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
extern int mode,verso, x_Mu, y_Mu;
extern int timer;
volatile int mat[13][13];
volatile char matStampa[13][13];
volatile int x1, y1, x2, y2,turno;
volatile int n_muri1=8;
volatile int n_muri2=8;
volatile int vittoria1=0;
char t1[2],t2[2];
char t3[1];
volatile uint32_t mossat; 
int check_alto(int x,int y){// check della funzione ricorsiva verso l'alto
    if(x==0){return 0;}
    if(mat[x-1][y]<2 && mat [x-2][y]<4 ){
        return 1;
    }
    else{return 0;}
}
int check_sinistra(int x,int y){// check della funzione ricorsiva a sinistra
    if(y==0){return 0;}
    if(mat[x][y-1]<2 && mat [x][y-2]<4 ){
        return 1;
    }
    else{return 0;}
}
int check_destra(int x,int y){// check della funzione ricorsiva a destra
    if(y==12){return 0;}
    if(mat[x][y+1]<2 && mat [x][y+2]<4 ){
        return 1;
    }
    else{return 0;}
}
int check_basso(int x,int y){// check della funzione ricorsiva verso il basso
    if(x==12){return 0;}
    if(mat[x+1][y]<2 && mat [x+2][y]<4 ){
        return 1;
    }
    else{return 0;}
}
 void reset_caselle(){
	mat[x1][y1]=4;
	mat[x2][y2]=5;
}
int funzione_ricorsiva1(int x, int y ){ //funzione che si occupa di cercare un percorso possibile per la pedina 1
    mat[x][y]=8; //si marcano le caselle già visitate con il numero 8 per evitare di ripercorrerle
if(x==12){ //condizione terminazione

    return 1;
}
if (!check_basso(x, y) && !check_sinistra(x, y) && !check_destra(x, y) && !check_alto(x, y)) { //uscita anticipata
        return 0;
    }
if(check_basso(x,y)){
        if(funzione_ricorsiva1(x+2,y)){
            mat[x+2] [y]=0;
            return 1;
        }
        mat[x+2] [y]=0;
    }


    if(check_sinistra(x,y)){
        if(funzione_ricorsiva1(x,y-2)){
            mat[x] [y-2]=0;
            return 1;
        }
        mat[x] [y-2]=0;
    }

        if(check_destra(x,y)){
           if( funzione_ricorsiva1(x,y+2)){
               mat[x] [y+2]=0;

               return 1;
           }
            mat[x] [y+2]=0;
        }

            if(check_alto(x,y)){
                if(funzione_ricorsiva1(x-2,y)){
                    mat[x-2] [y]=0;
                    return 1;
                }
                mat[x-2] [y]=0;
        
    
}
    
    return 0;
}
int funzione_ricorsiva2(int x, int y ){ //stessa funzione ma per la pedina 2
    mat[x][y]=8;
if(x==0){
    return 1;
}  
   if (!check_basso(x, y) && !check_sinistra(x, y) && !check_destra(x, y) && !check_alto(x, y)) {
        return 0;
    }
if(check_alto(x,y)){
                if(funzione_ricorsiva2(x-2,y)){
                    mat[x-2] [y]=0;
                    return 1;
                }
                mat[x-2] [y]=0;
        }



    if(check_sinistra(x,y)){
        if(funzione_ricorsiva2(x,y-2)){
            mat[x] [y-2]=0;
            return 1;
        }
        mat[x] [y-2]=0;
    }
   
        if(check_destra(x,y)){
           if( funzione_ricorsiva2(x,y+2)){
               mat[x] [y+2]=0;

               return 1;
           }
            mat[x] [y+2]=0;
        }
        
					if(check_basso(x,y)){
        if(funzione_ricorsiva2(x+2,y)){
            mat[x+2] [y]=0;
            return 1;
        }
        mat[x+2] [y]=0;
     
    
}
 
    return 0;
}
uint32_t mossa(int timer){ //funzione che si occupa di calcolare la mossa su un intero da 32 bit
	//usa delle maschere sotto forma di moltiplicazione per le potenze del 2 corrispondenti
	int a,b,c,d,e;
	uint32_t f;
	a=turno;
	b=mode;
	c=verso;
	if(!mode){
		c=0;
		if(turno){
			d=x2/2;
			e=y2/2;
		}
		else{
			d=x1/2;
			e=y1/2;
		}
	}
	else{
		a=!a;
		if(verso){
			e=x_Mu/2;
			d=y_Mu/2;
			d++;
		}
		else{
		e=x_Mu;
		d=y_Mu;
		e++;
		}
	}
	if(timer){
		c=1;
		b=0;
	}
	a=a*16777216; //2^24
	b=b*1048576;//2^20
	c=c*65536;//2^16
	d=d*256;//2^8
	e=e*1;//2^0
	f=a+b+c+d+e;
	return f;
}
void diminuisci_muri(){ //aggiornamento del numero di muri dei giocatori
	if(!turno){
		n_muri2--;
		if (n_muri2==0){
			GUI_Text(184, 255, (uint8_t *) "    ", White, White);
			GUI_Text(176, 255, (uint8_t *) "MURI", Blue, White);
			GUI_Text(170, 270, (uint8_t *) "FINITI", Blue, White);
		}
		else{
		GUI_Text(184, 255, (uint8_t *) "    ", White, White);
		t2[0]='0'+n_muri2;
		t2[1]=' ';
		GUI_Text(186, 255, (uint8_t *) t2, Blue, White);}
	}
	else{
		n_muri1--;
		if (n_muri1==0){
			GUI_Text(184, 255, (uint8_t *) "    ", White, White);
			GUI_Text(38, 255, (uint8_t *) "MURI", Red, White);
			GUI_Text(32, 270, (uint8_t *) "FINITI", Red, White);
		}
		else{
		GUI_Text(50, 255, (uint8_t *) "   ", White, White);
		t1[0]='0'+n_muri1;
		t1[1]='\0';
		GUI_Text(50, 255, (uint8_t *) t1, Red, White);}
	}
}
void color_turno(){ //parte grafica del numero dei turni
	if(turno){
		GUI_Text(92, 234, (uint8_t *) "       ", White, White);
		GUI_Text(92, 234, (uint8_t *) "TURNO 2", Blue, White);
	}else{
	GUI_Text(92, 234, (uint8_t *) "       ", White, White);
	GUI_Text(92, 234, (uint8_t *) "TURNO 1", Red, White);
	}
}
void controllo_riga(int y){ //controollo dell'intersezione dei muri con muri precedentemente inseriti, controllando tutta la riga corrispondente della matrice
	int i,j;
	for(i=0;i<13;i++){
		if(mat[y][i]==3){
			Fill_muro_Or_Singl(i,y, Yellow);
		}
		else{
			if(mat[y][i]==2){
				Fill_muro_Or_Singl(i,y,Green);
			}
			else{
				Fill_muro_Or_Singl(i,y,White);
			}
		}
	}
}
void controllo_colonnna(int x){//controollo dell'intersezione dei muri con muri precedentemente inseriti, controllando tutta la colonna corrispondente della matrice
	int i;
		for(i=0;i<13;i++){
		if(mat[i][x]==3){
			Fill_muro_Ver_Singl(x,i, Yellow);
		}
		else{
			if(mat[i][x]==2){
				Fill_muro_Ver_Singl(x,i,Green);
			}
			else{
				Fill_muro_Ver_Singl(x,i,White);
			}
		}
	}
}
//le quattro funzioni successive sono lo smarcamento e il marcaento delle caselle necessario per lo spostamento dei muri
void faiOr(int x, int y){
    mat[y][x]++;
    mat[y][x+1]++;
    mat[y][x+2]++;
	controllo_riga(y);
}
void disfaiOr(int x, int y){
    mat[y][x]--;
    mat[y][x+1]--;
    mat[y][x+2]--;
	controllo_riga(y);
}
void faiVer(int x, int y){
    mat[y][x]++;
    mat[y+1][x]++;
    mat[y+2][x]++;
	controllo_colonnna(x);
}
void disfaiVer(int x, int y){
    mat[y][x]--;
    mat[y+1][x]--;
    mat[y+2][x]--;
	controllo_colonnna(x);
}
void riempi_Mat(){  //funzione di setup della matrice, nelle caselle con entrambe gli indici pari si ha la possibilità di muovere la pedina
	int i,j;
    for(i=0;i<13;i++){
        if(i%2==0) {
            for(j=0;j<13;j++){
                if(j%2==0){
                    mat[i][j]=0;}
                else {
                    mat[i][j]=1;
                }
            }
        }
        else{
            for(j=0;j<13;j++){
                mat[i][j]=1;
            }
        }
    }
}
int mossa_valida(int a){ //controlli necessari per svolgere il movimento richiesto dal joystick e dal RIT
	if(turno){
		switch (a) {
            case 0:
                if(x2==0){
									return 0;
                }
                else {

                    if(mat[x2-1][y2]==2){
                       
											return 0;
                }
								if(mat[x2-2][y2]>=4){
									if(mat[x2-3][y2]==2){return 0;
									}
									return 2;
								}
                break;
            case 1:
                if(x2==12){
                   return 0;
                }
                else {
                    if(mat[x2+1][y2]==2){
												return 0;
                }if(mat[x2+2][y2]>=4){
									if(mat[x2+3][y2]==2){return 0;
									}
									return 2;
								}}
								
                break;
            case 2:
                if(y2==12){
									return 0;
									}
                else {
                    if(mat[x2][y2+1]==2){
									return 0;            
										}
                    if(mat[x2][y2+2]>=4){
									if(mat[x2][y2+3]==2){return 0;
									}
									return 2;
								}
                }
                break;
            case 3:
                if(y2==0){
                  return 0;
                }
                else {
                    if(mat[x2][y2-1]==2){
                      return 0;
                    }
                    
                
								if(mat[x2][y2-2]>=4){
									
									if(mat[x2][y2-3]==2){
										return 0;
									}
								return 2;
								}
								}
                break;
            default:
							return 0;
                break;
	}}
								return 1;
}
else{	switch (a) {
            case 0:
                if(x1==0){
									return 0;
                }
                else {

                    if(mat[x1-1][y1]==2){
                       
											return 0;
                }if (mat[x1-2][y1]>=4){
									if(mat[x1-3][y1]==2){
									return 0;}
									else{
									return 2;}
								}
								}
                break;
            case 1:
                if(x1==12){
                   return 0;
                }
                else {
                    if(mat[x1+1][y1]==2){
												return 0;
                }
										if (mat[x1+2][y1]>=4){
									if(mat[x1+3][y1]==2){
									return 0;}
									else{
									return 2;}
								}
								}
                break;
            case 2:
                if(y1==12){
									return 0;
									}
                else {
                    if(mat[x1][y1+1]==2){
									return 0;            
										}
										if (mat[x1][y1+2]>=4){
									if(mat[x1][y1+3]==2){
									return 0;}
									else{
									return 2;}
								}
                    
                }
                break;
            case 3:
                if(y1==0){
                  return 0;
                }
                else {
                    if(mat[x1][y1-1]==2){
                      return 0;
                    }
										if (mat[x1][y1-2]>=4){
									if(mat[x1][y1-3]==2){
									return 0;}
									else{
									return 2;}
								}
                    
                }
                break;
            default:
							return 0;
                break;
	}}
								return 1;
}
void setup(){ //setup iniziale del gioco, richiamato dal pulsante INT0
		riempi_Mat();
    x1=0;
    y1=6;
    mat[x1][y1]=4;
    x2=12;
    y2=6;
    mat[x2][y2]=5;
		FillCell(y1,x1,Red);
		FillCell(y2,x2,Blue);
		turno=0;
		GUI_Text(92, 234, (uint8_t *) "       ", White, White);
		GUI_Text(92, 234, (uint8_t *) "  WAIT", Green, White);
		t1[0]='0'+n_muri1;
		GUI_Text(50, 255, (uint8_t *) t1, Red, White);
	t2[0]='0'+n_muri2;
	GUI_Text(186, 255, (uint8_t *) t2, Blue, White);
}
void colora_possibili(uint16_t col){
	int a;
	if(turno){
		a=mossa_valida(0);
		if(a){
			if(a==2){
				FillCell(y2,x2-4, col);
			}else{
			FillCell(y2,x2-2, col);}
		}
		a=mossa_valida(1);
		if(a){
			if(a==2){FillCell(y2,x2+4, col);
			}else{
			FillCell(y2,x2+2, col);}
		}
		a=mossa_valida(2);
		if(a){
			if(a==2){
				FillCell(y2+4,x2, col);
			}else{
			FillCell(y2+2,x2, col);}
		}
		a=mossa_valida(3);
		if(a){
			if(a==2){FillCell(y2-4,x2,col);}
			else{
			FillCell(y2-2,x2,col);}
		}
	}
	
	else{
		a=mossa_valida(0);
		if(a){
			if(a==2){
				FillCell(y1,x1-4, col);
			}else{
			FillCell(y1,x1-2, col);}
		}
		a=mossa_valida(1);
		if(a){
			if(a==2){FillCell(y1,x1+4, col);
			}else{
			FillCell(y1,x1+2, col);}
		}
		a=mossa_valida(2);
		if(a){
			if(a==2){
				FillCell(y1+4,x1, col);
			}else{
			FillCell(y1+2,x1, col);}
		}
		a=mossa_valida(3);
		if(a){
			if(a==2){FillCell(y1-4,x1,col);}
			else{
			FillCell(y1-2,x1,col);}
		}
	}
}
	
void mossa_in(int a,int b){
	//conferma della mossa della pedina precedentemente controllata nel mossa_valida
	colora_possibili(White);
	if(turno){
	switch (a){
		case 0:
				FillCell(y2,x2,White);
				mat[x2][y2]=0;
				if(b){
					x2=x2-4;
				}
				else{
				x2=x2-2;}
				mat[x2][y2]=5;
				FillCell(y2,x2,Blue);
                break;
        case 1:
            FillCell(y2,x2,White);
				mat[x2][y2]=0;
				if(b){
					x2=x2+4;
				}
				else{
				x2=x2+2;}
				mat[x2][y2]=5;
				FillCell(y2,x2,Blue);
                break;
       case 2:
               FillCell(y2,x2,White);
								mat[x2][y2]=0;
								if(b){
					y2=y2+4;
				}
				else{
				y2=y2+2;}
			 mat[x2][y2]=5;
								FillCell(y2,x2,Blue);
                break;
                
       case 3:
							 FillCell(y2,x2,White);
								mat[x2][y2]=0;
								if(b){
					y2=y2-4;
				}
				else{
				y2=y2-2;}
			 mat[x2][y2]=5;
								FillCell(y2,x2,Blue);
                break;
                
            default:
							
                break;
	
	}}
	else{
		switch (a){
		case 0:
				FillCell(y1,x1,White);
				mat[x1][y1]=0;
				if(b){
					x1=x1-4;
				}
				else{
				x1=x1-2;}
				FillCell(y1,x1,Red);
				mat[x1][y1]=4;
                break;
        case 1:
            FillCell(y1,x1,White);
				mat[x1][y1]=0;
				if(b){
					x1=x1+4;
				}
				else{
				x1=x1+2;}
				FillCell(y1,x1,Red);
				mat[x1][y1]=4;
                break;
       case 2:
               FillCell(y1,x1,White);
								mat[x1][y1]=0;
								if(b){
					y1=y1+4;
				}
				else{
				y1=y1+2;}
								FillCell(y1,x1,Red);
								mat[x1][y1]=4;
                break;
                
       case 3:
							 FillCell(y1,x1,White);
								mat[x1][y1]=0;
								if(b){
					y1=y1-4;
				}
				else{
				y1=y1-2;}
								FillCell(y1,x1,Red);
			 mat[x1][y1]=4;
                break;
                
            default:
							
                break;
	
	}
	}
	mossat=mossa(0);
	turno=!turno;
	timer=20;
	vittoria();
	
	
	if(!vittoria1){
		colora_possibili(Green);
	color_turno();}
}
int prova_muro_or(int s, int c, int x_m, int y_m ){ //funzione di test dei muri orizzontali, richiama controllo riga e colonna
	if(s==0){
		 mat[y_m][x_m]++;
		 mat[y_m][x_m+1]++;
     mat[y_m][x_m+2]++;
		 
		Fill_muro_Or(x_m,y_m,Green);
	}
	else{
		switch (c) {
        case 0: //su
            if(y_m==1){
               return 0;
            }
            else{
                disfaiOr(x_m,y_m);
                y_m=y_m-2;
                faiOr(x_m,y_m);
								controllo_riga(y_m+2);
								controllo_riga(y_m);
							
						}
            break;
        case 1://giu 
            if(y_m==11){
                return 0;
            }
            else{
								disfaiOr(x_m,y_m);
                y_m=y_m+2;
                faiOr(x_m,y_m);
								controllo_riga(y_m-2);
								controllo_riga(y_m);}
            break;
        case 2: //destra
            if(x_m==10){
                     return 0;
            }
            else{
                disfaiOr(x_m,y_m);
                x_m=x_m+2;
                faiOr(x_m,y_m);
								controllo_riga(y_m);
						}
            break;
        case 3://sinistra
            if(x_m==0){
                 return 0;
            }
            else{
                disfaiOr(x_m,y_m);
								x_m=x_m-2;
								faiOr(x_m,y_m);
							controllo_riga(y_m);}
            break;
        default:
            break;



    }
	}
	return 1;
}
int prova_muro_ver(int s, int c, int x_m, int y_m ){//funzione di test dei muri verticali, richiama controllo riga e colonna

	if(s==0){
		 mat[y_m][x_m]++;
		 mat[y_m+1][x_m]++;
     mat[y_m+2][x_m]++;
		 controllo_colonnna(x_m);
		}
	else{
		switch (c) {
        case 0: //su
            if(y_m==0){
               return 0;
            }
            else{
                disfaiVer(x_m,y_m);
                y_m=y_m-2;
                faiVer(x_m,y_m);
								controllo_colonnna(x_m);
						}
            break;
        case 1://giu 
            if(y_m==10){
                return 0;
            }
          else{
                disfaiVer(x_m,y_m);
                y_m=y_m+2;
                faiVer(x_m,y_m);
								controllo_colonnna(x_m);
						}
            break;
        case 2: //destra
            if(x_m==11){
                     return 0;
            }
            else{
                
                disfaiVer(x_m,y_m);
                x_m=x_m+2;
                faiVer(x_m,y_m);
							controllo_colonnna(x_m);
							controllo_colonnna(x_m-2);
						}
            break;
        case 3://sinistra
            if(x_m==1){
                 return 0;
            }
             else{
                
                disfaiVer(x_m,y_m);
                x_m=x_m-2;
                faiVer(x_m,y_m);
							controllo_colonnna(x_m);
							 controllo_colonnna(x_m+2);
						}
            break;
        case 4:
            if(mat[y_m][x_m]==3||mat[y_m][x_m+1]==3||mat[y_m][x_m+2]==3){
                printf("Non puoi lasciare il muro qui, continua a spostarlo\n");}
            else{
                return 1;}
        default:
            break;



    }
	}
	return 1;
}
int vittoria(){//controllo dell'eventuale vittoria di una delle due pedine, nel caso si aggiorna il flag vittoria1 che blocca il gioco
	if (x1==12){
		GUI_Text(92, 234, (uint8_t *) "VINTO 1", Red, White);
		vittoria1++;
	}
	else if(x2==0){
		GUI_Text(92, 234, (uint8_t *) "VINTO 2", Blue, White);
		vittoria1++;
	}
}
int check (int x_m, int y_m){
	if(!verso){
		if(mat[y_m][x_m]==3||mat[y_m][x_m+1]==3||mat[y_m][x_m+2]==3){
			return 0;
	}
	else{
		turno=!turno;
		colora_possibili(Green);
		timer=20;
		color_turno();
	return 1;}
}
	else{
		if(mat[y_m][x_m]==3||mat[y_m][x_m+1]==3||mat[y_m][x_m+2]==3){return 0;
		}
		else{
			turno=!turno;
			colora_possibili(Green);
			timer=20;
			color_turno();
			return 1;
		}
}}
int ruota (int x, int y){ //rotazione del muro dopo la pressione di int2


	if(verso){ 
		//da verticale a orizzontale
		if(x==11){return 0;}
		disfaiVer(x,y);
		faiOr(x+1,y-1);
	} 
	else{
		if(y==11){return 0;
		}
		disfaiOr(x,y);
		faiVer(x-1,y+1);
	}
	return 1;
}
void cambio_turno(){ 
	colora_possibili(White);//reset turno dato dal timer
	turno=!turno;
	colora_possibili(Green);
	timer=20;
	color_turno();
	if(mode){
	if(verso){
						disfaiVer(x_Mu,y_Mu);
					}else{
					disfaiOr(x_Mu,y_Mu);}}
	mode=0;
}
int muri_finiti(){ //controllo sul numero di muri
	if(turno){
		if(n_muri2==0){ 
		return 0;
	}
	}
	else{
		if(n_muri1==0){
		return 0;
	}
	}
	return 1;
}
