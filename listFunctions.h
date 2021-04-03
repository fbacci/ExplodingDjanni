#ifndef LISTFUNCTIONS_H_INCLUDED
#define LISTFUNCTIONS_H_INCLUDED

#include "functions.h"

/** listFunctions */
void printDeck(Card *); //procedura che stampa il mazzo
Card * createDeck(char, Card *, int, int); //funzione per la creazione del mazzo
Card * createStoreDeck(char, Card *, int); //funzione per la creazione del mazzo contenente EXPLODING DJANNI e MEOOOW
Card * shuffleDeck(Card *, Card *, int [], int); //funzione che mischia un mazzo
Card * insertCard(Card *, int, char []); //funzione che inserisce una carta nel mazzo
Card * insertTail(Card *, int, char[]); //funzione che inserisce una carta alla fine del mazzo
Card * insertRandom(Card *, int, char [], int); //funzione che inserisce una carta in una posizione generata in maniera casuale
Card * insertSpCards(Card *, int, char [], int); //funzione per l'inserimento nel mazzo delle carte EXPLODINGDJANNI e MEOOOW
Card * findCards(Card *, int); //funzione che trova una carta
Card * removeFirstCard(Card *); //funzione che elimina una carta in testa
Card * removeCard(Card *, int); //funzione che elimina una carta dal mazzo che non sia la prima
void removeExplodingMeow(Card *); //procedura che elimina le carte EXPLODING DJANNI e MEOOOW
void freeDeck(Card *); //procedura che libera la lista del mazzo
int getDeckDim(Card *); //funzione che restituisce la dimensione del mazzo

#endif // LISTFUNCTIONS_H_INCLUDED
