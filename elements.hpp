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
#include "functions.h"		//Contains useful functions
//#include <stdio.h> is already in "functions.h"
//#include <string.h> is already in "functions.h"

//STATS constants
#define N_STATS 13		//Number of stats for a single entity
#define N_BSTATS 9		//Number of stats that cannot be calculated (will be needed in the constructor)
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
#define MAX_LEVEL 20		//Maximum level allowed
#define MAX_ARMOR 2		//Max number of armor an entity can have
#define MAX_ATTACKS 5		//Max number of attacks an entity can have
#define MAX_ITEMS 10		//Max number of equipments an entity can carry
#define MAX_STR 4096		//Max size of a string (used for serializators
#define BARD_SPELL_MAX_LEVEL 6		//Max level of a bard spell
#define ARCANE_SPELL_MAX_LEVEL 10	//Max level of an arcane spell

//Spells constants
#define S_ARCANE 0		//Used to identify arcane spells
#define S_CLERIC 1		//Used to identify cleric spells
#define S_BARD 2			//Used to identify bard spells
#define S_RANGER 3		//Used to identify ranger spells
#define S_PALADIN 4		//Used to identify paladin spells
#define S_DRUID 5			//Used to identify druid spells

//Tabs constants
#define BARD_SPELLSLOTS	"bard_spellslots.txt"		//Tab for bard spellslot

//Action constants
#define ATTACK 1
#define HEAL 2
#define CAST 3
#define ABILITY 4
#define NOTHING 5
#define MUSIC 6

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
	char* temp;
	temp = strtok(s,",");
	name = temp;
	temp = strtok(NULL,",");
	ac_bonus = atoi(temp);
	temp = strtok(NULL,",");
	dex_max = atoi(temp);
	temp = strtok(NULL,",");
	magic_fail = atoi(temp);
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
	int crit_value;			//Crit multiplier
	int crit_range;			//Range for critical hits
	int scaling;			//Stats which the weapon uses for scaling
