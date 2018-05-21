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

//MAX constants
#define MAX_NAME 30		//Maximum length allowed for a name
#define MAX_LEVEL 20		//Maximum level allowed
#define MAX_ARMOR 2		//Max number of armor an entity can have
#define MAX_ATTACKS 5		//Max number of attacks an entity can have
#define MAX_ITEMS 10		//Max number of equipments an entity can carry
#define MAX_STR 4096		//Max size of a string (used for serializators
//Every max level for spell is increased by 1 for array length purposes
#define SPELL_MAX_LEVEL 10	//Max level of any spell
#define BARD_SPELL_MAX_LEVEL 7		//Max level of a bard spell
#define ARCANE_SPELL_MAX_LEVEL 10	//Max level of an arcane spell
#define CLERIC_SPELL_MAX_LEVEL 10	//Max level of a cleric spell
#define DRUID_SPELL_MAX_LEVEL 10	//Max level of a druid spell
#define RANGER_SPELL_MAX_LEVEL 5	//Max level of a ranger spell
#define PALADIN_SPELL_MAX_LEVEL 5	//Max level of a paladin spell

//Spells constants
#define S_ARCANE 0		//Used to identify arcane spells
#define S_CLERIC 1		//Used to identify cleric spells
#define S_BARD 2			//Used to identify bard spells
#define S_RANGER 3		//Used to identify ranger spells
#define S_PALADIN 4		//Used to identify paladin spells
#define S_DRUID 5			//Used to identify druid spells

//Tabs constants
#define BARD_SPELLSLOTS	"bard_spellslots.txt"		//Tab for bard spellslots
#define CLERIC_SPELLSLOTS "cleric_spellslots.txt"	//Tab for cleric spellslots
#define DRUID_SPELLSLOTS "druid_spellslots.txt"		//Tab for druid spellslots
#define SORCERER_SPELLSLOTS "sorcerer_spellslots.txt"	//Tab for sorcerer spellslots
#define RANGER_SPELLSLOTS "ranger_spellslots.txt"	//Tab for ranger spellslots
#define PALADIN_SPELLSLOTS "paladin_spellslots.txt"	//Tab for paladin spellslots


