//
// Created by deric on 08/01/2024.
//
#include "LPC17xx.h"
#ifndef QUORIDOR_QUORIDOR_H
#define QUORIDOR_QUORIDOR_H

void faiOr(int x, int y);
void disfaiOr(int x, int y);
void faiVer(int x, int y);
void disfaiVer(int x, int y);
void riempi_Mat(void);
void setup(void);
int mossa_valida(int a);
void mossa_in(int a, int b);
int prova_muro_or(int s, int c, int x_m, int y_m );
void controllo_intersezione_or(int x, int y);
void controllo_riga(int y);
void colora_or(int x, int y);
int prova_muro_ver(int s, int c, int x_m, int y_m );
int ruota (int x, int y);
int check (int x_m, int y_m);
int vittoria();
void color_turno();
void diminuisci_muri();
int funzione_ricorsiva1(int x, int y );
int funzione_ricorsiva2(int x, int y );
int check_alto(int x,int y);
int check_basso(int x,int y);
int check_destra(int x,int y);
int check_sinistra(int x,int y);
void reset_caselle();
void cambio_turno();
int muri_finiti();
uint32_t mossa(int timer);
void colora_possibili(uint16_t col);
#endif //QUORIDOR_QUORIDOR_H
