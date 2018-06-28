/*
Author: Lorenzo Damiano, aka Zeren
Github: https://github.com/SoStealth
*/
//----------------------------------------------------------------------------------------------------------------------------------
/*
Elements
Questa libreria contiene le classi usate per rappresentare l'equipaggiamento del gioco.
*/

//include
#include "rules.hpp"

//MAX constants
#define MAX_BUFFER 4096		//Massima lunghezza delle stringhe di buffer e dei serializzatori

//Spells constants
#define S_ARCANE 0		//Identifica un incantesimo arcano
#define S_CLERIC 1		//Identifica un incantesimo da chierico
#define S_BARD 2		//Identifica un incantesimo da bardo
#define S_RANGER 3		//Identifica un incantesimo da ranger
#define S_PALADIN 4		//Identifica un incantesimo da paladino
#define S_DRUID 5		//Identifica un incantesimo da druido

//Throw describers
#define ROLL_DESC "1d20"	//Descrittore dei tiri per colpire
#define USES_DESC "1d10"	//Descrittore dei tiri per determinare il numero di oggetti

//----------------------------------------------------------------------------------------------------------------------------------
/*
Armor
Questa classe contiene attributi e metodi per rappresentare l'equipaggiamento difensivo.
*/
class Armor{
private:char* name;	//Nome dell'armatura
	int ac_bonus;	//Bonus alla classe armatura fornito
	int dex_max;	//Destrezza massima che può avere il portatore dell'armatura
	int magic_fail;	//Percentuale di fallimento di incantesimi mentre si indossa l'armatura
public:	Armor(char*);	//Riceve una stringa serializzata che rappresenta un'armatura
	~Armor();	//Distruttore
	char* get_name();	//Restituisce una copia del nome, necessita di free()
	int get_ac_bonus();	//Restituisce il bonus alla classe armatura
	int get_dex_max();	//Restituisce la destrezza massima
	int get_magic_fail();	//Restituisce la percentuale di fallimento incantesimi
	char* toString();	//Serializzatore
};
Armor::Armor(char* s) {
	char** temp;	//Contenitore per il risultato della split
	split(s,',',&temp);	//Splitta il serializzatore in base alla virgla
	name = strdup(temp[0]);	//Assegna i vari valori
	ac_bonus = atoi(temp[1]);
	dex_max = atoi(temp[2]);
	magic_fail = atoi(temp[3]);
	free(temp);	//Dealloca il contenitore
}
Armor::~Armor() {
	free(name);	//Il nome è assegnato con una strdup()
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
Questa classe definisce i tipi di attacco e gli attributi e i metodi per utilizzarli.
*/
class Attack{
private:char* name;		//Nome dell'attacco
	char* damage;			//Danno dell'attacco (descrittore)
	int crit_value;			//Moltiplicatore da applicare in caso di colpo critico
	int crit_range;			//Range del colpo critico
	int scaling;			//Statistica in cui scala il danno dell'attacco
public: Attack(char*);			//Riceve una stringa serializzata
	~Attack();			//Distruttore
	char* get_name();	//Restituisce il nome dell'attacco
	int get_scaling();	//Restituisce lo scaling
	int get_crit_value();
	int get_crit_range();
	char* get_damage();
	/* Genera un attacco con l'arma e restituisce il tiro per colpire e i danni */
	void make_attack(int*,int*,int,int);
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
int Attack::get_crit_value() {
	return crit_value;
}
int Attack::get_crit_range() {
	return crit_range;
}
char* Attack::get_damage() {
	return strdup(damage);
}
void Attack::make_attack(int* attack_roll, int* damage_roll, int bab, int scale) {
	int roll = throw_dice(ROLL_DESC);	//Tiro del dado per colpire (1d20)
	int crit = 20 - crit_range;		//Imposta il valore da superare per il colpo critico
	if(roll>crit) {		//Se è un colpo critico (supera il valore di critico)
		*damage_roll = throw_dice(damage)*crit_value + scale;	//genera il danno, lo moltiplica
																// per il valore di critico e somma il bonus
	} else {	//Se non è un colpo critico
		*damage_roll = throw_dice(damage) + scale;	//genera il danno e somma il bonus
	}
	*attack_roll = roll + bab + scale;	//genera il valore del tiro per colpire sommando bab e scaling
}
char* Attack::toString() {
	char ret[MAX_BUFFER];
	sprintf(ret,"%s,%s,%d,%d,%d",name,damage,crit_value,crit_range,scaling);
	return strdup(ret);		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
/*
Item
Contiene gli attributi e i metodi per gestire gli oggetti.
Come oggetti si intende oggetti consumabili utili ai fini del combattimento.
*/
class Item{
private:char* name;	//Nome dell'oggetto
	char* value;	//Descrittore del valore dell'oggetto (sia per danni che per cure)
	int uses;	//Numero di utilizzi rimasti per l'oggetto, generato randomicamente
	int heal;	//Indica se l'oggetto è un oggetto di cura o di danno
public:	Item(char*);
	~Item();
	char* get_name();	//Restituisce il nome dell'oggetto
	char* get_value();	//Restituisce il descrittore dell'oggetto
	int get_uses();		//Restituisce il numero di utilizzi dell'oggetto
	void refull_uses();	//Ricarica gli utilizzi dell'oggetto, sempre randomicamente
	bool is_heal();		//Indica se l'oggetto è di cura oppure no
	bool use();		//Controlla se sono rimasti utilizzi: se sì, riduce di 1 gli utilizzi e restituisce true; altrimenti, false
	char* toString();
};
Item::Item(char* s) {
	char** temp;
	split(s,',',&temp);
	name = strdup(temp[0]);
	value = strdup(temp[1]);
	heal = atoi(temp[2]);
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
Questa classe contiene attributi e metodi per gestire gli incantesimi.
Al momento, solo incantesimi di cura e danno sono inclusi.
*/
class Spell{
private:char* name;	//Nome dell'incantesimo
	int level;	//Livello dell'incantesimo
	char* value;	//Descrittore del valore dell'incantesimo
	int save_type;	//Tipo di tiro salvezza da effettuare se si vuole salvarsi dall'incantesimo
	int heal;	//Determina se la spell è di cura o no
public:	Spell(char*);
	~Spell();
	char* get_name();	//Restituisce il nome dell'incantesimo
	int get_level();	//Restituisce il livello dell'incantesimo
	char* get_value();	//Restituisce il valore dell'incantesimo
	int get_save_type();	//Restituisce il tipo di tiro salvezza
	int is_heal();		//Indica se l'incantesimo è di cura o no
	int get_dc(int);	//Restituisce la classe difficoltà dell'incantesimo (riceve il livello dell'incantatore)
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
int Spell::get_dc(int caster_level) {	//La DC o Classe Difficoltà è il valore da superare con il tiro
										// salvezza per evitare l'incantesimo
	int ret;
	ret = 10 + caster_level + level;	//La DC si ottiene sommando 10, livello dell'incantesimo e livello
										// dell'incantatore
	return ret;
}
char* Spell::toString() {
	char ret[MAX_BUFFER];
	sprintf(ret,"%s,%d,%s,%d,%d",name,level,value,save_type,heal);
	return strdup(ret);		//FREE
}
//----------------------------------------------------------------------------------------------------------------------------------
