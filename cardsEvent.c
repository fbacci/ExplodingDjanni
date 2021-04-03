#include "functions.h" //inclusione dell'header contenente la definizione delle strutture

extern int logIndex;

/** VARIABILI PROCEDURA attackEvent */

/* Variabile di tipo PlayerCards:
   - card: carta di appoggio;
   Variabili di tipo int:
   - j: variabile per il ciclo for;
   - choice: contiene la scelta del giocatore per quanto riguarda l'utilizzo di una NOPE/ATTACK per contrastare l'ATTACK subito. */

void attackEvent(Players *p, int n, int *t, int *cnt){
    PlayerCards *card = NULL;
    int j, t2;
    int choice = 0;

    t2 = changeTurn(p, *t);

    card = (PlayerCards *) malloc(sizeof(PlayerCards)); //allocazione di memoria per la carta di appoggio

    printf("\n%s devi eseguire %d turni.", p[t2].name, (*cnt + 1));

    printPlayerCards(p[t2].cards, p[t2].numCards);

    /* Si scorre la mano del giocatore che ha subito l'ATTACK per controllare se ci sono carte NOPE/ATTACK da usare. */
    for(j = 0; j < p[t2].numCards && choice == 0; j++){
        if(p[t2].cards[j].type == NOPE || p[t2].cards[j].type == ATTACK){
            printf("\nVuoi usare %d %s per annullare l'ATTACK? (0 / 1) ", p[t2].cards[j].type, p[t2].cards[j].sentence);
            scanf("%d", &choice);

            /* Se il giocatore possiede una NOPE/ATTACK e decide di usarla, si copiano in card le informazioni della carta usata
               e si cancella la carta dalla mano del giocatore. */
            if(choice == 1){
                card->type = p[t2].cards[j].type;
                strcpy(card->sentence, p[t2].cards[j].sentence);

                p[t2].cards = deletePlayerCard(p[t2].cards, &p[t2].numCards, j);
            } else {
                *t = t2;
                p[t2].attackTurn = true; //se il giocatore non ha modo di contrastare l'ATTACK ne subisce l'effetto
            }
        }
    }

    if(choice != 0){
        /* In base alla carta usata per contrastare l'ATTACK si eseguono diverse operazioni. */
        if(card->type == NOPE){
            *cnt = *cnt - 1; //viene decrementato il numero di turni da giocare

            /* Viene riferito al giocatore quanti turni deve eseguire. */
            printf("\n%s ha bloccato l'ATTACK e il turno torna di nuovo a %s.\n\n", p[t2].name, p[*t].name);

            return;
        }

        if(card->type == ATTACK){
            *t = changeTurn(p, *t);
            attackEvent(p, n, &(*t), &(*cnt)); //si richiama la procedura che gestisce l'attack
        }
    } else {
        *t = t2;
        p[*t].attackTurn = true;
    }
}

/** VARIABILI PROCEDURE nopeEvent */

/* Variabili di tipo int:
   - i, j: variabili per il ciclo for;
   - compare: contiene il risultato della strcmp () tra il nome del giocatore e la stringa "x";
   - cnt: contatore dei NOPE usati;
   - ok: vale 1 se il giocatore corrente possiede una o più NOPE, 0 altrimenti;
   - stopNope: vale 1 se non si hanno piu' NOPE o se nessuno vuole giocarne, 0 altrimenti;
   - cardIndex: indice della NOPE da giocare.
   Variabile di tipo char:
   - playerName[]: contiene il nome del giocatore che vuole giocare la NOPE. */

