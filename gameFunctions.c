#include "functions.h" //inclusione dell'header contenente la definizione delle strutture

extern int logIndex;

/** VARIABILI PROCEDURA newGame */

/* Variabili di tipo int:
   - deckDim: dimensione del mazzo;
   - numberOfEDM: numero di carte di tipo EXPLODINGDJANNI  e MEOOOW.
   Variabili di tipo Card:
   - deck: mazzo iniziale;
   - storeDeck: mazzo che contiene le carte di tipo EXPLODINGDJANNI  e MEOOOW.
   Variabile di tipo Players:
   - players[]: array statico contenente i giocatori. */

void newGame(char m, int *ed, int *meow, int *other){
    FILE *fp = NULL;
    FILE *logger = NULL;
    Card *deck = NULL, *storeDeck = NULL, *finalDeck = NULL;
    Players players[NPLAYERS];
    int *arrayPosition;
    int deckDim = 0, numberOfEDM = 0;
    int i, j, turn;

    fp = openFileMode(m);

    fscanf(fp, "%d %d %d", ed, meow, other); //legge i valori dal file e li scrive nelle variabili

    fclose(fp); //chiude il file

    printInfo(m, *ed, *meow, *other); //viene richiamata la procedura per la stampa delle informazioni della partita
    printf("\n\n");

    deckDim = *ed + *meow + *other; //calcola la dimensione del mazzo
    numberOfEDM = *ed + * meow; //calcola il numero di carte EXPLODING DJANNI e MEOOOOW

    setPlayers(players); //viene richiamata la procedura per l'acquisizione dei giocatori

    deck = createDeck(m, deck, deckDim, numberOfEDM); //creazione e mescolamento del mazzo
    storeDeck = createStoreDeck(m, storeDeck, numberOfEDM); // creazione del mazzo contenente le carte scartate (ED e MEOOOW)

    logger = fopen(LOGGER, "w");

    if(logger == NULL)
        exit(-1);

    /* CONSEGNA DELLE CARTE AI GIOCATORI */
    for(i = 0; i < NPLAYERS; i++){
        players[i].cards = giveCards(players[i].cards, &players[i].numCards, deck, storeDeck); //viene richiamata la funzione che consegna le carte ai giocatori
        storeDeck = removeFirstCard(storeDeck); //viene richiamata la funzione per rimuovere la prima carta del mazzo contenente le ED e le MEOOOW

        for(j = 0; j < NUMOFCARDS - 1; j++)
            deck= removeFirstCard(deck); //viene richimata la funzione per rimuovere dal mazzo le carte consegnate ai giocatori
    }

    for(i = 0; i < NPLAYERS; i++){
        players[i].cards = shufflePlayerCards(players[i].cards); //viene richiamata la funzione che mischia le carte in mano

        #if DEBUGPRINTPLC == 1
            printf("\n");
            printPlayerCards(players[i].cards, players[i].numCards);
        #endif // DEBUGPRINTPLC
    }

    fprintf(logger, "TURNO %d: VENGONO CONSEGNATE E MISCHIATE LE CARTE DEI GIOCATORI.\n", logIndex);

    /* INSERIMENTO NEL MAZZO DELLE CARTE EXPLODING DJANNI E MEOOOW */
    for(i = 0; i < numberOfEDM && storeDeck != NULL; i++){
        deck = insertCard(deck, storeDeck->type, storeDeck->sentence); //viene richiamata la funzione che inserisce una carta in testa al mazzo
        storeDeck = removeFirstCard(storeDeck); //viene richiamata la funzione che rimuove una carta dal mazzo
    }

    /* ALLOCAZIONE DELLA MEMORIA PER ARRAYPOSITION */
    arrayPosition = (int *) calloc(deckDim, sizeof(int)); //viene allocata la memoria per l'array dinamico contenente le posizioni

    if(arrayPosition == NULL)
        exit(-1);

    deckDim = getDeckDim(deck);

    /* Viene usata una lista finalDeck che contiene il mazzo che verra' usato nella partita.
       Viene mischiato nuovamente dopo che e' stato effettuato l'inserimento delle MEOOOW e degli EXPLODING DJANNI. */
    for(i = 0; i < deckDim; i++){
        finalDeck = shuffleDeck(deck, finalDeck, arrayPosition, deckDim);
    }

    fprintf(logger, "TURNO %d: VIENE MISCHIATO IL MAZZO DOPO AVER INSERITO NUOVAMENTE LE EXPLODING DJANNI E LE MEOOOW.\n", logIndex);

    #if DEBUGPRINTD == 1
        printf("\n");
        printDeck(finalDeck);
    #endif

    /* GENERAZIONE DEL PRIMO TURNO CASUALE */
    turn = rand()%(NPLAYERS);
    printf("\nIl primo turno spetta a %s!\n", players[turn].name);

    fprintf(logger, "TURNO %d: GENERAZIONE CASUALE DEL PRIMO TURNO DI GIOCO. IL TURNO SPETTA A %s.\n", logIndex, players[turn].name);

    fclose(logger);

    game(finalDeck, turn, players, logger, deckDim); //procedura che gestisce la partita
}

