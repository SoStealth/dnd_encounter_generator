/*
Author: Lorenzo Damiano, aka Zeren
Github: https://github.com/SoStealth

Elements
This library contains classes used to represent the elements in the game.
*/

//include
#include "rules.hpp"		//Contains functions for applying rules

//STATS constants
#define N_STATS 9		//Number of stats for a single entity
#define LEVEL 0			//Level of the character OR challenge rating of the creature
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
#define MAX_EQUIP 10		//Max number of equipments an entity can carry
#define MAX_STR 4096		//Max size of a string (used for serializators

//----------------------------------------------------------------------------------------------------------------------------------
/*
Armor
This class contains attributes and methods for the defence equipments.
Atm is not used since the program uses the AC stat but may be implemented in the future for touch attacks or magic fail percentage.
*/
class Armor{
private:char* name;
	int ac_bonus;	//Flat bonus value the armor gives to an entity AC
	int dex_max;	//Max dex modifier value the user can have while wearing the armor
	int magic_fail;	//Percentage of magic failure while wearing the armor
public:	Armor(char*);	//Receives a serialized armor string
	~Armor();
	char* get_name();
	int get_ac_bonus();
	int get_dex_max();
	int get_magic_fail();
	char* toString();	//Serializator
};
Armor::Armor(char* s) {
	//
}
Armor::~Armor() {
	free(name);
}
char* Armor::get_name() {
	return dupstr(name);	//FREE
}
int Armor::get_ac_bonus() {
	return ac_bonus;
}
int Armor::get_dex_max() {
	return dex_max;
}
int Armor::get_magic_fail() {
	return magic_fail;
}
char* Armor::toString() {	
	char* ret;
	asprintf(ret,"%s,%d,%d,%d",name,ac_bonus,dex_max,magic_fail);
	return ret;		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Attack
This class defines a type of attack and his values.
Contains method to get attack results.
*/
class Attack{
private:char* name;		//Name of the attack
	int damage;			//Damage of the attack
	int crit;			//Crit multiplier
public: Attack(char*);			//Receives serialized attack string
	~Attack();			//Destructor
	int hit(int);			//Generate an attack with the weapon: bonus
	int hurt(int);			//Generate damage value of the weapon: bonus
	char* toString();
};
Attack::Attack(char* s) {
	//
}
Attack::~Attack() {
	free(name);
}
int Attack::hit(int bonus) {
	int ret;
	ret = throw_dice(D20) + bonus;
	return ret;
}
int Attack::hurt(int bonus) {
	int ret;
	ret = throw_dice(damage) + bonus;
	return ret;
}
char* Attack::toString() {
	char* ret;
	asprintf(ret,"%s,%d,%d",name,damage,crit);
	return ret;		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Equipment
This contains equipment attributes and methods.
Equipment is intended as consumable item.
For now, only damage or heal items are included.
*/
class Equipment{
private:char* name;	//Name of the item
	int value;	//Numeric value of the item
	int uses;	//Number of uses for the item
	bool heal;	//If true, the item is a healing item; if false, is a damage item
public:	Equipment();
	~Equipment();
	char* get_name();
	int get_value();
	int get_uses();
	bool is_heal();
	bool use();		//Checks if the item still has uses; is yes, decreases uses by 1 and returns true; else, returns false
	char* toString();
};
Equipment::Equipment(char* s) {
	//
}
Equipment::~Equipment() {
	free(name);
}
char* Equipment::get_name() {
	return dupstr(name);	//FREE
}
int Equipment::get_value() {
	return value;
}
int Equipment::get_uses() {
	return uses;
}
bool Equipment::is_heal() {
	return heal;
}
bool Equipment::use() {
	if(uses>0) {
		uses--;
		return true;
	}
	return false;
}
char* Equipment::toString() {
	char* ret;
	asprintf(ret,"%s,%d,%d",name,value,heal);
	return ret;		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Spell
This contains all spell related attributes and methods.
For now only damage spells are included.
When using a spell, the spell gets sent to the enemy who calculates eventual damage.
*/
typedef struct{
	char name[MAX_NAME];
	int level;
	int uses;
	int value;
	int save_type;
}Spell;
//----------------------------------------------------------------------------------------------------------------------------------
class Entity{
private:char name[MAX_NAME];
	int stats[N_STATS];
	int current_hp;
	Armor armors[MAX_ARMOR];
	Attack attacks[MAX_ATTACKS];
	Equipment equipments[MAX_EQUIP];
public:	Entity(char*,bool);	//Receives name identifier for the entity OR filename and a boolean to identify which one
	~Entity();
	char* get_name();	//Receives name of entity
	int get_parameter(int);	//Receives parameter identifier
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
