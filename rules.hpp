/*
Author: Lorenzo Damiano, aka Zeren
Github: https://github.com/SoStealth
*/

/*
Rules
This library contains methods for applying the general D&D rules, like dice throws or modifiers calculator.
These rules are based on the 3.5 edition of D&D.
Library will be updated every time a new function for applying a rule will be needed.
*/

#include <time.h>
#include <stdlib.h>
#include "functions.h"

//STATS constants
#define N_STATS 13		//Number of stats serialized for a single entity
#define CLASS 0			//Class of the entity
#define LEVEL 1			//Level of the character OR challenge rating of the creature
#define HP 2			//Hit points, the "life" of an entity
#define STR 3			//Strength attribute
#define DEX 4			//Dexterity attribute
#define CON 5			//Constitution attribute
#define INT 6			//Intelligence attribute
#define WIS 7			//Wisdom attribute
#define CHA 8			//Charisma attribute
#define BAB 9			//Base attack bonus attribute
#define FORT 10			//Base fortitude save attribute
#define REFL 11			//Base reflexes save attribute
#define WILL 12			//Base will save attribute
//---------
#define N_STATS_MONSTER 15
#define NATURAL_AC 13
#define CD 14

//Dice constants
#define D100 100
#define D20 20
#define D12 12
#define D10 10
#define D8 8
#define D6 6
#define D4 4

void init_random();		//Sets the seed for RNG
int throw_dice(char*);		//Receives number of dice faces
int get_modifier(int);		//Receives a stat and return modifier

void init_random() {
	srand(time(NULL));
}

int throw_dice(char* describer) {
	int times,damage,bonus,ret;
	expression(describer,&times,&damage,&bonus);
	if(damage==0) {
		return bonus;
	} else {
		ret = (rand()%damage+1)*times+bonus;
	}
	return ret;
}

int get_modifier(int value) {
	return (value/2-5);
}