void loadGame(void){
    FILE *fp = NULL;
    FILE *logger = NULL;
    Card *deck = NULL;
    Players players[NPLAYERS];
    int i, j, turn, dim, cardType;
    char fileName[DIMFILE], sent[DIMSENT];
    _Bool attack;

    getchar();
    printf("\nInserire il nome della partita da caricare: ");
    scanf("%25[^\n]s", fileName);

    strcat(fileName, FILESAV);

    fp = fopen(fileName, "r+b");

    if(fp == NULL)
        exit(-1);

    for(i = 0; i < NPLAYERS; i++){
        fread(players[i].name, DIMS*sizeof(char), 1, fp);
        fread(&players[i].isAlive, sizeof(_Bool), 1, fp);

        if(players[i].isAlive == true){
            fread(&players[i].numCards, sizeof(int), 1, fp);
            fread(&players[i].type, sizeof(PlayerTypes), 1, fp);

            players[i].cards = (PlayerCards *) calloc(players[i].numCards, sizeof(PlayerCards));

            for(j = 0; j < players[i].numCards; j++){
                fread(&players[i].cards[j].type, sizeof(CardTypes), 1, fp);
                fread(players[i].cards[j].sentence, DIMSENT*sizeof(char), 1, fp);
            }
        }
    }

    fread(&dim, sizeof(int), 1, fp);

    for(i = 0; i < dim; i++){
        fread(&cardType, sizeof(CardTypes), 1, fp);
        fread(sent, DIMSENT*sizeof(char), 1, fp);

        deck = insertTail(deck, cardType, sent);
    }

    /** SALVATAGGIO INFO AGGIUNTIVE */
    fread(&turn, sizeof(int), 1, fp);
    fread(&attack, sizeof(_Bool), 1, fp);

    fclose(fp);

    logger = fopen(LOGGER, "w");

    if(logger == NULL)
        exit(-1);

    fprintf(logger, "TURNO %d: E' STATO CARICATO IL FILE DI SALVATAGGIO %s.\n", logIndex, fileName);
    fprintf(logger, "TURNO %d: SONO STATE CARICATE LE INFORMAZIONI DEI GIOCATORI.\n", logIndex);
    fprintf(logger, "TURNO %d: E' STATO CARICATO IL MAZZO.\n", logIndex);
    fprintf(logger, "TURNO %d: SONO STATE CARICATE LE INFORMAZIONI AGGIUNTIVE.\n", logIndex);

    fclose(logger);

    game(deck, turn, players, logger, dim);

    return;
}

/** VARIABILI PROCEDURA game */

/* Variabili di tipo int:
   - arrayPosition: array dinamico usato per tenere conto delle posizioni casuali gia' estratte;
   - turn: turno del giocatore;
   - okMeow: contiene o 0 (non si usa/non si possiede il MEOOOW) o 1 (il MEOOOW e' stato giocato);
   - cntNope: tiene conto di quanti NOPE sono stati giocati;
   - cntAttack: tiene conto dei turni da giocare in caso di ATTACK;
   - djCards: numero di carte di tipo DJANNICARDS giocate;
   - dimPl: contiene il numero di giocatori ancora vivi;
   - i, j: variabili per i cicli for;
   - pos: contiene la posizione casuale all'interno del mazzo nella quale verra' inserito l'EXPLODING DJANNI;
   - dim: contiene la dimensione del mazzo;
   - choice: contiene la scelta del giocatore (gioca/pesca/salva);
   - stop: vale 1 se il giocatore vuole concludere il turno, 0 altrimenti.
   Variabile di tipo Card:
   - finalDeck: lista che contiene il mazzo effettivo.
   Variabile di tipo PlayerCards:
   - card: carta di appoggio.*/