public: Attack(char*);			//Receives serialized attack string
	~Attack();			//Destructor
	int get_scaling();
	int attack_roll(int);			//Generate an attack with the weapon: bonus
	int damage_roll(int);			//Generate damage value of the weapon: bonus
	char* toString();
};
Attack::Attack(char* s) {
	char* temp;
	temp = strtok(s,",");
	name = temp;
	temp = strtok(NULL,",");
	damage = atoi(temp);
	temp = strtok(NULL,",");
	crit_value = atoi(temp);
	temp = strtok(NULL,",");
	crit_range = atoi(temp);
	temp = strtok(NULL,",");
	scaling = atoi(temp);
}
Attack::~Attack() {
	free(name);
}
int Attack::get_scaling() {
	return scaling;
}
int Attack::attack_roll(int bonus) {
	int ret;
	ret = throw_dice(D20) + bonus;
	return ret;
}
int Attack::damage_roll(int bonus, bool is_crit) {
	int ret;
	ret = throw_dice(damage);
	if(is_crit) ret = ret * crit_value;
	ret = ret + bonus;
	return ret;
}
char* Attack::toString() {
	char* ret;
	asprintf(ret,"%s,%d,%d,%d,%d",name,damage,crit_value,crit_range,scaling);
	return ret;		//FREE
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
	int value;	//Numeric value of the item
	int uses;	//Number of uses for the item
	bool heal;	//If true, the item is a healing item; if false, is a damage item
public:	Item();
	~Item();
	char* get_name();
	int get_value();
	int get_uses();
	bool is_heal();
	bool use();		//Checks if the item still has uses; is yes, decreases uses by 1 and returns true; else, returns false
	char* toString();
};
Item::Item(char* s) {
	char* temp;
	temp = strtok(s,",");
	name = temp;
	temp = strtok(NULL,",");
	value = atoi(temp);
	temp = strtok(NULL,",");
	uses = atoi(temp);
	temp = strtok(NULL,",");
	heal = (bool)(temp[0] - '0');		//Apparently subbing '0' to a character makes it an integer
}
Item::~Item() {
	free(name);
}
char* Item::get_name() {
	return dupstr(name);	//FREE
}
int Item::get_value() {
	return value;
}
int Item::get_uses() {
	return uses;
}
bool Item::is_heal() {
	return heal;
}
bool Item::use() {
	if(uses>0) {
		uses--;
		return true;
	}
	return false;
}
char* Item::toString() {
	char* ret;
	asprintf(ret,"%s,%d,%d",name,value,heal);
	return ret;		//FREE
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
	int type;	//Type of magic of the spell (determines which classes can use it)
	int value;	//Dice used by the spell to determine the outcome value
	int save_type;	//Type of saving throw used to be saved from the spell
	bool heal;	//Determines wether the spell is a healing or damaging one
public:	Spell(char*);	//Receives a serialized spell string
	~Spell();
	char* get_name();
	int get_level();
	int get_type();
	int get_value();
	int get_save_type();
	int get_dc(int);	//Receives spellcaster level
	char* toString();
};
Spell::Spell(char* s) {
	char* temp;
	temp = strtok(s,",");
	name = temp;
	temp = strtok(NULL,",");
	level = atoi(temp);
	temp = strtok(NULL,",");
	type = atoi(temp);
	temp = strtok(NULL,",");
	value = atoi(temp);
	temp = strtok(NULL,",");
	save_type = atoi(temp);
	temp = strtok(NULL,",");
	heal = atoi(temp);
}
Spell::~Spell() {
	free(name);
}
char* Spell::get_name() {
	return dupstr(name);	//FREE
}
int Spell::get_level() {
	return level;
}
int Spell::get_type() {
	return type;
}
int Spell::get_uses() {
	return uses;
}
int Spell::get_value() {
	return value;
}
int Spell::get_save_type() {
	return save_type;
}
bool Spell::is_heal() {
	return heal;
}
int Spell::get_dc(int caster_level) {
	int ret;
	ret = 10 + caster_level + level;
	return ret;
}
char* Spell::toString() {
	char* ret;
	asprintf(ret,"%s,%d,%d,%d,%d,%d",name,level,type,value,save_type,heal);
	return ret;		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Entity
This class contains basic attributes and methods for every entity in the game.
*/
class Entity{
protected:char name[MAX_NAME];	//Name of the entity
	int stats[N_STATS];	//Stats of the entity
	int current_hp;		//Entity's current hp
	Armor armors[MAX_ARMOR];	//Armors inventory
	Attack attacks[MAX_ATTACKS];	//Attacks used by the entity
	Item items[MAX_ITEMS];		//Items carried by the entity
public:	Entity(char*);		//Receives serialized entity string
	~Entity();
	char* get_name();	//Receives name of entity
	int get_stat(int);	//Receives parameter identifier
	int get_current_hp();
	bool is_alive();	//If current_hp > 0 true, else false
	bool equip_armor(char*);	//True if armor gets equipped, false if there is no armor space
	Armor get_armor(int);	//Receives armor identifier
	bool unequip_armor(int);	//Receives armor identifier, true if armor has been unequipped, false if there is no armor
	bool equip_attack(char*);	//True if attack gets equipped, false if there is no attack space
	Attack get_attack(int);		//Receives attack identifier
	bool unequip_attack(int);	//Receives attack identifier, true if attack has been unequipped, false if there is no attack
	bool equip_item(char*);	//True if item gets equipped, false if there is no item space
	Item get_item(int);	//Receives item identifier
	bool unequip_item(int);	//Receives item identifier, true if item has been unequipped, false if there is no item
	bool hit(int);		//Hits the entity and lowers his hp, return false if damage has been neglected
	bool heal(int);		//Heals the entity by an amount, return false if heal has been neglected
	/* Act methods: actions an entity can take */
	bool attack(Entity*,Attack);
	bool heal(Entity*,value);
	char* toString();	//serializator
};
Entity::Entity(char* s) {
	char* temp;
	temp = strtok(s,",");
	name = temp;
	for(int i=0;i<N_BSTATS;i++) {		//Only base stats are assigned, the rest can be calculated
		temp = strtok(NULL,",");
		stats[i] = atoi(temp);
	}
	current_hp = stats[HP];
	for(i=0;i<MAX_ARMOR;i++) {	//Sets all armor objects to NULL
		armors[i]=NULL;
	}
	for(i=0;i<MAX_ATTACKS;i++) {	//Sets all attack objects to NULL
		attacks[i]=NULL;
	}
	for(i=0;i<MAX_ITEMS;i++) {	//Sets all item objects to NULL
		items[i]=NULL;
	}
	if(stats[LEVEL]>MAX_LEVEL) {
		stats[LEVEL] = MAX_LEVEL;
	}
}
Entity::~Entity() {			//Deletes all external objects and frees name
	int i;
	for(i=0;i<MAX_ARMOR;i++) {	//Deletes all armor objects
		delete(armors[i]);
	}
	for(i=0;i<MAX_ATTACKS;i++) {	//Deletes all attack objects
		delete(attacks[i]);
	}
	for(i=0;i<MAX_ITEMS;i++) {	//Deletes all item objects
		delete(items[i]);
	}
	free(name);
}
char* Entity::get_name() {
	return dupstr(name);	//FREE
}
int Entity::get_stat(int id) {
	if(id<N_STATS) {		//Checks if id is within range
		return stats[id];
	} else {
		return null;
	}
}
int Entity::get_current_hp() {
	return current_hp;
}
bool Entity::is_alive() {
	return (current_hp>0);
}
bool Entity::equip_armor(char* s) {	//Receives serialized armor string
	bool ret=false;
	int i;
	for(i=0;i<MAX_ARMOR;i++) {
		if(armors[i]==NULL) {
			Armor armor(s);
			armors[i] = armor;
			ret=true;
		}
	}
	return ret;
}
Armor Entity::get_armor(int id) {
	if(id<MAX_ARMOR) {		//Checks if id is within range
		return armors[id];
	} else {
		return NULL;
	}
}
bool Entity::unequip_armor(int id) {
	if(id<MAX_ARMOR) {		//Checks if id is within range
		if(armors[id]!=NULL) {
			delete(armors[id]);
			armors[id]=NULL;	//NULL is used in 'Entity::equip_armor' to check if the slot is free
			return true;
		} else {
			return false;
		}
	}
}
bool Entity::equip_attack(char* s) {	//Receives serialized attack string
	bool ret=false;
	int i;
	for(i=0;i<MAX_ATTACKS;i++) {
		if(attacks[i]==NULL) {
			Attack attack(s);
			attacks[i] = attack;
			ret=true;
		}
	}
	return ret;
}
Attack Entity::get_attack(int id) {
	if(id<MAX_ATTACKS) {		//Checks if id is within range
		return attacks[id];
	} else {
		return NULL;
	}
}
bool Entity::unequip_attack(int id) {
	if(id<MAX_ATTACKS) {		//Checks if id is within range
		if(attacks[id]!=NULL) {
			delete(attacks[id]);
			attacks[id]=NULL;	//NULL is used in 'Entity::equip_attack' to check if the slot is free
			return true;
		} else {
			return false;
		}
	}
}
/*
equip_item method
Receives a serialized item string and creates an item in the first free item slot
Return false if all item slots are full
*/
bool Entity::equip_item(char* s) {	//Receives serialized item string
	bool ret=false;
	int i;
	for(i=0;i<MAX_ITEMS;i++) {
		if(items[i]==NULL) {
			Item item(s);
			items[i] = item;
			ret=true;
		}
	}
	return ret;
}
/*
get_item method
Returns the item corresponding to the 'id' parameter
In case the item is not found or the id is out of range, returns NULL
*/
Item Entity::get_item(int id) {
	if(id<MAX_ITEMS) {		//Checks if id is within range
		return items[id];
	} else {
		return NULL;
	}
}
/*
unequip_item method
Removes by deallocation the item corresponding to the 'id' parameter from the entity items array
In case the item is not found or the id is out of range, returns false.
*/
bool Entity::unequip_item(int id) {
	if(id<MAX_ITEMS) {		//Checks if id is within range
		if(items[id]!=NULL) {
			delete(items[id]);
			items[id]=NULL;		//NULL is used in 'Entity::equip_item' to check if the slot is free
			return true;
		}
	}
	return false;
}
/*
hit method
The current entity gets hit and loses 'value' number of hp
*/
bool hit(int value) {
	if(value==0) return false;
	current_hp = current_hp - value;
	return true;
}
/*
heal method
The current entity gets healed by 'value' number of hp
In case the current number of hp is higher than the maximum number of hp of the entity, it gets lowered to match that value
*/
bool heal(int value) {
	if(value==0) return false;
	current_hp = current_hp + value;
	if(current_hp>stats[HP]) {
		current_hp = stats[HP];
	}
	return true;
}
/* 
The toString() function doesn't take count of attacks, items and armors
Those elements are going to be assigned randomly and balanced to create a balanced entity
This way in the actual game people who have gained stronger equipment will find the fight easier, while
	people with worse equipment will find it harder
"It's not a bug, it's a feature!" -cit. YTMND
*/
char* Entity::toString() {
	char* ret;
	asprintf(ret,"%s",name);
	for(int i=0;i<N_BSTATS;i++) {		//Only base stats are serialized, because the rest can be calculated
		asprintf(ret,"%s,%d",ret,stats[i]);
	}
	return ret;		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Caster
This class, derived from Entity, represents entities who can use spells.
Contains known spells data and the method "cast" to be able to use them.
*/
class Caster : public Entity{
protected:Spell* spells;
	int spell_type;
public:	Caster(char*);		//Receives serialized caster string
	~Caster();
	bool equip_spell(char*);	//True if spell gets equipped, false if there is no spell space
	Spell get_spell(int);	//Receives spell identifier
	bool unequip_spell(int);	//Receives spell identifier, true if spell has been unequipped, false if there is no spell
	bool cast(bool);	//Receives a boolean that indicates wether the spell will be a healing spell or a damage one
	char* toString();	//Serializator
};
Caster::Caster(char* s) {
	Entity::Entity(s);
}
Caster::~Caster() {
	for(int i=0;i<n_spells;i++) {
		delete(spells[i]);
	}
	Entity::~Entity();
}
bool Caster::equip_spell(char* s) {	//Receives serialized spell string
	bool ret=false;
	int i;
	for(i=0;i<n_spells;i++) {
		if(spells[i]==NULL) {
			Spell spell(s);
			spells[i] = spell;
			ret=true;
		}
	}
	return ret;
}
Spell Caster::get_spell(int id) {
	if(id<n_spells) {		//Checks if id is within range
		return spells[id];
	} else {
		return NULL;
	}
}
bool Caster::unequip_spell(int id) {
	if(id<n_spells) {		//Checks if id is within range
		if(spells[id]!=NULL) {
			delete(spells[id]);
			spells[id]=NULL;		//NULL is used in 'Caster::equip_spell' to check if the slot is free
			return true;
		} else {
			return false;
		}
	}
}
char* Caster::toString() {
	//this is almost useless because all Caster parameters are calculated, set or generated randomly
	char* ret;
	ret = Entity::toString();
	return ret;
}
//----------------------------------------------------------------------------------------------------------------------------------
class Barbarian : public Entity{
private:int rage_time;		//Number of turns the barbarian rage lasts
public:	Barbarian(char*);	//Receives serialized barbarian string
	~Barbarian();
	bool rage();		//Sends the barbarian in rage
	bool end_rage();
	int act();		//Returns a value based on what the barbarian decided to do
	char* toString();	//Serializator
};
Barbarian::Barbarian(char* s) {
	char* temp;
	Entity::Entity(s);
	rage_time = 3 + get_modifier(stats[COS]);
}
Barbarian::~Barbarian() {
	Entity::~Entity();
}
/* Send the barbarian into rage status. If this is called, the method "end_rage" will be called every end turn 
to check wether the barbarian is still in rage or not */
bool Barbarian::rage() {
	if(rage_time==0) {
		return false;
	}
	if(level<11) {
		current_hp = current_hp + (2*level);
		stats[STR] += 4;
		stats[COS] += 4;
		stats[WILL] += 2;
		stats[AC] -= 2;
		return true;
	}
	if(level<20) {
		current_hp = current_hp + (2*level);
		stats[STR] += 6;
		stats[COS] += 6;
		stats[WILL] += 3;
		stats[AC] -= 2;
		return true;
	}
	current_hp = current_hp + (2*level);
	stats[STR] += 8;
	stats[COS] += 8;
	stats[WILL] += 4;
	stats[AC] -= 2;
	return true;
}
/* If the barbarian can still be in rage, decreases rage_time and return false. Else, reset stats and returns true */
bool Barbarian::end_rage() {
	if(rage_time>0) {
		rage_time--;
		return false;
	}
	if(level<11) {
		current_hp = current_hp - (2*level);
		stats[STR] -= 4;
		stats[COS] -= 4;
		stats[WILL] -= 2;
		stats[AC] += 2;
		return true;
	}
	if(level<20) {
		current_hp = current_hp - (2*level);
		stats[STR] -= 6;
		stats[COS] -= 6;
		stats[WILL] -= 3;
		stats[AC] += 2;
		return true;
	}
	current_hp = current_hp - (2*level);
	stats[STR] -= 8;
	stats[COS] -= 8;
	stats[WILL] -= 4;
	stats[AC] += 2;
	return true;
}
int Barbarian::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>=threshold) {
		return ATTACK;
	} else {
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return NOTHING;
			}
		}
		if(rage()) {
			end_rage();
		}
		return ATTACK;
	}
}
char* Barbarian::toString() {
	char* ret;
	ret = Entity::toString();
	return ret;		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
class Bard : public Caster{
private:int spell_uses[BARD_SPELL_MAX_LEVEL];
	int entertain;
public:	Bard(char*);		//Receives serialized bard string
	~Bard();
	int play();
	bool act(bool,bool);	//The character plays his turn, returns false if character cannot act (either by indecision or death)
	char* toString();	//Serializator
};
Bard::Bard(char* very_bardic_string) {
	Caster::Caster(very_bardic_string);
	spell_type = S_BARD;
	int table[MAX_LEVEL][BARD_SPELL_MAX_LEVEL];
	FILE* file = fopen(BARD_SPELLSLOTS,"r");
	get_table(file,MAX_LEVEL,BARD_SPELL_MAX_LEVEL,table);
	fclose(file);
	for(int i=0;i<BARD_SPELL_MAX_LEVEL;i++) {
		spell_uses[BARD_SPELL_MAX_LEVEL] = table[stats[LEVEL]][i];
	}
	entertain = strtok(NULL,",");
}
Bard::~Bard() {
	Caster::~Caster();
}
int Bard::play() {	//Plays music and tries to stop creatures from moving
	int ret;
	ret = throw_dice(D20) + get_modifier(stats[CHA]) + entertain;
	return ret;
}
bool Bard::act(bool i_need_healing, bool cant) {	//In case someone needs healing, the bard will authomatically try to heal him
	if(!is_alive()) {
		return NOTHING;
	}
	thrashold = stats[HP]/2;
	if(i_need_healing && !cant) {
		return HEAL;
	}
	if(current_hp>thrashold) {
		if((rand()%2)!=0) {
			return ATTACK;
		} else {
			return MUSIC;
		}
	} else {
		for(int i=0;i<MAX_SPELLS;i++) {
			if(spells[i]!=NULL && spells[i].is_heal() && spell_uses[spells[i].get_level()]>0) {
				spell_uses[spells[i].get_level()]--;
				cast(this,spells[i]);
				return NOTHING;
			}
		}
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return NOTHING;
			}
		}
		return ATTACK;
	}
}
char* Bard::toString() {
	char* ret;
	asprintf(ret,"%s,%d",Entity::toString(),entertain);
	return ret;
}
//----------------------------------------------------------------------------------------------------------------------------------
class Cleric : public Caster{
public:	Cleric(char*);
	~Cleric();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Druid : public Caster{
private:int wild_shape_time;		//Number of turns that Wild Shape lasts
	Animal shape;		//When "wild_shape" is used, this is set to the Animal in which the druid transforms
public:	Druid(char*);
	~Druid();
	bool act();
	int wild_shape();		//Turn the druid into an animal for random turns
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Sorcerer : public Caster{
public:	Sorcerer(char*);
	~Sorcerer();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Wizard : public Caster{
public:	Wizard(char*);
	~Wizard();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Monk : public Entity{
public:	Monk(char*);
	~Monk();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Fighter : public Entity{
public:	Fighter(char*);
	~Fighter();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Ranger : public Caster{
public:	Ranger(char*);
	~Ranger();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Rogue : public Entity{
public:	Rogue(char*);
	~Rogue();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Paladin : public Caster{
public:	Paladin(char*);
	~Paladin();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Monster : public Entity{
public:	Monster(char*);
	~Monster();
	bool act();
	char* toString();
};
//----------------------------------------------------------------------------------------------------------------------------------
class Animal : public Entity{
public:	Animal(char*);
	~Animal();
	bool act();		//The animal plays his turn, returns false if character cannot act (either by indecision or death)
	char* toString();
};
