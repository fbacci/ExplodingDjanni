#ifndef GAMEFUNCTIONS_H_INCLUDED
#define GAMEFUNCTIONS_H_INCLUDED

#include "functions.h"

/** gameFunctions */
void newGame(char, int *, int *, int *); //procedura per l'inizio di una nuova partita
void loadGame(void); //procedura per il caricamento di una partita esistente
void game(Card *, int, Players [], FILE *, int); //procedura che "contiene" lo svolgimento della partita

/* Funzione per giocare una carta. */
PlayerCards * discard(Card *, PlayerCards *, Players [], FILE *, int, int, int *, int *, int *, int *, int *, int *);
void discardAnotherCard(Players [], PlayerCards *, int, int *); //procedura per giocare un'altra carta
PlayerCards * draw(Card *, PlayerCards *, Players [], int, int *, int); //procedura per stampa una carta
PlayerCards * giveCards(PlayerCards *, int *, Card *, Card *); //funzione che consegna le carte
PlayerCards * shufflePlayerCards(PlayerCards []); //funzione che mischia la mano di un giocatore
PlayerCards * searchCard(PlayerCards [], int, int); //funzione che cerca una carta nella mano di un giocatore
Card * cardsEvent(Card *, Players [], PlayerCards *, int [], int, int, int, int *); //funzione che gestisce gli eventi delle carte
PlayerCards * addPlayerCard(PlayerCards [], int *, int, char []); //funzione che inserisce una carta nella mano di un giocatore
PlayerCards * deletePlayerCard(PlayerCards [], int *, int); //funzione che elimina una carta dalla mano di un giocatore
int changeTurn(Players [], int); //procedura che cambia il turno
void printPlayerCards(PlayerCards [], int); //procedura che stampa le carte della mano di un giocatore
void printPlayersInfo(Players []); //procedura che stampa le informazioni di un giocatore
FILE * saveGame(FILE *, Card *, Players *, int, int, int, _Bool, int);
void winner(Players []); //procedura per decretare il vincitore

#endif // GAMEFUNCTIONS_H_INCLUDED