void game(Card *finalDeck, int turn, Players *p, FILE *logger, int dim){
    FILE *fp = NULL;
    _Bool attack = false;
    Card *deck = NULL;
    PlayerCards *card = NULL;
    int stop = -1, cntNope = 0, okMeow = 0, cntAttack = 0, djCards = 1, save = 0, dimPl = NPLAYERS;
    int choice, pos;
    char fileName[DIMFILE];

    logger = fopen(LOGGER, "a");

    if(logger == NULL)
        exit(-1);

    card = (PlayerCards *) malloc(sizeof(PlayerCards));

    if(card == NULL)
        exit(-1);

    printf("\n");

    while(dimPl != 1 && save == 0){
        printf("Carte di %s.", p[turn].name);
        printPlayerCards(p[turn].cards, p[turn].numCards); //stampa delle carte del giocatore

        do{
            #if DEBUGPRINTNUMOFCARDS == 1
                printf("\nDECK DIM %d\n", dim);
            #endif // DEBUGPRINTNUMOFCARDS

            if(attack == true){
                p[turn].attackTurn = true;
                cntAttack++;
            }

            /* Richiesta di un'azione da parte del giocatore */
            printf("\n(1) Gioca una carta; \n"
                   "(2) Pesca una carta; \n"
                   "(3) Salva il gioco.\n");
            scanf("%d", &choice);

            /* Il giocatore gioca una carta.*/
            if(choice == 1){
                do{
                    card = discard(finalDeck, card, p, logger, dim, dimPl, &turn, &stop, &cntNope, &cntAttack, &choice, &djCards);

                    /* Se il contatore dei NOPE giocati e' nullo o di valore pari allora viene gestito l'evento della carta giocata,
                       altrimenti se ne blocca l'effetto. */
                    if(cntNope == 0 || cntNope%2 == 0){
                        switch(card->type){
                            case SHUFFLE: finalDeck = shuffleEvent(finalDeck, dim, logger);
                                          break;
                            case SEETHEFUTURE: seeTheFutureEvent(finalDeck, logger);
                                               break;
                            case FAVOR: favorEvent(p, p[turn].cards, dimPl, turn, logger);
                                        break;
                            case DJANNICARDS: if(djCards == 2){
                                                printf("Hai attivato due %d %s.", card->type, card->sentence);
                                                favorEvent(p, p[turn].cards, dimPl, turn, logger);
                                              } else {
                                                    if(djCards == 3){
                                                        printf("Hai attivato tre %d %s.", card->type, card->sentence);
                                                        threeDJCEvent(p, p[turn].cards, dimPl, turn, logger);
                                                    }
                                              }

                                              djCards = 1;
                                              break;
                            default: break;
                        }
                    } else {
                        printf("\nLa carta %d %s non ha effetto!\n", card->type, card->sentence);
                    }

                    discardAnotherCard(p, card, turn, &stop); //prcedura per giocare un'altra carta
                }while(stop != 0 && card->type != SKIP && choice != 0);
            }

            /* Se non e' attivo l'ATTACK (choice != 0) il giocatore pesca una carta. */
            if(choice != 0){
                /* Il giocatore vuole salvare la partita. */
                if(choice == 3){
                    deck = finalDeck;

                    /* Se l'ultima carta attiva e' una ATTACK si imposta a true la variabile di controllo attack. */
                    if(card->type == ATTACK)
                        attack = true;

                    getchar();
                    printf("Inserire nome del file di salvataggio: ");
                    scanf("%25[^\n]s", fileName);

                    strcat(fileName, FILESAV); //viene concatenata l'estensione .sav al nome del file

                    fprintf(logger, "TURNO %d: %s HA SALVATO IL GIOCO.\n", ++logIndex, p[turn].name);
                    fclose(logger);

                    fp = fopen(fileName, "w+b"); //apertura del file binario in modalita' scrittura

                    if(fp == NULL)
                        exit(-1);

                    fp = saveGame(fp, deck, p, dimPl, dim, turn, attack, cntAttack); //viene richiamata la funzione per salvare il gioco

                    save = 1; //viene impostata a 1 la variabile di controllo per il salvataggio

                    fclose(fp); //viene chiuso il file di salvataggio

                    return;
                }

                if(choice == 2 || stop == 0 || card->type != SKIP || cntNope%2 != 0){
                    card = draw(finalDeck, card, p, turn, &dim, dimPl); //procedura per pescare una carta

                    fprintf(logger, "TURNO %d: %s HA PESCATO %d %s.\n", ++logIndex, p[turn].name, card->type, card->sentence);

                    /* Se la carta pescata non e' una EXPLODING DJANNI si aggiunge la carta alla mano del giocatore.
                       Se si pesca una EXPLODING DJANNI si attiva la procedura meowEvent per controllare se il giocatore
                       e' in grado di difendersi giocando una carta MEOOOW.*/
                    if(card->type != EXPLODINGDJANNI){
                        p[turn].cards = addPlayerCard(p[turn].cards, &p[turn].numCards, finalDeck->type, finalDeck->sentence);
                        finalDeck = removeFirstCard(finalDeck); //si rimuove la carta pescata dal mazzo
                    } else {
                        meowEvent(p, turn, &dim, &okMeow, &pos);
                        finalDeck = removeFirstCard(finalDeck); //si rimuove la carta pescata dal mazzo

                        /* Se il giocatore ha giocata la carta MEOOOW si reinserisce l'EXPLODING DJANNI in posizione casuale
                           all'interno del mazzo.
                           Se il giocatore muore viene ridotto il numero di giocatori in vita. */
                        if(okMeow == 1){
                            finalDeck = insertRandom(finalDeck, card->type, card->sentence, pos);
                            fprintf(logger, "TURNO %d: %s SI E' SALVATO USANDO UNA CARTA MEOOOW.\n", logIndex, p[turn].name);
                        } else {
                            printf("\n%s E' MORTO!\n\n", p[turn].name);
                            fprintf(logger, "TURNO %d: %s ESCE DAL GIOCO.\n", logIndex, p[turn].name);
                            p[turn].isAlive = false; //il giocatore non e' piu' in gioco
                            dimPl--;
                        }

                        okMeow = 0; //si azzera la variabile di controllo del meow
                    }
                }

                /* Se il contatore dei turni non e' nulla si decrementa altrimenti si pone fine al turno del giocatore che
                   ha subito l'ATTACK. */
                if(cntAttack != 0){
                    cntAttack--;
                } else {
                    attack = false;
                    p[turn].attackTurn = false;
                }
            }

            #if DEBUGPRINTPLC == 1
                printf("\n");
                printPlayerCards(p[turn].cards, p[turn].numCards);
            #endif // DEBUGPRINTPLC
        }while(p[turn].attackTurn == true); //ciclo che si ripete fino a che e' valido il turno causato dall'ATTACK

        if(choice != 0)
            turn = changeTurn(p, turn); //si richiama la procedura per cambiare turno

        cntNope = 0; //si annulla il contatore dei NOPE
    }

    /* Se il numero di giocatori in vita e' pari a 1 allora si richiama la procedura per decretare il vincitore. */
    if(dimPl == 1){
        winner(p);
    }
}