//Action constants
#define ATTACK 1	//The character decides to attack
#define HEAL 2		//The character decides to heal another character
#define CAST 3		//The character decides to cast a spell
#define ABILITY 4	//The character decides to use an ability
#define NOTHING 5	//The character decides to do nothing (or can't do nothing)
#define MUSIC 6		//Bards: the character starts playing

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
	void make_attack(int*,int*,int,int);		//Generate an attack with the weapon
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
void Attack::make_attack(int* attack_roll, int* damage_roll, int bab, int scale) {
	int roll = throw_dice(D20);
	int crit = 20 - crit_range;
	if(roll>crit) {
		*damage_roll = throw_dice(damage)*crit_value + scale;
	} else {
		*damage_roll = throw_dice(damage) + scale;
	}
	*attack_roll = roll + bab + scale;
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
	heal = (bool)(temp[0] - '0');		//Apparently subbing '0' to a character makes it an integer
	uses = throw_dice(D10);
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
	int value;	//Dice used by the spell to determine the outcome value
	int save_type;	//Type of saving throw used to evade the spell
	bool heal;	//Determines wether the spell is a healing or damaging one
public:	Spell(char*);	//Receives a serialized spell string
	~Spell();
	char* get_name();
	int get_level();
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
	asprintf(ret,"%s,%d,%d,%d,%d",name,level,value,save_type,heal);
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
	int get_ac();
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
	char* toString();	//serializator
};
Entity::Entity(char* s) {
	char* temp;
	temp = strtok(s,",");
	name = temp;
	for(int i=0;i<N_STATS;i++) {		//Only base stats are assigned, the rest can be calculated
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
	stats[FORT] = stats[FORT] + get_modifier(stats[COS]);
	stats[REFL] = stats[REFL] + get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] + get_modifier(stats[WIS]);
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
int Entity::get_ac() {
	int ret;
	ret = BASE_AC;
	for(i=0;i<MAX_ARMOR;i++) {	//Deletes all armor objects
		if(armors[i]!=NULL) {
			ret = ret + armors[i].get_ac_bonus();
		}
	}
	ret = ret + get_modifier(stats[DEX]);
	return ret;
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
bool Entity::attack(Entity* target, Attack attack) {
	int attack_roll;
	int damage_roll;
	int scale = get_modifier(stats[attack->get_scaling()]);
	attack.make_attack(&attack_roll,&damage_roll,stats[BAB],scale);
	if(attack_roll > target->get_ac()) {
		target.hit(damage_roll);
		return true;
	}
	return false;
}
/* 
The toString() function doesn't take count of attacks, items and armors
Those elements are going to be assigned randomly and balanced to create a balanced entity
This way in the actual game people who have gained stronger equipment will find the fight easier, while
	people with worse equipment will find it harder
"It's not a bug, it's a feature!" -cit. YTMND
*/
char* Entity::toString() {
	stats[FORT] = stats[FORT] - get_modifier(stats[COS]);
	stats[REFL] = stats[REFL] - get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] - get_modifier(stats[WIS]);
	char* ret;
	asprintf(ret,"%s",name);
	for(int i=0;i<N_STATS;i++) {		//Only base stats are serialized, because the rest can be calculated
		asprintf(ret,"%s,%d",ret,stats[i]);
	}
	stats[FORT] = stats[FORT] + get_modifier(stats[COS]);
	stats[REFL] = stats[REFL] + get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] + get_modifier(stats[WIS]);
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
	int spell_uses[SPELL_MAX_LEVEL];
	int spell_type;
public:	Caster(char*);		//Receives serialized caster string
	~Caster();
	bool equip_spell(char*);	//True if spell gets equipped, false if there is no spell space
	Spell get_spell(int);	//Receives spell identifier
	bool unequip_spell(int);	//Receives spell identifier, true if spell has been unequipped, false if there is no spell
	bool can_cast();
	bool cast(Entity*,Spell);	//Target, spell
	char* toString();	//Serializator
};
Caster::Caster(char* s) {
	Entity::Entity(s);
	for(int i=0;i<SPELL_MAX_LEVEL;i++) {
		spell_uses[i] = 0;
	}
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
bool Caster::can_cast() {
	for(int i=0;i<SPELL_MAX_LEVEL;i++) {
		if(spell_uses[i]>0) return true;
	}
	return false;
}
bool Caster::cast(Entity* target, Spell spell) {
	int cd = spell->get_cd(stats[LEVEL]);
	int save = throw_dice(D20) + stats[spell->get_save_type()];
	if(spell->is_heal()) {
		target->heal(throw_dice(spell->get_value()));
		return true;
	}
	if(save <= cd) {
		target->hit(throw_dice(spell->get_value()));
		return true;
	}
	return false;
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
private:int entertain;
public:	Bard(char*);		//Receives serialized bard string
	~Bard();
	int play();
	int act();	//The character plays his turn, returns false if character cannot act (either by indecision or death)
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
		spell_uses[i] = table[stats[LEVEL]][i];
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
int Bard::act() {	//In case someone needs healing, the bard will authomatically try to heal him
	if(!is_alive()) {
		return NOTHING;
	}
	thrashold = stats[HP]/2;
	if(current_hp>thrashold) {
		if((rand()%2)!=0) {
			return ATTACK;
		} else {
			return MUSIC;
		}
	} else {
		if(this->can_cast()) {
			for(int i=0;i<MAX_SPELLS;i++) {
				if(spells[i]!=NULL && spells[i].is_heal() && spell_uses[spells[i].get_level()]>0) {
					spell_uses[spells[i].get_level()]--;
					cast(this,spells[i]);
					return HEAL;
				}
			}
		}
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return HEAL;
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
	int act();
	char* toString();
};
Cleric::Cleric(char* s) {
	Caster::Caster(s);
	spell_type = S_CLERIC;
	int table[MAX_LEVEL][CLERIC_SPELL_MAX_LEVEL];
	FILE* file = fopen(CLERIC_SPELLSLOTS,"r");
	get_table(file,MAX_LEVEL,CLERIC_SPELL_MAX_LEVEL,table);
	fclose(file);
	for(int i=0;i<CLERIC_SPELL_MAX_LEVEL;i++) {
		spell_uses[i] = table[stats[LEVEL]][i];
	}
}
Cleric::~Cleric() {
	Caster::~Caster();
}
int Cleric::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>threshold) {
		return ATTACK;
	} else {
		if(this->can_cast()) {
			for(int i=0;i<MAX_SPELLS;i++) {
				if(spells[i]!=NULL && spells[i].is_heal() && spell_uses[spells[i].get_level()]>0) {
					spell_uses[spells[i].get_level()]--;
					cast(this,spells[i]);
					return HEAL;
				}
			}
		}
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return HEAL;
			}
		}
		return ATTACK;
	}
}
char* Cleric::toString() {
	return Caster::toString();	
}
//----------------------------------------------------------------------------------------------------------------------------------
class Druid : public Caster{
private:int wild_shape_time;		//Number of turns that Wild Shape lasts
	Animal* shape;		//When "wild_shape" is used, this is set to the Animal in which the druid transforms
public:	Druid(char*);
	~Druid();
	int wild_shape(s);		//Turn the druid into an animal for random turns
	int act();
	char* toString();
};
Druid::Druid(char* s) {
	Caster::Caster(s);
	spell_type = S_DRUID;
	int table[MAX_LEVEL][DRUID_SPELL_MAX_LEVEL];
	FILE* file = fopen(DRUID_SPELLSLOTS,"r");
	get_table(file,MAX_LEVEL,DRUID_SPELL_MAX_LEVEL,table);
	fclose(file);
	for(int i=0;i<DRUID_SPELL_MAX_LEVEL;i++) {
		spell_uses[i] = table[stats[LEVEL]][i];
	}
	shape = NULL;
	wild_shape_time = 10*stats[level];
}
Druid::~Druid() {
	delete(shape);
	Caster::~Caster();
}
int Druid::wild_shape(char* s) {
	if(wild_shape_time == 0 || stats[LEVEL]<5) {
		shape = NULL;
		return false;
	}
	if(shape == NULL) {
		shape = new Animal(s);
	}
	wild_shape_time--;
	return true;
}
int Druid::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>threshold) {
		if(shape!=NULL) {
			this->wild_shape();
		}
		return ATTACK;
	} else {
		if(this->can_cast()) {
			for(int i=0;i<MAX_SPELLS;i++) {
				if(spells[i]!=NULL && spells[i].is_heal() && spell_uses[spells[i].get_level()]>0) {
					spell_uses[spells[i].get_level()]--;
					cast(this,spells[i]);
					return NOTHING;
				}
			}
		}
		if(current_hp<5 && shape!=NULL) {
			wild_shape_time = 0;
		}
		if(shape==NULL) {
			for(int i=0;i<MAX_ITEMS;i++) {
				if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
					heal(this,items[i].get_value());
					return NOTHING;
				}
			}
		}
		char* animal = get_random_animal();
		this->wild_shape();
		return ATTACK;
	}
}
char* Druid::toString() {
	return Caster::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Sorcerer : public Caster{
public:	Sorcerer(char*);
	~Sorcerer();
	int act();
	char* toString();
};
Sorcerer::Sorcerer(char* s) {
	Caster::Caster(s);
	spell_type = S_ARCANE;
	int table[MAX_LEVEL][ARCANE_SPELL_MAX_LEVEL];
	FILE* file = fopen(SORCERER_SPELLSLOTS,"r");
	get_table(file,MAX_LEVEL,ARCANE_SPELL_MAX_LEVEL,table);
	fclose(file);
	for(int i=0;i<ARCANE_SPELL_MAX_LEVEL;i++) {
		spell_uses[i] = table[stats[LEVEL]][i];
	}
}
Sorcerer::~Sorcerer() {
	Caster::~Caster();
}
int Sorcerer::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>threshold) {
		if((rand()%3)!=0 && this->can_cast()) {
			return CAST;
		} else {
			return ATTACK;
		}
	} else {
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return HEAL;
			}
		}
		if((rand()%3)!=0 && this->can_cast()) {
			return CAST;
		} else {
			return ATTACK;
		}
	}
}
char* Sorcerer::toString() {
	return Caster::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Wizard : public Caster{
public:	Wizard(char*);
	~Wizard();
	int act();
	char* toString();
};
Wizard::Wizard(char* s) {
	Caster::Caster(s);
	spell_type = S_ARCANE;
	int table[MAX_LEVEL][ARCANE_SPELL_MAX_LEVEL];
	FILE* file = fopen(SORCERER_SPELLSLOTS,"r");
	get_table(file,MAX_LEVEL,ARCANE_SPELL_MAX_LEVEL,table);
	fclose(file);
	for(int i=0;i<ARCANE_SPELL_MAX_LEVEL;i++) {
		spell_uses[i] = table[stats[LEVEL]][i];
	}
}
Wizard::~Wizard() {
	Caster::~Caster();
}
int Wizard::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>threshold) {
		if((rand()%5)!=0 && this->can_cast()) {
			return CAST;
		} else {
			return ATTACK;
		}
	} else {
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return HEAL;
			}
		}
		if((rand()%3)!=0 && this->can_cast()) {
			return CAST;
		} else {
			return ATTACK;
		}
	}
}
char* Wizard::toString() {
	return Caster::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Monk : public Entity{
private:int body_integrity();
public:	Monk(char*);
	~Monk();
	int act();
	char* toString();
};
Monk::Monk(char* s) {
	Entity::Entity(s);
	if(stats[LEVEL]>6) {
		body_integrity = stats[LEVEL]*2;
	} else {
		body_integrity = 0;
	}
}
int Monk::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>=threshold) {
		return ATTACK;
	} else {
		if(body_integrity>0) {
			while(body_integrity>0 && current_hp<stats[HP]) {
				current_hp++;
				body_integrity--;
			}
			return HEAL;
		}
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return HEAL;
			}
		}
		return ATTACK;
	}
}
char* Monk::toString() {
	return Entity::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Fighter : public Entity{
public:	Fighter(char*);
	~Fighter();
	int act();
	char* toString();
};
Fighter::Fighter(char* s) {
	Entity::Entity(s);
}
Fighter::~Fighter() {
	Entity::~Entity(s);
}
int Fighter::act() {
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
				return HEAL;
			}
		}
		return ATTACK;
	}
}
char* Fighter::toString() {
	return Entity::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Ranger : public Caster{
public:	Ranger(char*);
	~Ranger();
	int act();
	char* toString();
};
Ranger::Ranger(char* s) {
	Caster::Caster(s);
	spell_type = S_RANGER;
	int table[MAX_LEVEL][RANGER_SPELL_MAX_LEVEL];
	FILE* file = fopen(RANGER_SPELLSLOTS,"r");
	get_table(file,MAX_LEVEL,RANGER_SPELL_MAX_LEVEL,table);
	fclose(file);
	for(int i=0;i<RANGER_SPELL_MAX_LEVEL;i++) {
		spell_uses[i] = table[stats[LEVEL]][i];
	}
}
Ranger::~Ranger() {
	Caster::~Caster();
}
int Ranger::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>threshold) {
		return ATTACK;
	} else {
		if(this->can_cast()) {
			for(int i=0;i<MAX_SPELLS;i++) {
				if(spells[i]!=NULL && spells[i].is_heal() && spell_uses[spells[i].get_level()]>0) {
					spell_uses[spells[i].get_level()]--;
					cast(this,spells[i]);
					return NOTHING;
				}
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
char* Ranger::toString() {
	return Caster::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Rogue : public Entity{
private:bool hidden;
public:	Rogue(char*);
	~Rogue();
	int act();
	char* toString();
};
Rogue::Rogue(char* s) {
	Entity::Entity(s);
	hidden = false;
}
Rogue::~Rogue() {
	Entity::~Entity(s);
}
int Rogue::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp<threshold) {
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return HEAL;
			}
		}
	}
	if(!hidden) {
		if(rand()%4==3) {
			hidden=true;
			stats[DEX] = stats[DEX] + 1000;
			return ABILITY;
		}
	} else {
		hidden = false;
		stats[DEX] = stats[DEX] - 1000;
		return ATTACK;
	}
}
char* Rogue::toString() {
	return Entity::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Paladin : public Caster{
private:int lay_hand;
public:	Paladin(char*);
	~Paladin();
	int act();
	char* toString();
};
Paladin::Paladin(char* s) {
	Caster::Caster(s);
	spell_type = S_PALADIN;
	int table[MAX_LEVEL][PALADIN_SPELL_MAX_LEVEL];
	FILE* file = fopen(PALADIN_SPELLSLOTS,"r");
	get_table(file,MAX_LEVEL,PALADIN_SPELL_MAX_LEVEL,table);
	fclose(file);
	for(int i=0;i<PALADIN_SPELL_MAX_LEVEL;i++) {
		spell_uses[i] = table[stats[LEVEL]][i];
	}
	lay_hand = 0;
	if(get_modifier(stats[CHA])>0) {
		lay_hand = stats[LEVEL] * get_modifier(stats[CHA]);
	}
}
Paladin::~Paladin() {
	Entity::~Entity(s);
}
int Paladin::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>=threshold) {
		return ATTACK;
	} else {
		if(lay_hand>0) {
			while(lay_hand>0 && current_hp<stats[HP]) {
				current_hp++;
				lay_hand--;
			}
			return HEAL;
		}
		if(this->can_cast()) {
			for(int i=0;i<MAX_SPELLS;i++) {
				if(spells[i]!=NULL && spells[i].is_heal() && spell_uses[spells[i].get_level()]>0) {
					spell_uses[spells[i].get_level()]--;
					cast(this,spells[i]);
					return NOTHING;
				}
			}
		}
		for(int i=0;i<MAX_ITEMS;i++) {
			if(items[i]!=NULL && items[i].is_heal() && items[i].use()) {
				heal(this,items[i].get_value());
				return HEAL;
			}
		}
		return ATTACK;
	}
}
char* Paladin::toString() {
	return Entity::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Monster : public Entity{
private:int natural_ac;
public:	Monster(char*);
	~Monster();
	int act();
	char* toString();
};
Monster::Monster(char* s) {
	Entity::Entity(s);
	//aggiungere natural_ac
}
Monster::~Monster() {
	Entity::~Entity(s);
}
int Monster::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	return ATTACK;
}
char* Monster::toString() {
	//aggiungere natural_ac
	return Entity::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Animal : public Entity{
public:	Animal(char*);
	~Animal();
	bool act();		//The animal plays his turn, returns false if character cannot act (either by indecision or death)
	char* toString();
};
