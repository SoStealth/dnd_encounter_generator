/*
Author: Lorenzo Damiano, aka Zeren
Github: https://github.com/SoStealth
*/
//----------------------------------------------------------------------------------------------------------------------------------
/*
Entities
This library contains classes used to represent the entities in the game.
There are 3 types of entities: characters (represented by classes), monsters and animals
*/

#include "equipments.hpp"

#define DEATH_VALUE -10

//Tabs constants
#define BARD_SPELLSLOTS	"tabs/bard_spellslots.csv"		//Tab for bard spellslots
#define CLERIC_SPELLSLOTS "tabs/cleric_spellslots.csv"	//Tab for cleric spellslots
#define DRUID_SPELLSLOTS "tabs/druid_spellslots.csv"		//Tab for druid spellslots
#define SORCERER_SPELLSLOTS "tabs/sorcerer_spellslots.csv"	//Tab for sorcerer spellslots
#define RANGER_SPELLSLOTS "tabs/ranger_spellslots.csv"	//Tab for ranger spellslots
#define PALADIN_SPELLSLOTS "tabs/paladin_spellslots.csv"	//Tab for paladin spellslots
#define WIZARD_SPELLSLOTS "tabs/wizard_spellslots.csv"	//Tab for wizard spellslots

//Action constants
#define ATTACK 1	//The character decides to attack
#define HEAL 2		//The character decides to heal another character
#define CAST 3		//The character decides to cast a spell
#define NOTHING 4	//The character decides to do nothing (or can't do nothing)
#define MUSIC 5		//Bards: the character starts playing

//Max constants
#define MAX_LEVEL 20		//Maximum level allowed
#define MAX_ARMOR 2		//Max number of armor an entity can have
#define MAX_ATTACKS 5		//Max number of attacks an entity can have
#define MAX_ITEMS 10		//Max number of equipments an entity can carry
#define MAX_SPELLS 40
//Every max level for spell is increased by 1 for array length purposes
#define SPELL_MAX_LEVEL 10	//Max level of any spell
#define BARD_SPELL_MAX_LEVEL 7		//Max level of a bard spell
#define ARCANE_SPELL_MAX_LEVEL 10	//Max level of an arcane spell
#define CLERIC_SPELL_MAX_LEVEL 10	//Max level of a cleric spell
#define DRUID_SPELL_MAX_LEVEL 10	//Max level of a druid spell
#define RANGER_SPELL_MAX_LEVEL 5	//Max level of a ranger spell
#define PALADIN_SPELL_MAX_LEVEL 5	//Max level of a paladin spell

//In case someone needs to attack but hasn't got any attacks
#define MELEE "Melee attack,4,0,0,2"

//------------------------------------------------------------------------------------------------------------------