/** VARIABILI FUNZIONE discard */

/* Variabili di tipo int:
   - i: variabile per il ciclo for;
   - cardToBePlayed: indice della carta da giocare. */

PlayerCards * discard(Card *finalDeck, PlayerCards *card, Players *p, FILE *logger, int dim, int dimPl, int *turn, int *stop,
                      int *cntNope, int *cntA, int *c, int *djC){

    int i, cardToBePlayed;

    do{
        printf("\nQuale carta vuoi giocare? ");
        scanf("%d", &cardToBePlayed);

        cardToBePlayed--;

        card = searchCard(p[*turn].cards, cardToBePlayed, p[*turn].numCards); //viene cercata la carta che il giocatore vuole usare
    } while(card == NULL);

    printf("\nHai giocato %d %s.\n\n", card->type, card->sentence);
    fprintf(logger, "TURNO %d: %s HA GIOCATO %d %s.\n", ++logIndex, p[*turn].name, card->type, card->sentence);

    /* Si elimina la carta usata dal giocatore. */
    p[*turn].cards = deletePlayerCard(p[*turn].cards, &p[*turn].numCards, cardToBePlayed);

    /* Se la carta e' di tipo DJANNICARDS si incrementa il numero di DJANNICARDS giocate.
       Se il giocatore ha piu' di una DJANNICARDS dello stesso tipo puo' giocarle (2 o 3), e viene nuovamente incrementato
       il contatore. */
    if(card->type == DJANNICARDS){
        for(i = 0; i < p[*turn].numCards && *djC <=3 ; i++){
            if(strstr(p[*turn].cards[i].sentence, card->sentence) != NULL){
                *djC = *djC + 1;
                p[*turn].cards = deletePlayerCard(p[*turn].cards, &p[*turn].numCards, i); //si cancella la carta
            }
        }
    }

    /* Se la carta giocata non e' una carta FAVOR, ATTACK o DJANNICARDS viene richiamata la funzione per controllare se
       qualche avversario vuole bloccare l'effetto con la NOPE. */
    if(card->type != FAVOR && card->type != ATTACK && card->type != DJANNICARDS){
        *cntNope = nopeEvent(p, dimPl, *turn);
    }

    /* Se si gioca una SKIP/ATTACK si imposta a 1 la variabile stop, in quanto il giocatore non dovra' pescare ulteriori
       carte ma termina cosi' il suo turno. */
    if(card->type == SKIP || card->type == ATTACK){
        *stop = 1;
    }

    /* Se viene giocata una carta ATTACK si azzera la scelta del giocatore, in quanto sarà l'avversario che subisce l'attack a
       decidere come giocare il primo turno da giocare.
       Viene quindi cambiato il turno e si richiama poi la procedura per gestire l'ATTACK. */
    if(card->type == ATTACK){
        *c = 0;
        *cntA = *cntA + 1; //si incrementa il contatore dei turni
        attackEvent(p, dimPl, &(*turn), &(*cntA));
    }

    return card; //viene restituita la carta giocata
}