int nopeEvent(Players *p, int n, int t){
    int i, j, compare;
    int cnt = 0, ok = 0, stopNope = 0, cardIndex = -1;
    char playerName[DIMS];

    do{
        for(i = 0; i < n; i++){
            /* Si scorre la mano del giocatore corrente fino a trovare una NOPE. */
            for(j = 0; j < p[i].numCards; j++){
                /* Se la carta corrente e' di tipo NOPE si avvisa il giocatore che la possiede. */
                if(p[i].cards[j].type == NOPE){
                    printf("\n%s hai una carta di tipo NOPE.", p[i].name);
                    ok = 1; //si possiede una carta NOPE
                }
            }
        }


        /* Se uno o più gocatori possiedono una NOPE viene chiesto il nome del giocatore che la deve usare. */
        if(ok != 0){
            getchar();
            printf("\nChi vuole giocare una carta NOPE? (x se nessuno possiede/vuole giocare una NOPE) ");
            scanf("%31[^\n]s", playerName);

            for(i = 0; i < n && compare != 0; i++){
                /* Se nessuno vuole giocare una NOPE (playerName = "x") la compare vale 0, 1 altrimenti. */
                compare = strcmp(playerName, "x");

                /* Se il nome del giocatore corrente corrisponde a quello contenuto in playerName si fa scorrere la mano del
                   giocatore fino a trovare una NOPE, la quale viene eliminata dalla mano del giocatore. */
                if(compare != 0){
                    if(strstr(p[i].name, playerName) != NULL){
                        for(j = 0; j < p[i].numCards && cardIndex == -1; j++){
                            if(p[i].cards[j].type == NOPE){
                                printf("\n%s ha giocato la NOPE!\n", p[i].name);

                                cardIndex = j;
                                cnt++; //si incrementa il contatore dei NOPE

                                p[i].cards = deletePlayerCard(p[i].cards, &p[i].numCards, j);
                            }
                        }
                    }
                } else {
                    stopNope = 1; //nessuno vuole giocare la NOPE
                }

                //ok = 0; si annulla la variabile di controllo del NOPE
                cardIndex = -1; //si imposta cardIndex al suo valore di default
            }
        } else {
            stopNope = 1; //nessuno possiede una NOPE
        }
    } while(stopNope != 1);

    printf("\n");

    return cnt; //viene restituito il numero di NOPE giocati
}

/** VARIABILI FUNZIONE shuffleEvent */

/* Variabili di tipo int:
   - i: variabile per il ciclo for;
   - arrayPosition: vettore delle posizioni.
   Variabile di tipo Card:
   - sd: lista che conterra' il mazzo mischiato. */

Card * shuffleEvent(Card *d, int n, FILE *logger){
    Card *sd = NULL;
    int i, *arrayPosition;

    arrayPosition = (int *) calloc(n, sizeof(int)); //allocazione del vettore dinamico delle posizooni

    /* Si richiama la funzione shuffleDeck tante volte quante sono le carte del mazzo. */
    for(i = 0; i < n; i++){
        sd = shuffleDeck(d, sd, arrayPosition, n);
    }

    printf("Il mazzo e' stato mischiato.\n");
    fprintf(logger, "TURNO %d: IL MAZZO E' STATO MISCHIATO.\n", logIndex);

    return sd; //viene restituito il mazzo mischiato
}

/** VARIABILI PROCEDURA seeTheFutureEvent */

/* Variabile di tipo int:
   - i: variabile per il ciclo for. */

void seeTheFutureEvent(Card *d, FILE *logger){
    int i;

    /* Si scorre il mazzo per le prime tre carte, stampando le informazioni. */
    for(i = 0; i < F3C && d != NULL; i++){
        printf("%d %s\n", d->type, d->sentence);
        d = d->next;
    }

    fprintf(logger, "TURNO %d: VENGONO MOSTRATE LE PRIME TRE CARTE DEL MAZZO.\n", logIndex);
}

/** VARIABILI PROCEDURA favorEvent */

/* Variabili di tipo int:
   - i, j: variabili per il ciclo for;
   - cardPosition: posizione della carta da cedere;
   - nope: contiene la scelta del giocatore per quanto riguarda l'utilizzo del NOPE;
   - stopNope: vale 1 se il NOPE e' stato giocato, 0 altrimenti.
   Variabile di tipo char:
   - playerName[]: contiene il nome del giocatore che subisce l'effetto della FAVOR.
   Variabile di tipo PlayerCards:
   - card: carta di appoggio. */

