#ifndef CARDSEVENT_H_INCLUDED
#define CARDSEVENT_H_INCLUDED

#include "functions.h"

/** cardsEvent function */
void attackEvent(Players [], int, int *, int *); //procedura che gestisce l'ATTACK
int nopeEvent(Players [], int, int); //funzione che gestisce il NOPE
Card * shuffleEvent(Card *, int, FILE *); //funzione che gestisce la SHUFFLE
void seeTheFutureEvent(Card *d, FILE *); //procedura che gestisce la SEETHEFUTURE
void favorEvent(Players [], PlayerCards [], int, int, FILE *); //procedura che gestisce la FAVOR
void meowEvent(Players [], int, int *, int *, int *); //procedura che gestisce la MEOOOW
void threeDJCEvent(Players [], PlayerCards [], int, int, FILE *); //procedura che gestisce le 3 DJANNICARDS


#endif // CARDSEVENT_H_INCLUDED