/** VARIABILI PROCEDURA discardAnotherCard */

/* Nessuna variabile aggiuntiva */

void discardAnotherCard(Players *p, PlayerCards *card, int turn, int *stop){
    /* Se la carta giocata non e' una SKIP viene chiesto al giocatore se vuole continuare il suo turno
        e quindi se vuole giocare un'altra carta. */
    if(card->type != SKIP && card->type != ATTACK){
        printPlayerCards(p[turn].cards, p[turn].numCards);

        printf("\nVuoi giocare un'altra carta? (0 / 1) ");
        scanf("%d", &(*stop));
    }
}

/** VARIABILI FUNZIONE draw */

/* Nessuna variabile aggiuntiva. */

PlayerCards * draw(Card *finalDeck, PlayerCards *card, Players *p, int turn, int *dim, int dimPl){
    printf("\n%s ha pescato una carta.\n\n", p[turn].name);

    /* Vengono copiate in card le informazioni della carta pescata. */
    card->type = finalDeck->type;
    strcpy(card->sentence, finalDeck->sentence);

    *dim = *dim - 1; //viene decrementata la dimensione del mazzo

    /* Se viene pescato una carta EXPLODING DJANNI viene stampato un messaggio per avvisare il giocatore. */
    if(card->type == EXPLODINGDJANNI){
        printf("Hai pescato un EXPLODING DJANNI!");
    }

    return card; //viene restituita la carta pescata
}

/** VARIABILI FUNZIONE giveCards */

/* Variabile di tipo int:
   - j: variabile per il ciclo del for. */

PlayerCards * giveCards(PlayerCards *p, int *numCards, Card *d, Card *s){
    int j;

    /* viene allocata la memoria necessaria a contenere la mano del giocatore. */
    p = (PlayerCards *) calloc(NUMOFCARDS, sizeof(PlayerCards));

    /* Si inserisce la prima carta del mazzo nell'array dinamico. */
    for(j = 0; j < NUMOFCARDS - 1; j++){
        p[j].type = d->type;
        strcpy(p[j].sentence, d->sentence);

        *numCards = *numCards + 1; //viene incrementato il numero di carte in mano al giocatore

        d = d->next; //si scorre il mazzo
    }

    /* Si inserisce una carta MEOW nella mano del giocatore. */
    p[j].type = s->type;
    strcpy(p[j].sentence, s->sentence);
    *numCards = *numCards + 1;

    #if DEBUGPRINTPLC == 1
        printf("\n");
        printPlayerCards(p, numCards);
    #endif // DEBUGPRINTPLC

    return p; //viene restituito il vettore dei giocatori
}