void favorEvent(Players *p, PlayerCards *c, int nPl, int t, FILE *logger){
    int i, j, cardPosition;
    int nope = 0;
    char playerName[DIMS];
    PlayerCards *card = NULL;

    card = (PlayerCards *) malloc(sizeof(PlayerCards)); //allocazione di memoria per la carta di appoggio

    /* Viene scelto il giocatore su cui usare la FAVOR. */
    getchar();
    printf("\nScegli il giocatore: ");
    scanf("%31[^\n]s", playerName);

    fprintf(logger, "TURNO %d: %s CHIEDE UNA CARTA A %s.\n", logIndex, p[t].name, playerName);

    /* Si cerca il giocatore scelto. */
    for(i = 0; i < nPl; i++){
        if(strstr(p[i].name, playerName) != NULL){
            printPlayerCards(p[i].cards, p[i].numCards);

            /* Si controlla la mano del giocatore per vedere se e' in possesso di una carta NOPE. */
            for(j = 0; j < p[i].numCards && nope == 0; j++){
                if(p[i].cards[j].type == NOPE){
                    printf("\nHai una carta NOPE, vuoi bloccare l'effetto della FAVOR/DJANNICARDS? (0 / 1) ");
                    scanf("%d", &nope);

                    if(nope == 1){
                        p[i].cards = deletePlayerCard(p[i].cards, &p[i].numCards, j); //si cancella la NOPE utilizzata
                    }
                }
            }

            /* Il giocatore che subisce la FAVOR/DJANNICARDS non ne ha bloccato l'effetto. */
            if(nope == 0){
                /* Si chiede al giocatore che deve cedere la carta di sceglierne una dalla sua mano. */
                printf("\n%s quale carta vuoi cedere? ", p[i].name);
                scanf("%d", &cardPosition);
                getchar();

                cardPosition--;

                card = searchCard(p[i].cards, cardPosition, p[i].numCards); //si ricerca la carta nella mano

                printf("\nHai ceduto %d %s.\n\n", card->type, card->sentence);
                fprintf(logger, "TURNO %d: %s CEDE %d %s A %s.\n", logIndex, playerName, card->type, card->sentence, p[t].name);

                /* Si cancella la carta ceduta dalla mano del giocatore e la si aggiunge a quella del giocatore che ha
                    usato la carta FAVOR/DJANNICARDS. */
                p[i].cards = deletePlayerCard(p[i].cards, &p[i].numCards, cardPosition);
                p[t].cards = addPlayerCard(p[t].cards, &p[t].numCards, card->type, card->sentence);
            } else {
                fprintf(logger, "TURNO %d: %s USA LA NOPE ANNULLANDO L'EFFETTO DELLA CARTA.\n", logIndex, playerName);
                printf("\n%s ha usato la NOPE, la carta FAVOR/DJANNICARDS giocata da %s non ha effetto!", p[i].name, p[t].name);
            }
        }
    }
}

/** VARIABILI PROCEDURA meowEvent */

/* Variabili di tipo int:
   - i: variabile per il ciclo for;
   - usaMeow: variabile che contiene la scelta del giocatore per quanto riguarda la carta MEOOOW. */

void meowEvent(Players *p, int turn, int *dim, int *okMeow, int *pos){
    int i, useMeow;

    /* Si ricerca la carta MEOOOW nella mano del giocatore. */
    for(i = 0; i < p[turn].numCards && *okMeow != 1; i++){
        if(p[turn].cards[i].type == MEOOOW){
            printf("\nHai una carta MEOOOOW, vuoi giocarla? (0 / 1) ");
            scanf("%d", &useMeow);

            if(useMeow == 1){ //il giocatore decide di usare la MEOOOW
                printf("\nTi sei salvato!\n\n");
                p[turn].cards = deletePlayerCard(p[turn].cards, &p[turn].numCards, i);

                /* L'EXPLODING DJANNI viene inserito in una posizione casuale del mazzo. */
                *pos = rand()%(*dim); //viene generata una posizione casuale
                *dim = *dim + 1;

                *okMeow = 1; //e' stata giocata la carta MEOOOW
            } else {
                *okMeow = 0; //non e' stata giocata la carta MEOOOW
            }
        } else {
            *okMeow = 0; //non si hanno carte MEOOOW
        }
    }

    /* Se la MEOOOW non e' stata giocata o se il giocatore non ne possedeva viene ridotta la dimensione del mazzo in quanto
       l'EXPLODING DJANNI viene eliminata. */
    if(*okMeow == 0){
        *dim = *dim - 1;
    }
}