/*
Entity
This class contains basic attributes and methods for every entity in the game.
*/
class Entity{
protected:char* name;	//Name of the entity
	int stats[N_STATS];	//Stats of the entity
	int current_hp;		//Entity's current hp
	bool alive;
	Armor* armors[MAX_ARMOR];	//Armors inventory
	Attack* attacks[MAX_ATTACKS];	//Attacks used by the entity
	Item* items[MAX_ITEMS];		//Items carried by the entity
public:	Entity(char*);		//Receives serialized entity string
	~Entity();
	char* get_name();	//Receives name of entity
	int get_stat(int);	//Receives parameter identifier
	int get_ac();
	int get_current_hp();
	bool is_alive();	//If current_hp > 0 true, else false
	bool equip_armor(char*);	//True if armor gets equipped, false if there is no armor space
	Armor* get_armor(int);	//Receives armor identifier
	bool unequip_armor(int);	//Receives armor identifier, true if armor has been unequipped, false if there is no armor
	bool equip_attack(char*);	//True if attack gets equipped, false if there is no attack space
	Attack* get_attack(int);		//Receives attack identifier
	bool unequip_attack(int);	//Receives attack identifier, true if attack has been unequipped, false if there is no attack
	bool equip_item(char*);	//True if item gets equipped, false if there is no item space
	Item* get_item(int);	//Receives item identifier
	bool unequip_item(int);	//Receives item identifier, true if item has been unequipped, false if there is no item
	bool hit(int);		//Hits the entity and lowers his hp, return false if damage has been neglected
	bool heal(int);		//Heals the entity by an amount, return false if heal has been neglected
	/* Act methods: actions an entity can take */
	bool attack(Entity*,Attack*);
	bool heal_with_item();
	char* toString();	//serializator
};
Entity::Entity(char* s) {
	char** temp;
	int i;
	split(s,',',&temp);
	name = strdup(temp[0]);
	for(i=0;i<N_STATS;i++) {		//Only base stats are assigned, the rest can be calculated
		stats[i] = atoi(temp[i+1]);
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
	stats[FORT] = stats[FORT] + get_modifier(stats[CON]);
	stats[REFL] = stats[REFL] + get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] + get_modifier(stats[WIS]);
	alive = true;
}
Entity::~Entity() {
	int i;
	free(name);
	for(i=0;i<MAX_ARMOR;i++) {	//Sets all armor objects to NULL
		if(armors[i]!=NULL) {
			free(armors[i]);
		}
	}
	for(i=0;i<MAX_ATTACKS;i++) {	//Sets all attack objects to NULL
		if(attacks[i]!=NULL) {
			free(attacks[i]);
		}
	}
	for(i=0;i<MAX_ITEMS;i++) {	//Sets all item objects to NULL
		if(items[i]!=NULL) {
			free(items[i]);
		}
	}
}
char* Entity::get_name() {
	return strdup(name);	//FREE
}
int Entity::get_stat(int id) {
	if(id<N_STATS) {		//Checks if id is within range
		return stats[id];
	} else {
		return NULL;
	}
}
int Entity::get_ac() {
	int ret;
	ret = BASE_AC;
	for(int i=0;i<MAX_ARMOR;i++) {	//Deletes all armor objects
		if(armors[i]!=NULL) {
			ret = ret + armors[i]->get_ac_bonus();
		}
	}
	ret = ret + get_modifier(stats[DEX]);
	return ret;
}
int Entity::get_current_hp() {
	return current_hp;
}
bool Entity::is_alive() {
	return alive;
}
bool Entity::equip_armor(char* s) {	//Receives serialized armor string
	int i;
	for(i=0;i<MAX_ARMOR;i++) {
		if(armors[i]==NULL) {
			armors[i] = new Armor(s);
			return true;
		}
	}
	return false;
}
Armor* Entity::get_armor(int id) {
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
		}
	}
	return false;
}
bool Entity::equip_attack(char* s) {	//Receives serialized attack string
	int i;
	for(i=0;i<MAX_ATTACKS;i++) {
		if(attacks[i]==NULL) {
			attacks[i] = new Attack(s);
			return true;
		}
	}
	return false;
}
Attack* Entity::get_attack(int id) {
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
		}
	}
	return false;
}
/*
equip_item method
Receives a serialized item string and creates an item in the first free item slot
Return false if all item slots are full
*/
bool Entity::equip_item(char* s) {	//Receives serialized item string
	int i;
	for(i=0;i<MAX_ITEMS;i++) {
		if(items[i]==NULL) {
			items[i] = new Item(s);
			return true;
		}
	}
	return false;
}
/*
get_item method
Returns the item corresponding to the 'id' parameter
In case the item is not found or the id is out of range, returns NULL
*/
Item* Entity::get_item(int id) {
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
bool Entity::hit(int value) {
	if(value<=0) return false;
	current_hp = current_hp - value;
	if(current_hp<=DEATH_VALUE) {
		alive = false;
	}
	return true;
}
/*
heal method
The current entity gets healed by 'value' number of hp
In case the current number of hp is higher than the maximum number of hp of the entity, it gets lowered to match that value
*/
bool Entity::heal(int value) {
	if(value==0) return false;
	current_hp = current_hp + value;
	if(current_hp>stats[HP]) {
		current_hp = stats[HP];
	}
	return true;
}
bool Entity::attack(Entity* target) {
	int attack_roll;
	int damage_roll;
	Attack* attack = NULL;
	for(int i=0;i<MAX_ATTACKS;i++) {
		if(attacks[i]!=NULL) {
			attack = new Attack(attacks[i]->toString());
		}
	}
	if(attack == NULL) {
		attack = new Attack(MELEE);
	}
	int scale = get_modifier(stats[attack->get_scaling()]);
	attack->make_attack(&attack_roll,&damage_roll,stats[BAB],scale);
	int critical = attack_roll - stats[BAB] - scale;
	if(attack_roll > target->get_ac() || critical == 20) {
		if(damage_roll<1)
			damage_roll = 1;
		target->hit(damage_roll);
		return true;
	}
	return false;
}
bool Entity::heal_with_item() {
	for(int i=0;i<MAX_ITEMS;i++) {
		if(items[i]!=NULL && items[i]->is_heal() && items[i]->use()) {
			heal(items[i]->get_value());
			return true;
		}
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
	stats[FORT] = stats[FORT] - get_modifier(stats[CON]);
	stats[REFL] = stats[REFL] - get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] - get_modifier(stats[WIS]);
	char ret[MAX_BUFFER];
	sprintf(ret,"%s",name);
	for(int i=0;i<N_STATS;i++) {		//Only base stats are serialized, because the rest can be calculated
		sprintf(ret,"%s,%d",ret,stats[i]);
	}
	stats[FORT] = stats[FORT] + get_modifier(stats[CON]);
	stats[REFL] = stats[REFL] + get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] + get_modifier(stats[WIS]);
	return strdup(ret);		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Caster
This class, derived from Entity, represents entities who can use spells.
Contains known spells data and the method "cast" to be able to use them.
*/
class Caster : public Entity{
protected:Spell* spells[MAX_SPELLS];
	int spell_uses[SPELL_MAX_LEVEL];
	int spell_type;
	void load_spellslots(int,char*);
public:	Caster(char*);		//Receives serialized caster string
	~Caster();
	bool equip_spell(char*);	//True if spell gets equipped, false if there is no spell space
	Spell* get_spell(int);	//Receives spell identifier
	bool unequip_spell(int);	//Receives spell identifier, true if spell has been unequipped, false if there is no spell
	bool can_cast();
	/* Act methods */
	bool cast(Entity*,Spell*);	//Target, spell
	bool heal_with_spell();
	char* toString();	//Serializator
};
Caster::Caster(char* s) : Entity(s) {
	for(int i=0;i<SPELL_MAX_LEVEL;i++) {
		spell_uses[i] = 0;
	}
	for(int i=0;i<MAX_SPELLS;i++) {	//Sets all item objects to NULL
		spells[i]=NULL;
	}
}
void Caster::load_spellslots(int class_spell_max_level, char* filename) {
	int** table;
	table = get_table(filename,MAX_LEVEL,class_spell_max_level);
	for(int i=0;i<class_spell_max_level;i++) {
		spell_uses[i] = table[stats[LEVEL]][i];
	}
}
Caster::~Caster() {
	for(int i=0;i<MAX_SPELLS;i++) {	//Sets all item objects to NULL
		if(spells[i]!=NULL) {
			delete(spells[i]);
		}
	}
}
bool Caster::equip_spell(char* s) {	//Receives serialized spell string
	int i;
	for(i=0;i<MAX_SPELLS;i++) {
		if(spells[i]==NULL) {
			spells[i] = new Spell(s);
			return true;
		}
	}
	return false;
}
Spell* Caster::get_spell(int id) {
	if(id<MAX_SPELLS) {		//Checks if id is within range
		return spells[id];
	} else {
		return NULL;
	}
}
bool Caster::unequip_spell(int id) {
	if(id<MAX_SPELLS) {		//Checks if id is within range
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
bool Caster::cast(Entity* target, Spell* spell) {
	int cd = spell->get_dc(stats[LEVEL]);
	int save = throw_dice(D20) + target->get_stat(spell->get_save_type());
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
bool Caster::heal_with_spell() {
	if(this->can_cast()) {
		for(int i=0;i<MAX_SPELLS;i++) {
			if(spells[i]!=NULL && spells[i]->is_heal() && spell_uses[spells[i]->get_level()]>0) {
				spell_uses[spells[i]->get_level()]--;
				cast(this,spells[i]);
				return true;
			}
		}
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
Barbarian::Barbarian(char* s) : Entity(s) {
	rage_time = 3 + get_modifier(stats[CON]);
}
Barbarian::~Barbarian() {
}
/* Send the barbarian into rage status. If this is called, the method "end_rage" will be called every end turn 
to check wether the barbarian is still in rage or not */
bool Barbarian::rage() {
	if(rage_time==0) {
		return false;
	}
	if(stats[LEVEL]<11) {
		current_hp = current_hp + (2*stats[LEVEL]);
		stats[STR] += 4;
		stats[CON] += 4;
		stats[WILL] += 2;
		return true;
	}
	if(stats[LEVEL]<20) {
		current_hp = current_hp + (2*stats[LEVEL]);
		stats[STR] += 6;
		stats[CON] += 6;
		stats[WILL] += 3;
		return true;
	}
	current_hp = current_hp + (2*stats[LEVEL]);
	stats[STR] += 8;
	stats[CON] += 8;
	stats[WILL] += 4;
	return true;
}
/* If the barbarian can still be in rage, decreases rage_time and return false. Else, reset stats and returns true */
bool Barbarian::end_rage() {
	if(rage_time>0) {
		rage_time--;
		return false;
	}
	if(stats[LEVEL]<11) {
		current_hp = current_hp - (2*stats[LEVEL]);
		stats[STR] -= 4;
		stats[CON] -= 4;
		stats[WILL] -= 2;
		return true;
	}
	if(stats[LEVEL]<20) {
		current_hp = current_hp - (2*stats[LEVEL]);
		stats[STR] -= 6;
		stats[CON] -= 6;
		stats[WILL] -= 3;
		return true;
	}
	current_hp = current_hp - (2*stats[LEVEL]);
	stats[STR] -= 8;
	stats[CON] -= 8;
	stats[WILL] -= 4;
	return true;
}
int Barbarian::act() {
	int threshold;
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>=threshold) {
		return ATTACK;
	} else {
		if(heal_with_item()) {
			return NOTHING;
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
Bard::Bard(char* very_bardic_string) : Caster(very_bardic_string) {
	spell_type = S_BARD;
	load_spellslots(BARD_SPELL_MAX_LEVEL,BARD_SPELLSLOTS);
	entertain = stats[LEVEL] + get_modifier(stats[CHA]) + 3;
}
Bard::~Bard() {
}
int Bard::play() {	//Plays music and tries to stop creatures from moving
	int ret;
	ret = throw_dice(D20) + get_modifier(stats[CHA]) + entertain;
	return ret;
}
int Bard::act() {	//In case someone needs healing, the bard will authomatically try to heal him
	int thrashold;
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
		if(heal_with_spell() || heal_with_item()) {
			return NOTHING;
		}
		return ATTACK;
	}
}
char* Bard::toString() {
	return Entity::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Cleric : public Caster{
public:	Cleric(char*);
	~Cleric();
	int act();
	char* toString();
};
Cleric::Cleric(char* s) : Caster(s) {
	spell_type = S_CLERIC;
	load_spellslots(CLERIC_SPELL_MAX_LEVEL,CLERIC_SPELLSLOTS);
}
Cleric::~Cleric() {
}
int Cleric::act() {
	int threshold;
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>threshold) {
		return ATTACK;
	} else {
		if(heal_with_spell() || heal_with_item()) {
			return NOTHING;
		}
		return ATTACK;
	}
}
char* Cleric::toString() {
	return Caster::toString();	
}
//----------------------------------------------------------------------------------------------------------------------------------
class Animal : public Entity{
public:	Animal(char*);
	~Animal();
	char* toString();
};
Animal::Animal(char* s) : Entity(s) {
}
Animal::~Animal() {
}
char* Animal::toString() {
	return Entity::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Druid : public Caster{
private:int wild_shape_time;		//Number of turns that Wild Shape lasts
		Animal* shape;		//When "wild_shape" is used, this is set to the Animal in which the druid transforms
public:	Druid(char*);
	~Druid();
	bool wild_shape(char*);		//Turn the druid into an animal for time turns
	bool is_animal();
	int act();
	char* toString();
};
Druid::Druid(char* s) : Caster(s) {
	spell_type = S_DRUID;
	load_spellslots(DRUID_SPELL_MAX_LEVEL,DRUID_SPELLSLOTS);
	shape = NULL;
	wild_shape_time = 10*stats[LEVEL];
}
Druid::~Druid() {
	if(shape!=NULL) {
		delete(shape);
	}
}
bool Druid::wild_shape(char* s) {
	if(wild_shape_time == 0 || stats[LEVEL]<5) {
		shape = NULL;
		return false;
	}
	if(shape == NULL) {
		shape = new Animal(s);
		return true;
	}
	wild_shape_time--;
	return true;
}
bool Druid::is_animal() {
	return (shape!=NULL);
}
int Druid::act() {
	int threshold;
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>threshold) {
		if(shape!=NULL) {
			this->wild_shape("");
		} else {
			char* animal = "Test animal,1,10,6,10,14,8,14,16,0,0,0,2";
			this->wild_shape(animal);
			return NOTHING;
		}
		return ATTACK;
	} else {
		if(heal_with_spell()) {
			return NOTHING;
		}
		if(current_hp<5 && shape!=NULL) {
			wild_shape_time = 0;
		}
		if(shape==NULL) {
			if(heal_with_item()) {
				return NOTHING;
			}
		}
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
Sorcerer::Sorcerer(char* s) : Caster(s) {
	spell_type = S_ARCANE;
	load_spellslots(ARCANE_SPELL_MAX_LEVEL,SORCERER_SPELLSLOTS);
}
Sorcerer::~Sorcerer() {
}
int Sorcerer::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	int threshold = stats[HP]/2;
	if(current_hp>threshold) {
		if((rand()%3)!=0 && this->can_cast()) {
			return CAST;
		} else {
			return ATTACK;
		}
	} else {
		if(heal_with_item()) {
			return NOTHING;
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
Wizard::Wizard(char* s) : Caster(s) {
	spell_type = S_ARCANE;
	load_spellslots(ARCANE_SPELL_MAX_LEVEL,WIZARD_SPELLSLOTS);
}
Wizard::~Wizard() {
}
int Wizard::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	int threshold = stats[HP]/2;
	if(current_hp>threshold) {
		if((rand()%5)!=0 && this->can_cast()) {
			return CAST;
		} else {
			return ATTACK;
		}
	} else {
		if(heal_with_item()) {
			return NOTHING;
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
private:int body_integrity;
public:	Monk(char*);
	~Monk();
	int act();
	char* toString();
};
Monk::Monk(char* s) : Entity(s) {
	if(stats[LEVEL]>6) {
		body_integrity = stats[LEVEL]*2;
	} else {
		body_integrity = 0;
	}
}
Monk::~Monk() {
}
int Monk::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	int threshold = stats[HP]/2;
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
		if(heal_with_item()) {
			return NOTHING;
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
Fighter::Fighter(char* s) : Entity(s) {
}
Fighter::~Fighter() {
}
int Fighter::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	int threshold = stats[HP]/2;
	if(current_hp>=threshold) {
		return ATTACK;
	} else {
		if(heal_with_item()) {
			return NOTHING;
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
Ranger::Ranger(char* s) : Caster(s) {
	spell_type = S_RANGER;
	load_spellslots(RANGER_SPELL_MAX_LEVEL,RANGER_SPELLSLOTS);
}
Ranger::~Ranger() {
}
int Ranger::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	int threshold = stats[HP]/2;
	if(current_hp>threshold) {
		return ATTACK;
	} else {
		if(heal_with_spell() || heal_with_item()) {
			return NOTHING;
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
	bool attack(Entity*,Attack*);
	char* toString();
};
Rogue::Rogue(char* s) : Entity(s) {
	hidden = false;
}
Rogue::~Rogue() {
}
int Rogue::act() {
	if(!is_alive()) {
		printf("Dead");
		return NOTHING;
	}
	int threshold = stats[HP]/2;
	if(current_hp<threshold) {
		if(heal_with_item()) {
			return NOTHING;
		}
	}
	if(!hidden) {
		if(rand()%3==2) {
			hidden = true;
			stats[DEX] = stats[DEX] + 1000;
		}
		return NOTHING;
	}
	return ATTACK;
}
bool Rogue::attack(Entity* target, Attack* attack) {
	int attack_roll;
	int damage_roll;
	if(hidden) {
		stats[DEX] = stats[DEX] - 1000;
	}
	int scale = get_modifier(stats[attack->get_scaling()]);
	attack->make_attack(&attack_roll,&damage_roll,stats[BAB],scale);
	if(hidden) {
		hidden = false;
		int bonus_dices = (stats[LEVEL]-1)/2+1;
		for(int k=0;k<bonus_dices;k++) {
			damage_roll = damage_roll + throw_dice(D6);
		}
	}
	int critical = attack_roll - stats[BAB] - scale;
	if(attack_roll > target->get_ac() || critical == 20) {
		if(damage_roll<1)
			damage_roll = 1;
		target->hit(damage_roll);
		return true;
	}
	return false;
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
Paladin::Paladin(char* s) : Caster(s) {
	spell_type = S_PALADIN;
	load_spellslots(PALADIN_SPELL_MAX_LEVEL,PALADIN_SPELLSLOTS);
	lay_hand = 0;
	if(get_modifier(stats[CHA])>0) {
		lay_hand = stats[LEVEL] * get_modifier(stats[CHA]);
	}
}
Paladin::~Paladin() {
}
int Paladin::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	int threshold = stats[HP]/2;
	if(current_hp>=threshold) {
		return ATTACK;
	} else {
		if(lay_hand>0) {
			while(lay_hand>0 && current_hp<stats[HP]) {
				current_hp++;
				lay_hand--;
			}
			return NOTHING;
		}
		if(heal_with_spell() || heal_with_item()) {
			return NOTHING;
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
Monster::Monster(char* s) : Entity(s) {
	char** temp;
	split(s,',',&temp);
	natural_ac = atoi(temp[13]);
}
Monster::~Monster() {
}
int Monster::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	return ATTACK;
}
char* Monster::toString() {
	char* ret;
	sprintf(ret,"%s,%d",Entity::toString(),natural_ac);
	return ret;
}

