#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> //inclusione libreria con funzioni per i tipi _Bool
#include <time.h>

#include "macro.h" //inclusione dell'header contenente le macro

/* Definizione delle enumerazioni che indicano il tipo della carta e il tipo del giocatore. */
typedef enum {EXPLODINGDJANNI, MEOOOW, SHUFFLE, NOPE, SEETHEFUTURE, ATTACK, SKIP, FAVOR, DJANNICARDS} CardTypes;
typedef enum {Computer, Reale} PlayerTypes;

struct Card{
    char sentence[DIMSENT]; //frase riportata nella carta
    CardTypes type;         //tipo della carta
    struct Card *next;
};

typedef struct Card Card; //definizione della struttura Card

typedef struct{
    char sentence[DIMSENT]; //frase riportata nella carta
    CardTypes type;         //tipo della carta
} PlayerCards;

typedef struct{
    int id;           //id giocatore
    char name[DIMS];  //nome giocatore
    _Bool attackTurn; ////valore booleano che indica se il giocatore ha ancora turni da eseguire dopo un ATTACK
    _Bool isAlive;    //valore booleano che indica se il giocatore e' esploso o meno
    int numCards;     //valore intero che indica quante carte ha in mano il giocatore
    PlayerTypes type; //tipo di giocatore
    PlayerCards *cards;  //array dinamico contenente le carte in mano al giocatore
} Players;

#endif // STRUCTS_H_INCLUDED
