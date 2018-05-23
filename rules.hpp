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

//STATS constants
#define N_STATS 12		//Number of stats serialized for a single entity
#define LEVEL 0			//Level of the character OR challenge rating of the creature
#define HP 1			//Hit points, the "life" of an entity
#define STR 2			//Strength attribute
#define DEX 3			//Dexterity attribute
#define CON 4			//Constitution attribute
#define INT 5			//Intelligence attribute
#define WIS 6			//Wisdom attribute
#define CHA 7			//Charisma attribute
#define BAB 8			//Base attack bonus attribute
#define FORT 9			//Base fortitude save attribute
#define REFL 10			//Base reflexes save attribute
#define WILL 11			//Base will save attribute
//---------
#define BASE_AC 10

//Dice constants
#define D100 100
#define D20 20
#define D12 12
#define D10 10
#define D8 8
#define D6 6
#define D4 4

void init_random();		//Sets the seed for RNG
int throw_dice(int);		//Receives number of dice faces
int get_modifier(int);		//Receives a stat and return modifier

void init_random() {
	srand(time(NULL));
}

int throw_dice(int faces) {
	return rand()%faces+1;
}

int get_modifier(int value) {
	return (value/2-5);
}
