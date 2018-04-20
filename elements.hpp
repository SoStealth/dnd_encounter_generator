/*
Author: Lorenzo Damiano, aka Zeren
Github: https://github.com/SoStealth
*/

/*
Elements
This library contains classes used to represent the elements in the game.
*/

//include
#include "rules.hpp"		//Contains functions for applying rules

//STATS constants
#define N_STATS 9		//Number of stats for a single entity
#define HP 1			//Hit points, the "life" of an entity
#define AC 2			//Armor class, defence of the enemy
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

//MAX constants
#define MAX_NAME 30		//Maximum length allowed for a name
#define MAX_ARMOR 2		//Max number of armor an entity can have
#define MAX_ATTACKS 5		//Max number of attacks an entity can have

/*
Armor
This structure contains the values of the defence equipment on the character.
Atm is not used since the program uses the AC stat but may be implemented in the future for touch attacks or magic fail percentage.
*/
typedef struct{
	int ac_bonus;	//Flat bonus value the armor gives to an entity AC
	int dex_max;	//Max dex modifier value the user can have while wearing the armor
	int fail_magic;	//Percentage of magic failure while wearing the armor
}Armor;

/*
Attack
This class defines a type of attack and his values.
Contains method to get attack results.
*/
class Attack{
private:char name[MAX_NAME];		//Name of the attack
	int damage;			//Damage of the attack
	int crit;			//Crit multiplier
public: Attack(char*,int,int);		//Constructor: name, damage, crit
	~Attack();			//Destructor
	int hit(int);			//Generate an attack with the weapon: bonus
	int hurt(int);			//Generate damage value of the weapon: bonus
};
Attack::Attack(char* name, int damage, int crit) {
	this->name = name;
	this->damage = damage;
	this->crit = crit;
}
Attack::~Attack() {
	free(name);
}
int Attack::hit(int bonus) {
	int ret;
	ret = 
	return ret;
}


class Equipment{};

class Entity{
private:char name[MAX_NAME];
	int stats[N_STATS];
	int current_hp;
	Armor armors[MAX_ARMOR];
	Attack attack[MAX_ATTACKS];
public:	Entity(char*,bool);	//Receives name identifier for the entity OR filename and a boolean to identify which one
	~Entity();
	int get_parameter(int);	//Receives parameter identifier
	char* get_name();	//Receives name of entity
	int attack
};
Entity::Entity(char* filename) {
	FILE* file = fopen(filename,"r");
	//structure: "name,HP,AC,STR,DEX,CON,INT,WIS,CHAR"
	char c = fgetc(file);
	int i = 0;
	while(c!=EOF && c!=',') {
		name[i]=c;
		i++;
	}
	c = fgetc(file);
	for(i=0;i<N_STATS;i++) {
		
	}
}
