#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include "structs.h"

#include "gameFunctions.h"
#include "listFunctions.h"
#include "cardsEvent.h"

/** functions */
void setMode(char *); //procedura per settare la difficolta' della partita
void setPlayers(Players *); //procedura per caricare i dati dei giocatori
void printInfo(char, int, int, int); //procedura per stampare le informazioni del mazzo
FILE * openFileMode(char); //funzione che apre un file

#endif // FUNCTIONS_H_INCLUDED
