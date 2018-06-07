#include "entities.hpp"

#define C_VALUE 0
#define M_VALUE 1
#define TURNS 2
#define N_FACTORS 3

#define STRONG_THRESHOLD 4
#define WEAK_THRESHOLD -4

bool balance(char* balance_value, Entity* *monsters, int n_monsters) {
	char** temp;
	int factors[N_FACTORS];
	split(balance_value,',',&temp);
	for(int i=0;i<N_FACTORS;i++)
		factors[i] = atoi(temp[i]);
	
	//checking conditions
	if(factors[C_VALUE]<0) {
		if(factors[C_VALUE]>STRONG_THRESHOLD) {	//Mostri forti
			if(factors[C_VALUE]>(STRONG_THRESHOLD*4)) {	//Troppo forti, cambiare mostri
				//cambiare mostri
			} else {	//Non così forti, modificare numero o statistiche
				if(factors[M_VALUE]>WEAK_THRESHOLD) {	//Giocator
					
				}
			}
		}
	}
}
