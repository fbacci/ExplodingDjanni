#include "functions.h" //inclusione dell'header contenente la definizione delle strutture

int logIndex = 0;

int main(){
    /** Dichiarazione variabile */
    /* Variabile di tipo int:
        - choice: variabile intera contenente la scelta menu';
        - numM: variabile intera contenente il numero di carte MEOOOW in base alla modalita' di gioco;
        - numED: variabile intera contenente il numero di carte EXPLODING DJANNI in base alla modalita' di gioco;
        - numC: variabile intera contenente il numero di carte diverse da EXPLODING DJANNI e MEOOOW in base alla modalita' di gioco;
       Variabili di tipo char:
        - mode: variabile che contiene la modalita' di gioco scelta
    */

    int choice, numED, numM, numC;
    char mode;

    srand(time(NULL));

    /** EXPLODING DJANNI */
    printf("**********************************\n* E X P L O D I N G  D J A N N I *\n**********************************\n");

    printf("\nCosa vuoi fare?"
           "\n(1) Nuova partita;"
           "\n(2) Carica partita.\n");
    scanf("%d", &choice);

    /* In base alla scelta dell'utente vengono eseguite diverse operazioni */
    switch(choice){
        case 1: setMode(&mode);
                newGame(mode, &numED, &numM, &numC); //viene richiamata la procedura per la lettura del file
                break;
        case 2: loadGame(); //viene richiamata la procedura per l'apertura di un file di salvataggio
                break;
    }

    return 0;
}
