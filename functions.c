#include "functions.h" //inclusione dell'header contenente la definizione delle strutture

/** VARIABILI PROCEDURA setMode */

/* Nessuna variabile aggiuntiva. */

void setMode(char *m){
    /* Si richiede la modalita' di gioco. */
    printf("\nIn che modalita' di gioco vuoi giocare?");

    printf("\n(e) Easy;"
           "\n(m) Medium;"
           "\n(h) Hard.");

    printf("\n");

    scanf("%s", m);
    getchar();
}

/** VARIABILI PROCEDURA setPlayers */

/* Variabile di tipo int:
   - i: variabile per il ciclo for. */

void setPlayers(Players *p){
    int i;

    /* Si richiedono le informazioni di ogni giocatore. */
    for(i = 0; i < NPLAYERS; i++){
        printf("Inserire il nome del giocatore n. %d: ", i + 1);
        scanf("%31[^\n]s", p->name);

        printf("Inserire il tipo di giocatore\n"
               "   0 - computer\n   1 - reale\n");
        scanf("%ud", &p->type);
        getchar();

        p->id = i;
        p->isAlive = true;
        p->attackTurn = false;
        p->numCards = 0;

        p++;

        printf("\n");
    }
}

/** VARIABILI FUNZIONE openFile */

/* Variabile di tipo FILE:
   - fp: puntatore al file. */

FILE * openFileMode(char m){
    FILE *fp = NULL;

    /*In base alla scelta della difficolta' di gioco viene aperto un file di testo diverso */
    switch(m){
        case 'e': fp = fopen("explodingDjanniEasy.txt", "r+"); //si apre il file contenente il mazzo per la modalita' easy
                  break;
        case 'm': fp = fopen("explodingDjanniMedium.txt", "r+"); //si apre il file contenente il mazzo per la modalita' medium
                  break;
        case 'h': fp = fopen("explodingDjanniHard.txt", "r+"); //si apre il file contenente il mazzo per la modalita' hard
                  break;
    }

    /* In caso di errore nell'apertura del file si esce dal programma */
    if(fp == NULL)
        exit(-1);

    return fp; //si restituisce il file
}

/** VARIABILI PROCEDURA printInfo */

/* Nessuna variabile aggiuntiva. */

void printInfo(char m, int ed, int meow, int other){
        switch(m){
        case 'e': printf("\nInformazioni mazzo modalita' EASY: ");
                  break;
        case 'm': printf("\nInformazioni mazzo modalita' MEDIUM: ");
                  break;
        case 'h': printf("\nInformazioni mazzo modalita' HARD: ");
                  break;
    }

    printf("\n  %d EXPLODING DJANNI;\n  %d MEOOOW;\n  %d carte rimanenti.", ed, meow, other);
}
