#include "functions.h" //inclusione dell'header contenente la definizione delle strutture

/** VARIABILI FUNZIONE CREATEDECK */

/* Variabile di tipo int:
    - i: variabile intera per il ciclo for;
    - nCard: variabile intera contenente il numero della carta, ossia il tipo della carta;
    - cardPosition: variabile intera contenente la posizione di una carta all'interno della lista. La posizione e' generata
                    in modo casuale;
    - remainingCards: variabile intera contenente il numero di carte diverse da EXPLODING DJANNI e MEOOOW in base alla modalita'
                      di gioco;
    - arrayPosition[]: array di interi usato per tenere traccia delle posizioni gia' estratte.
    Variabili di tipo char:
        - sent[]: stringa contenente la frase di una carta.
    Variabili di tipo Card:
        - deck: mazzo non mescolato, inizialmente contiene sia carte EXPLODING DJANNI che carte MEOOOOW;
        - card: carta "di appoggio".
*/

Card * createDeck(char m, Card *shuffledDeck, int deckD, int numEDM){
    FILE *fp = NULL;
    Card *deck = NULL;
    int i, nCard, remainingCards;
    char sent[DIMSENT];
    int *arrayPosition;

    fp = openFileMode(m);

    fscanf(fp, "%*[^\n]"); //scarta la prima riga del file, ossia quella che contiene il numero di carte del mazzo

    /** Fino a che non si raggiunge la fine del file vengono copiate le informazioni di ogni carta nelle due variabili
        per poi inserirle nel mazzo. */

    while(!feof(fp)){
        fscanf(fp, "%d", &nCard);
        fscanf(fp, "%127[^\n]s", sent);
        deck = insertCard(deck, nCard, sent);
    }

    fclose(fp); //chiude il file

    /** Vengono rimosse le carte EXPLODING DJANNI e MEOOOOW presenti alla fine della lista e vengono memorizzate
        in un'altra lista. */

    for(i = 0; i < numEDM; i++)
        removeExplodingMeow(deck);

    remainingCards = deckD - numEDM; //calcola le carte rimanenti dopo la rimozione delle EXPLODING DJANNI e MEOOOW

    arrayPosition = (int *) calloc(remainingCards, sizeof(int));

    /** Il seguente ciclo for permette di inserire in maniera casuale le carte in un'altra lista, shuffledDeck.*/
    for(i = 0; i < remainingCards; i++){
        shuffledDeck = shuffleDeck(deck, shuffledDeck, arrayPosition, remainingCards);
    }

    #if DEBUGLISTF == 1
        printf("\n");
        printDeck(shuffledDeck);
    #endif

    return shuffledDeck; //viene restituito il mazzo
}

/** VARIABILI FUNZIONE createStoreDeck */

/* Variabili di tipo int:
   - i: variabile intera per il ciclo for;
   - nCard: variabile intera contenente il numero della carta, ossia il tipo della carta.
   Variabile di tipo char:
   - sent[]: stringa contenente la frase di una carta. */

Card * createStoreDeck(char m, Card *sd, int dim){
    FILE *fp = NULL;
    int i, nCard;
    char sent[DIMSENT];

    fp = openFileMode(m); //viene aperto il file richiamando la funzione openFile

    fscanf(fp, "%*[^\n]"); //scarta la prima riga del file, ossia quella che contiene il numero di carte del mazzo

    /* Si memorizzano le informazioni di ogni carta, inserendola poi nel mazzo. */
    for(i = 0; i < dim; i++){
        fscanf(fp, "%d", &nCard);
        fscanf(fp, "%127[^\n]s", sent);
        sd = insertCard(sd, nCard, sent);
    }

    fclose(fp); //si chiude il file

    return sd; //si restituisce il mazzo
}

/** VARIABILI FUNZIONE shuffleDeck */

/* Variabile di tipo int:
   - cardPos: variabile che contiene la posizione di una carta generata in maniera casuale.
   Variabile di tipo Card:
   - c: variabile che contiene le informazioni di una carta. */

Card * shuffleDeck(Card *d, Card *sd, int *a, int n){
    Card *c = NULL;
    int cardPos;

    c = (Card *) malloc(sizeof(Card)); //allocazione di memoria per la carta di appoggio

    /* Viene generata una posizione casuale. Se viene generata una posizione gia' estratta si ripete l'operazione. */
    do{
        cardPos = rand()%(n);
    }while(a[cardPos] == 1);

    a[cardPos] = 1; //si imposta a 1 la cella di posizione cardPos per indicare che questa posizione e' stata estratta

    c = findCards(d, cardPos); //si cerca la carta

    sd = insertCard(sd, c->type, c->sentence); //si inserisce la carta nel mazzo

    return sd; //viene restituito il mazzo
}

/** VARIABILI FUNZIONE insertCard */

/* Variabile di tipo card:
   - newCard: variabile che contiene le informazioni della carta da aggiungere al mazzo. */

Card * insertCard(Card *d, int number, char s[]){
    Card *newCard = NULL;

    newCard = (Card *) malloc(sizeof(Card)); //allocazione di memoria per la carta da aggiungere

    if(newCard == NULL)
        exit(-1);

    /* Si copiano le informazioni della carta, accondando ad essa il resto del mazzo. */
    newCard->type = number;
    strcpy(newCard->sentence, s);
    newCard->next = d;

    return newCard; //viene restituita la nuova carta
}

/** VARIABILI FUNZIONE insertTail */

/* */

Card * insertTail (Card *d, int cType, char *sent){
    Card *aux = d;

    if(d == NULL){
        d = insertCard(d, cType, sent);
    } else {
        while(aux->next != NULL){
            aux = aux->next;
        }

        aux->next = insertCard(NULL, cType, sent);
    }

    return d;
}

