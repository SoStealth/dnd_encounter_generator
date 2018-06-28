/*
Autore: Lorenzo Damiano
Github: https://github.com/SoStealth
*/

/*
Rules
Questa libreria contiene funzioni per applicare le regole generali, come lanciare dadi o calcolare
modificatori. Queste regole sono basate sull'edizione 3.5 di D&D.
*/

#include <time.h>
#include <stdlib.h>
#include "functions.h"

/*
Costanti STATS
Queste costanti definiscono le varie statistiche delle entità del gioco.
Vengono utilizzate nei record per identificare la statistica da prendere e nei costruttori
per assegnarle.
Sono rappresentate da dei numeri interi in quanto le statistiche vengono salvate
all'interno di array di interi.
*/
#define N_STATS 13		//Numero di statistiche base per un'entità
#define CLASS 0			//Rappresenta la classe dell'entità
#define LEVEL 1			//Livello dell'entità
#define HP 2			//Punti vita dell'entità
#define STR 3			//Attributo forza
#define DEX 4			//Attributo destrezza
#define CON 5			//Attributo costituzione
#define CLE 6			//Attributo intelligenza
#define WIS 7			//Attributo saggezza
#define CHA 8			//Attributo carisma
#define BAB 9			//Bonus di attacco base
#define FORT 10			//Bonus base al tiro salvezza sulla tempra
#define REFL 11			//Bonus base al tiro salvezza sui riflessi
#define WILL 12			//Bonus base al tiro salvezza sulla volontà
//---------
#define N_STATS_MONSTER 15	//Numero di statistiche per i mostri
#define NATURAL_AC 13	//Bonus all'armatura naturale dei mostri
#define CD 14	//Classe difficoltà del mostro

/*
Costanti DADI
Valori dei possibili dadi lanciabili
*/
#define D100 100
#define D20 20
#define D12 12
#define D10 10
#define D8 8
#define D6 6
#define D4 4

/*
init_random()
Imposta il seed per il RNG
*/
void init_random();

/*
throw_dice
Lancia un dado e calcola il punteggio secondo un espressione
Il risultato del lancio è casuale, mentre i bonus e il numero di lanci sono fissi
Riceve:
- expression: espressione che indica il tipo di lancio
Restituisce:
- valore del lancio
*/
int throw_dice(char*);		//Receives number of dice faces

/*
get_modifier
Calcola il modificatore di una caratteristica in base al suo valore
Il modificatore è un bonus/malus ai tiri di dado fornito dalle caratteristiche
Riceve:
- valore della caratteristica
Restituisce:
- modificatore corrispondente
*/
int get_modifier(int);		//Receives a stat and return modifier

void init_random() {
	srand(time(NULL));	//Randomizza in base al time attuale (alto tasso di randomicità)
}

int throw_dice(char* describer) {
	int times,damage,bonus,ret;
	expression(describer,&times,&damage,&bonus);	//Converte l'espressione ricevuta in valori interi
	if(damage==0) {	//Se il danno è 0, ergo il risultato è fisso
		return bonus;	//Restituisce il valore fisso
	} else {
		ret = (rand()%damage+1)*times+bonus;	//Calcola il valore del lancio tirando il numero di
												// dadi e sommando il bonus
	}
	return ret;	//restituisce il valore del lancio
}

int get_modifier(int value) {
	return (value/2-5);
}