/** VARIABILI PROCEDURA threeDJCEvent */

/* Variabili di tipo int:
   - i, j: variabili per il ciclo for;
   - cardPosition: indice della carta che il giocatore decide di prendere dalla mano di un avversario;
   - nope: contiene la scelta dell'avverdario per quanto riguarda l'uso della NOPE contro le DJANNICARDS.
   Variabile di tipo char:
   - playerName[]: contiene il nome del giocatore che subisce le DJANNICARDS.
   Variabile di tipo PlayerCards:
   - card: variabile di appoggio. */

void threeDJCEvent(Players *p, PlayerCards *c, int nPl, int t, FILE *logger){
    int i, j, cardPosition;
    int nope = 0;
    char playerName[DIMS];
    PlayerCards *card = NULL;

    card = (PlayerCards *) malloc(sizeof(PlayerCards)); //allocazione di memoria per la carta di appoggio

    /* Viene scelto il giocatore su cui usare le DJANNICARDS. */
    printf("\nScegli il giocatore: ");
    getchar();
    scanf("%31[^\n]s", playerName);

    fprintf(logger, "TURNO %d: %s VUOLE RUBARE UNA CARTA A %s.\n", logIndex, p[t].name, playerName);

    /* Si cerca il giocatore scelto. */
    for(i = 0; i < nPl; i++){
        if(strstr(p[i].name, playerName) != NULL){
            /* Si controlla la mano dell'avversario per vedere se e' in possesso di una carta NOPE. */
            for(j = 0; j < p[i].numCards && nope == 0; j++){
                if(p[i].cards[j].type == NOPE){
                    printf("\nHai una carta NOPE, vuoi bloccare l'effetto delle tre DJANNICARDS? (0 / 1) ");
                    scanf("%d", &nope);

                    /* L'avversario decide di usare la NOPE, la quale viene cancellata dalla sua mano. */
                    if(nope == 1){
                        p[i].cards = deletePlayerCard(p[i].cards, &p[i].numCards, j);
                    }
                }
            }

            /* Il giocatore non possiede o decide di non usare la NOPE. */
            if(nope == 0){
                printPlayerCards(p[i].cards, p[i].numCards);

                /* Si chiede al giocatore che carta rubare dall'avversario scelto. */
                printf("\n%s quale carta vuoi prendere? ", p[t].name);
                scanf("%d", &cardPosition);
                getchar();

                cardPosition--;

                card = searchCard(p[i].cards, cardPosition, p[i].numCards); //si ricerca la carta nella mano

                fprintf(logger, "TURNO %d: %s PRENDE %d %s ALL'AVVERSARIO.\n", logIndex, p[t].name, card->type, card->sentence);
                printf("\nHai preso %d %s.\n\n", card->type, card->sentence);

                /* Si cancella la carta ceduta dalla mano del giocatore e la si aggiunge a quella del giocatore che ha
                    usato la carta FAVOR. */
                p[i].cards = deletePlayerCard(p[i].cards, &p[i].numCards, cardPosition);
                p[t].cards = addPlayerCard(p[t].cards, &p[t].numCards, card->type, card->sentence);
            } else {
                fprintf(logger, "TURNO %d: %s USA LA NOPE ANNULLANDO L'EFFETTO DELLA CARTA.\n", logIndex, playerName);
                printf("\n%s ha usato la NOPE, le DJANNICARDS giocate da %s non hanno effetto!", p[i].name, p[t].name);
            }
        }
    }
}