/** VARIABILI FUNZIONE shufflePlayerCards */

/* Variabili di tipo int:
   - c: indice di un giocatore generato casualmente;
   - i: variabile per il ciclo for.
   Variabili di tipo PLayerCards:
   - card: carta di appoggio per la carta MEOOOW;
   - aux: carta di appoggio per la carta da scambiare con la MEOOOW. */

PlayerCards * shufflePlayerCards(PlayerCards *pl){
    int c, i;
    PlayerCards *card = NULL, *aux = NULL;

    /* ALLOCAZIONE DI MEMORIA */
    card = (PlayerCards *) malloc(sizeof(PlayerCards));
    aux = (PlayerCards *) malloc(sizeof(PlayerCards));

    c = rand()&(NUMOFCARDS - 1); //viene generata una posizione random

    /* Si cerca la carta di posizione c nell'array e la si carica in card */
    for(i = 0; i < NUMOFCARDS; i++){
        if(i == c){
            card->type = pl[i].type;
            strcpy(card->sentence, pl[i].sentence);
        }
    }

    /* Si cerca la carta MEOOOW, la si copia in aux e si sovrascrivono i valori con quelli copiati in card. */
    for(i = 0; i < NUMOFCARDS; i++){
        if(i == NUMOFCARDS - 1){
            aux->type = pl[i].type;
            strcpy(aux->sentence, pl[i].sentence);

            pl[i].type = card->type;
            strcpy(pl[i].sentence, card->sentence);
        }
    }

    /* Si cerca nuovamente la carta in posizione c e si copia la carta MEOOOW. */
    for(i = 0; i < NUMOFCARDS; i++){
        if(i == c){
            pl[i].type = aux->type;
            strcpy(pl[i].sentence, aux->sentence);
        }
    }

    return pl; //viene restituita la mano del giocatore
}

/** VARIABILI FUNZIONE addPlayerCard */

/* Variabile di tipo int:
   - i: variabile per il ciclo for. */

PlayerCards * addPlayerCard(PlayerCards *pl, int *nOfCards, int cardType, char cardSentence[]){
    int i;

    *nOfCards = *nOfCards + 1; //si incrementa il numero di carte in mano al giocatore

    /* Si usa la realloc per aumentare lo spazio di memoria necessario a memorizzare le carte in mano. */
    pl = (PlayerCards *) realloc(pl, (*nOfCards)*sizeof(PlayerCards));

    /* Si scorre l'array fino a quando non si raggiunge l'ultima cella e si copiano in essa i valori della carta da aggiungere. */
    for(i = 0; i < *nOfCards; i++){
        if(i == (*nOfCards - 1)){
            pl[i].type = cardType;
            strcpy(pl[i].sentence, cardSentence);
        }
    }

    return pl; //viene restituita la mano del giocatore
}

/** VARIABILI FUNZIONE deletePlayerCard */

/* Variabile di tipo int:
   - i: variabile per il ciclo for. */

PlayerCards * deletePlayerCard(PlayerCards *pl, int *nOfCards, int cardToBeDeleted){
    int i;

    /* Si scorre la mano del giocatore. Quando l'indice e' maggiore o uguale della posizione della carta da cancellare o
       minore del numero di carte in mano, si shiftano le carte, eliminando così la carta giocata (o ceduta). */
    for(i = 0; i < *nOfCards; i++){
        if(i >= cardToBeDeleted && i < *nOfCards){
            pl[i].type = pl[i+1].type;
            strcpy(pl[i].sentence, pl[i+1].sentence);
        }
    }

    *nOfCards = *nOfCards - 1; //si riduce il numero di carte in mano al giocatore

    /* Si usa la realloc per ridurre la memoria occupata dall'array dinamico. */
    pl = (PlayerCards *) realloc(pl, (*nOfCards)*sizeof(PlayerCards));

    return pl;
}

/** VARIABILI FUNZIONE searchCard */

/* Variabile di tipo PlayerCards:
   - card: variabile che contiene le informazioni della carta cercata.
   Variabile di tipo int:
   - i: variabile per il ciclo for. */

