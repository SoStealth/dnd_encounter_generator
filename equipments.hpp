/*
Author: Lorenzo Damiano, aka Zeren
Github: https://github.com/SoStealth
*/
//----------------------------------------------------------------------------------------------------------------------------------
/*
Elements
This library contains classes used to represent the elements in the game.
*/

//include
#include "rules.hpp"		//Contains functions for applying rules

//MAX constants
#define MAX_NAME 30		//Maximum length allowed for a name
#define MAX_BUFFER 4096		//Max size of a string (used for serializators)

//Spells constants
#define S_ARCANE 0		//Used to identify arcane spells
#define S_CLERIC 1		//Used to identify cleric spells
#define S_BARD 2		//Used to identify bard spells
#define S_RANGER 3		//Used to identify ranger spells
#define S_PALADIN 4		//Used to identify paladin spells
#define S_DRUID 5		//Used to identify druid spells

//Throw describers
#define ROLL_DESC "1d20"
#define USES_DESC "1d10"

//----------------------------------------------------------------------------------------------------------------------------------
/*
Armor
This class contains attributes and methods for the defence equipments.
Atm is not used since the program uses the AC stat but may be implemented in the future for touch attacks or magic fail percentage.
*/
class Armor{
private:char* name;
	int ac_bonus;	//Flat bonus value the armor gives to an entity AC
	int dex_max;	//Max dex modifier value the user can have while wearing the armor, if negative there is no limit
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
	char** temp;
	split(s,',',&temp);
	name = strdup(temp[0]);
	ac_bonus = atoi(temp[1]);
	dex_max = atoi(temp[2]);
	magic_fail = atoi(temp[3]);
	free(temp);
}
Armor::~Armor() {
	free(name);
}
char* Armor::get_name() {
	return strdup(name);	//FREE
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
	char ret[MAX_BUFFER];
	sprintf(ret,"%s,%d,%d,%d",name,ac_bonus,dex_max,magic_fail);
	return strdup(ret);		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Attack
This class defines a type of attack and his values.
Contains method to get attack results.
*/
class Attack{
private:char* name;		//Name of the attack
	char* damage;			//Damage of the attack
	int crit_value;			//Crit multiplier
	int crit_range;			//Range for critical hits
	int scaling;			//Stats which the weapon uses for scaling
public: Attack(char*);			//Receives serialized attack string
	~Attack();			//Destructor
	char* get_name();
	int get_scaling();
	void make_attack(int*,int*,int,int);		//Generate an attack with the weapon
	char* toString();
};
Attack::Attack(char* s) {
	char** temp;
	split(s,',',&temp);
	name = strdup(temp[0]);
	damage = strdup(temp[1]);
	crit_value = atoi(temp[2]);
	crit_range = atoi(temp[3]);
	scaling = atoi(temp[4]);
	free(temp);
}
Attack::~Attack() {
	free(name);
	free(damage);
}
char* Attack::get_name() {
	return strdup(name);
}
int Attack::get_scaling() {
	return scaling;
}
void Attack::make_attack(int* attack_roll, int* damage_roll, int bab, int scale) {
	int roll = throw_dice(ROLL_DESC);
	int crit = 20 - crit_range;
	if(roll>crit) {
		*damage_roll = throw_dice(damage)*crit_value + scale;
	} else {
		*damage_roll = throw_dice(damage) + scale;
	}
	*attack_roll = roll + bab + scale;
}
char* Attack::toString() {
	char ret[MAX_BUFFER];
	sprintf(ret,"%s,%s,%d,%d,%d",name,damage,crit_value,crit_range,scaling);
	return strdup(ret);		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Item
This contains item attributes and methods.
Item is intended as consumable items.
For now, only damage or heal items are included.
*/
class Item{
private:char* name;	//Name of the item
	char* value;	//Numeric value of the item
	int uses;	//Number of uses for the item
	int heal;	//FLAG: if true, the item is a healing item; if false, is a damage item
public:	Item(char*);
	~Item();
	char* get_name();
	char* get_value();
	int get_uses();
	void refull_uses();
	bool is_heal();
	bool use();		//Checks if the item still has uses; is yes, decreases uses by 1 and returns true; else, returns false
	char* toString();
};
Item::Item(char* s) {
	char** temp;
	split(s,',',&temp);
	name = strdup(temp[0]);
	value = strdup(temp[1]);
	heal = atoi(temp[2]);		//Apparently subbing '0' to a character makes it an integer
	uses = throw_dice(USES_DESC);
	free(temp);
}
Item::~Item() {
	free(name);
}
char* Item::get_name() {
	return strdup(name);	//FREE
}
char* Item::get_value() {
	return strdup(value);
}
int Item::get_uses() {
	return uses;
}
void Item::refull_uses() {
	uses = throw_dice(USES_DESC);
}
bool Item::is_heal() {
	return (heal>0);
}
bool Item::use() {
	if(uses>0) {
		uses--;
		return true;
	}
	return false;
}
char* Item::toString() {
	char ret[MAX_BUFFER];
	sprintf(ret,"%s,%s,%d",name,value,heal);
	return strdup(ret);		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Spell
This class contains all spell related attributes and methods.
For now only damage and heal spells are included.
When using a spell, the spell gets sent to the enemy who calculates eventual damage.
*/
class Spell{
private:char* name;	//Name of the spell
	int level;	//Level of the spell
	char* value;	//Dice used by the spell to determine the outcome value
	int save_type;	//Type of saving throw used to evade the spell
	int heal;	//FLAG: Determines wether the spell is a healing or damaging one
public:	Spell(char*);	//Receives a serialized spell string
	~Spell();
	char* get_name();
	int get_level();
	char* get_value();
	int get_save_type();
	int is_heal();
	int get_dc(int);	//Receives spellcaster level
	char* toString();
};
Spell::Spell(char* s) {
	char** temp;
	split(s,',',&temp);
	name = strdup(temp[0]);
	level = atoi(temp[1]);
	value = strdup(temp[2]);
	save_type = atoi(temp[3]);
	heal = atoi(temp[4]);
	free(temp);
}
Spell::~Spell() {
	free(name);
	free(value);
}
char* Spell::get_name() {

	return strdup(name);	//FREE
}
int Spell::get_level() {
	return level;
}
char* Spell::get_value() {
	return strdup(value);
}
int Spell::get_save_type() {
	return save_type;
}
int Spell::is_heal() {
	return heal;
}
int Spell::get_dc(int caster_level) {
	int ret;
	ret = 10 + caster_level + level;
	return ret;
}
char* Spell::toString() {
	char ret[MAX_BUFFER];
	sprintf(ret,"%s,%d,%s,%d,%d",name,level,value,save_type,heal);
	return strdup(ret);		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