/** VARIABILI FUNZIONE insertRandom */

/* Variabile di tipo int:
   - i: variabile per il ciclo for.
   Variabile di tipo Card:
   - current: copia del mazzo usata per scorrere le carte fino alla posizione desiderata. */

Card * insertRandom(Card *d, int t, char sent[], int pos){
    Card *current = d;
    int i;

    /* Se pos e' 0 si sfrutta l'inserimento in testa, altrimenti si scorre fino alla carta collocata in pos - 1.
       Il successore di questa carta sara' la carta da aggiungere al mazzo, e la si aggiunge sfruttando nuovamente l'inserimento
       in testa. */
    if(pos == 0){
        current = insertCard(d, t, sent);
    } else {
        for(i = 0; i < pos - 1 && current != NULL; i++){
            current = current->next;
        }

        if(current != NULL){
            current->next = insertCard(current->next, t, sent);
        }
    }

    return d; //viene restituito il mazzo
}

/** VARIABILI FUNZIONE insertSpCards */

/* Variabile di tipo int:
   - index: variabile che contiene l'indice generato casualmente. */

Card * insertSpCards(Card *d, int t, char sent[], int n){
    int index;

    index = rand()%(n); //viene generato un indice casuale

    d = insertRandom(d, t, sent, index); //si inserisce la carta nel mazzo

    return d; //si restituisce il mazzo
}

/** VARIABILI FUNZIONE findCards */

/* Variabili di tipo int:
   - i: variabile per il ciclo for.
   Variabili di tipo Card:
   - current: copia della lista usata per scorrere il mazzo;
   - aux: contiene le informazione della carta cercata. */

Card * findCards(Card *d, int p){
    Card *current = d, *aux = NULL;
    int i = 0;

    aux = (Card *) malloc(sizeof(Card)); //allocazione di memoria per la carta di appoggio

    /* Si scorre il mazzo fino a che l'indice i non e' uguale a p. */
    while(current != NULL && i != p){
        i++;
        current = current->next;
    }

    return aux = current; //si restituisce la carta
}

/** VARIABILI FUNZIONE removeFirstCard */

/* Variabile di tipo card:
   - aux: mantiene una copia della carta successiva a quella da cancellare. */

Card * removeFirstCard(Card *d){
    Card *aux = NULL;

    aux = (Card *) malloc(sizeof(Card)); //allocazione di memoria per la lista di appoggio

    /* aux prende il successore della prima carta del mazzo, e si viene liberata la prima carta. */
    if(d != NULL){
        aux = d->next;
        free(d);
    }

    return aux; //si restituisce il mazzo
}

/** VARIABILI FUNZIONE removeCard */

/* Variabile di tipo int:
   - i: variabile per il ciclo for.
   Variabile di tipo Card:
   - current: copia della lista usata per scorrere il mazzo. */

Card * removeCard(Card *d, int pos){
    Card *current = d;
    int i;

    /* Se pos e' 0 si sfrutta l'eliminazione in testa, altrimenti si scorre fino alla carta collocata in pos - 1.
       Il successore di questa carta sara' la carta da eliminare dal mazzo, e la si elimina sfruttando nuovamente l'eliminazione
       in testa. */
    if(pos == 0){
        current = removeFirstCard(current);
    } else {
        for(i = 0; i < pos - 1 && current != NULL; i++){
            current = current->next;
        }

        if(current != NULL){
            current->next = removeFirstCard(current->next);
        }
    }

    return d; //si restituisce il mazzo
}

/** VARIABILI PROCEDURA removeExplodingMeow */

/* Variabili di tipo Card:
   - current: copia della lista usata per scorrere il mazzo;
   - aux: mantiene una copia della carta successiva a quella da cancellare. */

void removeExplodingMeow(Card *d){
    Card *current = d, *aux;

    /* Si scorre il mazzo fino a che il successore della carta corrente non e' NULL.
       Ad ogni ciclo aux prende le carte correnti mentre si fa scorrere il mazzo.*/
    while(current->next != NULL){
        aux = current;
        current = current->next;
    }

    free(aux->next); //si libera la carta successiva (l'ultima del mazzo)
    aux->next = NULL; //il successore della carta corrente e' NULL
}

/** VARIABILI FUNZIONE getDeckDim */

/* Variabile di tipo int:
   - n: variabile che viene incrementata ogni volta che scorre la lista.
   Variabile di tipo Card:
   - current: copia della lista usata per scorrere il mazzo. */

int getDeckDim(Card *d){
    Card *current = d;
    int n = 0; //si inizializza a 0 la dimensione del mazzo

    /*Si scorre il mazzo, incrementando di 1 ad ogni ciclo la dimensione. */
    while(current != NULL){
        n++;
        current = current->next;
    }

    return n; //si restituisce la dimensione del mazzo
}

/** VARIABILI PROCEDURA freeDeck */

/* Variabile di tipo Card:
   - aux: contiene le informazioni della carta corrente da eliminare. */

void freeDeck(Card *d){
    Card *aux;

    /* Ad ogni ciclo si copia in aux il mazzo d, mentre d scorre. Si libera aux per cancellare ogni carta del mazzo. */
    while(d != NULL){
        aux = d;
        d = d->next;
        free(aux);
    }
}

/** VARIABILI PROCEDURA printDeck */

/* Variabile di tipo int:
   - i: variabile per il ciclo for. */

void printDeck(Card *d){
    int i = 0;

    /* Si scorre il mazzo stampando le informazionei di ogni carta. */
    while(d != NULL){
        printf("Carta n. %2d) %d %s\n", i, d->type, d->sentence);
        d = d->next;
        i++;
    }
}