PlayerCards * searchCard(PlayerCards c[], int pos, int n){
    PlayerCards *card = NULL;
    int i;

    card = (PlayerCards *) malloc(sizeof(PlayerCards)); //si alloca la memoria necessaria a contenere una carta

    /* Si scorre la mano del giocatore fino a che non si trova la carta che ha deciso di giocare. A questo punto si copiano le
       informazione nella carta di appoggio. */
    for(i = 0; i < n; i++){
        if(i == pos){
            card->type = c[i].type;
            strcpy(card->sentence, c[i].sentence);
        }
    }

    return card; //viene restituita la carta cercata
}

/** VARIABILI PROCEDURA printPlayersInfo */

/* Variabile di tipo int:
   - i: variabile per il ciclo for. */

void printPlayersInfo(Players *p){
    int i;

    for(i = 0; i < NPLAYERS; i++){
        printf("%d %s sei ", p[i].id, p[i].name); //stampa dell'id e del nome del giocatore

        /* Stampa del tipo di giocatore. */
        switch(p[i].type){
            case 0: printf("un computer,");
                    break;
            case 1: printf("reale,");
                    break;
        }

        /* Stampa dell'informazione che stabilisce se un giocatore e' ancora in gioco o meno e delle carte in mano nel caso in cui
           sia ancora vivo.*/
        if(p[i].isAlive == true){
            printf(" sei ancora vivo e hai %d carte in mano!\n", p[i].numCards);

            printf("Carte in mano: \n");
            printPlayerCards(p[i].cards, p[i].numCards);
        } else {
            printf(" sei morto!\n");
        }

        printf("\n");
    }
}

/** VARIABILI FUNZIONE changeTurn */

/* Nessuna variabile aggiuntiva. */

int changeTurn(Players p[], int t){
    int turn = t;

    /* Se il turno corrente spetta all'ultimo giocatore allora t si azzera, altrimenti si incremente il turno, controllando
       che il turno non appartenga a un giocatore morto. */

    do{
        if(turn == NPLAYERS){
            turn = 0;
        } else {
            turn++;
        }
    }while(p[turn].isAlive != true);

    return turn;
}

/** VARIABILI PROCEDURA printPlayerCards */

/* Variabile di tipo int:
   - i: variabile per il ciclo for. */

void printPlayerCards(PlayerCards *c, int n){
    int i;

    for(i = 0; i < n; i++){
        printf("\n    %2d) %d %s", i + 1, c[i].type, c[i].sentence); //stampa del tipo e della frase della carta corrente
    }

    printf("\n");
}

/** VARIABILI FUNZIONE saveGame */

/* Variabili di tipo int:
   - i, j: variabili per il ciclo for. */

FILE * saveGame(FILE *fp, Card *d, Players *p, int n, int dim, int t, _Bool a, int cntAtt){
    int i, j;

    /** SALVATAGGIO GIOCATORI */
    for(i = 0; i < n; i++){
        fwrite(p[i].name, DIMS*sizeof(char), 1, fp);
        fwrite(&p[i].isAlive, sizeof(_Bool), 1, fp);

        if(p[i].isAlive == true){
            fwrite(&p[i].numCards, sizeof(int), 1, fp);
            fwrite(&p[i].type, sizeof(PlayerTypes), 1, fp);

            for(j = 0; j < p[i].numCards; j++){
                fwrite(&p[i].cards[j].type, sizeof(CardTypes), 1, fp);
                fwrite(p[i].cards[j].sentence, DIMSENT*sizeof(char), 1, fp);
            }
        }
    }

    /** SALVATAGGIO CARTE */
    fwrite(&dim, sizeof(int), 1, fp);

    for(i = 0; d != NULL && i < dim; i++){
        fwrite(&d->type, sizeof(CardTypes), 1, fp);
        fwrite(d->sentence, DIMSENT*sizeof(char), 1, fp);

        d = d->next;
    }

    /** SALVATAGGIO INFO AGGIUNTIVE */
    fwrite(&p[t].id, sizeof(int), 1, fp);
    fwrite(&a, sizeof(_Bool), 1, fp);

    return fp;
}

/** VARIABILI PROCEDURA winner */

/* Variabile di tipo int:
   - i: variabile per il ciclo for. */

void winner(Players *p){
    int i;

    /* Si ricerca l'unica giocatore in vita e si proclama il vincitore. */
    for(i = 0; i < NPLAYERS; i++){
        if(p[i].isAlive == true){
            printf("COMPLIMENTI %s! Hai vinto la partita.", p[i].name);
        }
    }
}
