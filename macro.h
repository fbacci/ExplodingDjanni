#ifndef MACRO_H_INCLUDED
#define MACRO_H_INCLUDED

#define DIMS 32 //dimensione nome giocatore
#define DIMSENT 128 //dimensione frase riportata in ogni carta
#define DIMFILE 30 //dimensione nome file
#define NPLAYERS 4 //numero di giocatori
#define NUMOFCARDS 5 // numero di carte in mano ad un giocatore all'inizio della partita
#define SPECIALCARDS 7 //numero di ED e MEOOOW
#define F3C 3 //numero di carte da stampare nel caso in cui il giocatore scarti la carta SEETHEFUTURE
#define LOGGER "logger.txt" //nome del file logger
#define FILESAV ".sav" //estensione file di salvataggio

#define DEBUGLISTF 0 //macro che s permette l'esecuzione di operazioni di debug riguardanti le listFunctions
#define DEBUGPRINTD 0 //macro che se vale 1 permette la stampa di un mazzo
#define DEBUGPRINTNUMOFCARDS 0 //macro che se vale 1 stampa il numero di carte presenti nel mazzo
#define DEBUGPRINTPLC 0 //macro che se vale 1 permette la stampa delle carte di un giocatore
#define DEBUGLOADGAME 1 //macro che se vale 1 permette la stampa di informazioni relative al caricamento di una partita

#endif // MACRO_H_INCLUDED
