/*
Author: Lorenzo Damiano, aka Zeren
Github: https://github.com/SoStealth
*/
//----------------------------------------------------------------------------------------------------------------------------------
/*
Entities
Questa libreria contiene le classi per rappresentare le entità del gioco (personaggi,animali,mostri)
*/

#include "equipments.hpp"

//Costanti per valori
#define DEATH_VALUE 0	//Valore sotto al quale il personaggio non può agire
#define BASE_AC 10		//Valore base della classe armatura

//Costanti di classe: servono per identificare il tipo di entità (usato per il cast dinamico)
#define BARBARIAN 0	
#define BARD 1
#define CLERIC 2
#define DRUID 3
#define SORCERER 4
#define WIZARD 5
#define MONK 6
#define FIGHTER 7
#define RANGER 8
#define ROGUE 9
#define PALADIN 10
#define ANIMAL 11
#define MONSTER 12

//Costanti per tabelle
#define BARD_SPELLSLOTS	"libs/tabs/bard_spellslots.csv"		//Tabella per gli spellslots del bardo
#define CLERIC_SPELLSLOTS "libs/tabs/cleric_spellslots.csv"	//Tabella per gli spellslots del chierico
#define DRUID_SPELLSLOTS "libs/tabs/druid_spellslots.csv"		//Tabella per gli spellslots del druido
#define SORCERER_SPELLSLOTS "libs/tabs/sorcerer_spellslots.csv"	//Tabella per gli spellslots dello stregone
#define RANGER_SPELLSLOTS "libs/tabs/ranger_spellslots.csv"	//Tabella per gli spellslots del ranger
#define PALADIN_SPELLSLOTS "libs/tabs/paladin_spellslots.csv"	//Tabella per gli spellslots del paladino
#define WIZARD_SPELLSLOTS "libs/tabs/wizard_spellslots.csv"	//Tabella per gli spellslots del mago

//Costanti per le azioni
#define ATTACK 1	//Il personaggio decide di attaccare
#define HEAL 2		//Il personaggio decide di curare sé stesso o qualcuno
#define CAST 3		//Il personaggio decide di lanciare un incantesimo
#define NOTHING 4	//Il personaggio non agisce
#define MUSIC 5		//Per bardi: il personaggio decide di suonare
#define HIDDEN 6	//Per ladri: il personaggio si nasconde per attaccare furtivamente

//Costanti per massimi
#define MAX_LEVEL 20		//Livello massimo consentito
#define MAX_ARMOR 2		//Massimo numero di oggetti difensivi per personaggio
#define MAX_ATTACKS 5		//Massimo numero di attacchi per personaggio
#define MAX_ITEMS 10		//Massimo numero di oggetti per personaggio
#define MAX_SPELLS 40		//Massimo numero di incantesimi conosciuti per personaggio
//Ogni livello massimo di spell è aumentato di 1 per questioni di lunghezza di array
#define SPELL_MAX_LEVEL 10	//Massimo livello per incantesimi generali
#define BARD_SPELL_MAX_LEVEL 7		//Massimo livello per gli incantesimi da bardo
#define ARCANE_SPELL_MAX_LEVEL 10	//Massimo livello per gli incantesimi arcani
#define CLERIC_SPELL_MAX_LEVEL 10	//Massimo livello per gli incantesimi da chierico
#define DRUID_SPELL_MAX_LEVEL 10	//Massimo livello per gli incantesimi da druido
#define RANGER_SPELL_MAX_LEVEL 5	//Massimo livello per gli incantesimi da ranger
#define PALADIN_SPELL_MAX_LEVEL 5	//Massimo livello per gli incantesimi da paladino

//Costanti di utilità
#define MELEE "Melee attack,4,0,0,2"	//Nel caso un pg non abbia attacchi, utilizza questo per attaccare

//Descrittori
#define FURTIVE_DESC "1d6"	//Bonus ai danni per il ladro se compie un attacco furtivo

//------------------------------------------------------------------------------------------------------------------

/*
Entity
Questa classe contiene gli attributi e i metodi per ogni entità nel gioco.
*/
class Entity{
protected:char* name;	//Nome dell'entità
	int stats[N_STATS];	//Statistiche dell'entità
	int current_hp;		//Punti vita rimasti all'entità
	bool alive;			//Indica se l'entità è viva
	Armor* armors[MAX_ARMOR];	//Inventario degli oggetti difensivi
	Attack* attacks[MAX_ATTACKS];	//Inventario degli attacchi
	Item* items[MAX_ITEMS];		//Inventario degli oggetti
public:	Entity(char*);
	char* get_name();	//Restituisce il nome dell'entità
	int get_stat(int);	//Restituisce la statistica corrispondente all'identificatore
	bool set_stat(int,int);	//Imposta una statistica ad un valore diverso
	int get_ac();	//Restituisce la classe armatura dell'entità
	int get_current_hp();	//Restituisce i punti vita attuali dell'entità
	void refull_hp();	//Restituisce tutti i punti vita all'entità
	void reset();		//Resetta l'entità (usato dopo un combattimento)
	bool is_alive();	//Indica se la creatura è viva
	bool equip_armor(char*);	//Equipaggia un oggetto difensivo
	Armor* get_armor(int);	//Trova e restituisce il puntatore ad un oggetto difensivo
	bool unequip_armor(int);	//Disequipaggia un oggetto difensivo
	bool equip_attack(char*);	//Equipaggia un attacco
	Attack* get_attack(int);		//Trova e restituisce il puntatore ad un attacco
	bool unequip_attack(int);	//Disequipaggia un attacco
	bool equip_item(char*);	//Equipaggai un oggetto consumabile
	Item* get_item(int);	//Trova e restituisce il puntatore ad un oggetto consumabile
	bool unequip_item(int);	//Disequipaggia un oggetto consumabile
	void refull_items();	//Ricarica gli usi di tutti i consumabili
	bool hit(int);		//Infligge danno all'entità per un valore fisso
	bool heal(int);		//Cura l'entità per un valore fisso
	/* Metodi per le azioni */
	int attack(Entity*);	//Attacca un bersaglio con un attacco (riceve il bersaglio)
	bool heal_with_item();	//Cerca un oggetto di cura e se lo trova, lo utilizza
	char* toString();	//serializator
	/* METODI DICHIARATI PER EREDITARIETA' */
	bool equip_spell(char*);	//Per i caster
	Spell* get_spell(int);	//Per i caster
	bool unequip_spell(int);	//Per i caster
	bool can_cast();	//Per i caster
	int cast(Entity*,bool);	//Per i caster
	bool heal_with_spell();	//Per i caster
	bool rage();	//Per i barbari
	bool end_rage();	//Per i barbari
	bool wild_shape(char*);	//Per i druidi
	bool is_animal();	//Per i druidi
	int play();		//Per i bardi
	int act();		//Per ogni personaggio o mostro
	/* ----------------------------------- */
	virtual ~Entity() {	//Distruttore virtuale: serve a permettere il dynamic_cast
		int i;
		free(name);
		for(i=0;i<MAX_ARMOR;i++) {	//Dealloca tutte le armature
			if(armors[i]!=NULL) {
				free(armors[i]);
			}
		}
		for(i=0;i<MAX_ATTACKS;i++) {	//Dealloca tutti gli attacchi
			if(attacks[i]!=NULL) {
				free(attacks[i]);
			}
		}
		for(i=0;i<MAX_ITEMS;i++) {	//Dealloca tutti gli oggetti
			if(items[i]!=NULL) {
				free(items[i]);
			}
		}
	}
};
Entity::Entity(char* s) {
	char** temp;
	int i;
	split(s,',',&temp);
	name = strdup(temp[0]);
	for(i=0;i<N_STATS;i++) {
		stats[i] = atoi(temp[i+1]);
	}
	current_hp = stats[HP];
	for(i=0;i<MAX_ARMOR;i++) {	//Imposta tutti i puntatori agli oggetti difensivi a NULL
		armors[i]=NULL;
	}
	for(i=0;i<MAX_ATTACKS;i++) {	//Imposta tutti i puntatori agli attacchi a NULL
		attacks[i]=NULL;
	}
	for(i=0;i<MAX_ITEMS;i++) {	//Imposta tutti i puntatori ai consumabili a NULL
		items[i]=NULL;
	}
	if(stats[LEVEL]>MAX_LEVEL) {	//Controlla il livello per verificare che non sia troppo alto
		stats[LEVEL] = MAX_LEVEL;
	}
	stats[FORT] = stats[FORT] + get_modifier(stats[CON]);	//Aggiorna i tiri salvezza in base alle stats
	stats[REFL] = stats[REFL] + get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] + get_modifier(stats[WIS]);
	alive = true;	//L'entità è viva
}
char* Entity::get_name() {
	return strdup(name);	//FREE
}
int Entity::get_stat(int id) {
	int ret;
	stats[FORT] = stats[FORT] - get_modifier(stats[CON]);	//Ritorna ai valori base dei tiri salvezza
	stats[REFL] = stats[REFL] - get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] - get_modifier(stats[WIS]);
	if(id<N_STATS) {		//Controlla se l'ID è in range
		ret = stats[id];
	} else {
		ret = -1;
	}
	stats[FORT] = stats[FORT] + get_modifier(stats[CON]);	//Riassegna i corretti punteggi
	stats[REFL] = stats[REFL] + get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] + get_modifier(stats[WIS]);
	return ret;
}
bool Entity::set_stat(int id, int value) {
	if(id<N_STATS && value>0) {		//Controlla se l'ID è in range e il valore è maggiore di 0
		stats[id] = value;
		return true;
	} else {
		return false;
	}
}
int Entity::get_ac() {
	int ret;
	ret = BASE_AC;	//Aggiunge la classe armatura base
	for(int i=0;i<MAX_ARMOR;i++) {
		if(armors[i]!=NULL) {
			ret = ret + armors[i]->get_ac_bonus(); //Aggiunge tutti i bonus degli oggetti difensivi
		}
	}
	ret = ret + get_modifier(stats[DEX]);	//Aggiunge il modificatore della destrezza
	return ret;
}
int Entity::get_current_hp() {
	return current_hp;
}
void Entity::refull_hp() {
	alive = true;	//Reimposta la creatura a viva
	current_hp = stats[HP];
}
void Entity::reset() {
	this->refull_hp();	//Reimposta i punti vita
	for(int i=0;i<MAX_ITEMS;i++) {	//Reimposta gli usi di tutti gli oggetti
		if(items[i]!=NULL) {
			items[i]->refull_uses();
		}
	}
}
bool Entity::is_alive() {
	return alive;
}
bool Entity::equip_armor(char* s) {	//Riceve una stringa serializzata di un oggetto difensivo
	int i;
	for(i=0;i<MAX_ARMOR;i++) {
		if(armors[i]==NULL) {	//Controlla se c'è uno slot per armature libero
			armors[i] = new Armor(s);	//Aggiunge l'armatura all'inventario
			return true;
		}
	}
	return false;	//Nessuno slot libero
}
Armor* Entity::get_armor(int id) {	//Riceve la posizione dell'armatura nell'inventario
	if(id<MAX_ARMOR) {		//Controlla se l'ID è in range
		return armors[id];	//Restituisce il puntatore all'armatura
	} else {
		return NULL;
	}
}
bool Entity::unequip_armor(int id) {	//Riceve la posizione dell'armatura nell'inventario
	if(id<MAX_ARMOR) {		//Controlla se l'ID è in range
		if(armors[id]!=NULL) {
			delete(armors[id]);	//Dealloca l'armatura
			armors[id]=NULL;	//In questo modo il controllo in equip_armor() segnerà lo slot libero 
			return true;
		}
	}
	return false;	//L'armatura che si sta tentando di disequipaggiare non è presente
}
bool Entity::equip_attack(char* s) {	//Riceve una stringa serializzata di un attacco
	int i;
	for(i=0;i<MAX_ATTACKS;i++) {
		if(attacks[i]==NULL) {	//Controlla se c'è uno slot libero
			attacks[i] = new Attack(s);	//Aggiunge l'attacco all'inventario
			return true;
		}
	}
	return false;	//Nessuno slot libero
}
Attack* Entity::get_attack(int id) {	//Riceve la posizione dell'attacco nell'inventario
	if(id<MAX_ATTACKS) {		//Controlla se l'ID è in range
		return attacks[id];		//Restituisce il puntatore all'attacco
	} else {
		return NULL;
	}
}
bool Entity::unequip_attack(int id) {	//Riceve la posizione dell'attacco nell'inventario
	if(id<MAX_ATTACKS) {		//Controlla se l'ID è in range
		if(attacks[id]!=NULL) {
			delete(attacks[id]);	//Dealloca l'attacco
			attacks[id]=NULL;	//In questo modo equip_attack() segnerà lo slot libero
			return true;
		}
	}
	return false;	//L'attacco che si sta cercando di disequipaggiare non è presente
}
bool Entity::equip_item(char* s) {	//Riceve una stringa serializzata di un consumabile
	int i;
	for(i=0;i<MAX_ITEMS;i++) {
		if(items[i]==NULL) {	//Controlla se c'è uno slot libero
			items[i] = new Item(s);	//Aggiunge l'oggetto all'inventario
			return true;
		}
	}
	return false;	//Nessuno slot libero
}
Item* Entity::get_item(int id) {	//Riceve la posizione dell'oggetto nell'inventario
	if(id<MAX_ITEMS) {		//Controlla se l'ID è in range
		return items[id];	//Restituisce il puntatore all'oggetto
	} else {
		return NULL;
	}
}
bool Entity::unequip_item(int id) {	//Riceve la posizione dell'oggetto nell'inventario
	if(id<MAX_ITEMS) {		//Controlla se l'ID è in range
		if(items[id]!=NULL) {
			delete(items[id]);	//Dealloca l'oggetto
			items[id]=NULL;		//In questo modo equip_item() segnerà lo slot libero
			return true;
		}
	}
	return false;	//L'oggetto che si sta cercando di disequipaggiare non è presente
}
void Entity::refull_items() {
	for(int i=0;i<MAX_ITEMS;i++) {
		if(items[i]!=NULL) {
			items[i]->refull_uses();	//Per ogni oggetto nell'inventario, ricarica gli usi
		}
	}
}
bool Entity::hit(int value) {	//Riceve quanti punti vita togliere all'entità
	if(value<=0) return false;	//Controllo: non si possono togliere punti vita negativi
	current_hp = current_hp - value;	//Sottrae i punti vita da quelli attuali
	if(current_hp<=DEATH_VALUE) {	//Se i punti vita pareggiano o scendono sotto il valore di morte
		alive = false;	//l'entità non è più viva
	}
	return true;
}
bool Entity::heal(int value) {	//Riceve quanti punti vita aggiungere all'entità
	if(value<=0) return false;	//Non si possono aggiungere punti vita negativi
	current_hp = current_hp + value;	//Aggiunge i punti vita a quelli attuali
	if(current_hp>stats[HP]) {	//Se il numero attuale di punti vita supera il massimo
		current_hp = stats[HP];	//imposta il valore attuale al massimo
	}
	return true;
}
int Entity::attack(Entity* target) {	//Riceve il bersaglio dell'attacco
	int attack_roll;	//Qui verrà salvato il tiro per colpire
	int damage_roll;	//Qui verranno salvati i danni
	Attack* attack = NULL;
	for(int i=0;i<MAX_ATTACKS;i++) {	//Controlla se è presente un attacco nell'inventario
		if(attacks[i]!=NULL) {
			attack = new Attack(attacks[i]->toString());
		}
	}
	if(attack == NULL) {	//Se non ha trovato alcun tipo di attacco, assegna quello di default
		attack = new Attack(MELEE);
	}
	int scale;	//Qui verrà salvato il valore da aggiungere per lo scaling
	if(attack->get_scaling()!=0) {	//Se l'arma scala in qualche caratteristica
		scale = get_modifier(stats[attack->get_scaling()]);	//Assegna il modificatore della caratteristica
	} else {
		scale = 0;	//altrimenti il valore è 0
	}
	attack->make_attack(&attack_roll,&damage_roll,stats[BAB],scale);	//Genera i valori di attacco
	int critical = attack_roll - stats[BAB] - scale;	//Controllo per il colpo critico
	if(attack_roll > target->get_ac() || critical == 20) {	//Se il tiro per colpire supera la classe
															// armatura nemica o il colpo è critico
		if(damage_roll<1)	//Se il danno è minore di 1
			damage_roll = 1;	// imposta il danno a 1 (non è possibile infliggere meno di 1 danno)
		target->hit(damage_roll);	//Abbassa i punti vita del bersaglio in base al danno
		return damage_roll;	//Restituisce il danno inflitto
	}
	return 0;	//Restituisce 0 se il bersaglio non è stato colpito
}
bool Entity::heal_with_item() {
	for(int i=0;i<MAX_ITEMS;i++) {
		if(items[i]!=NULL && items[i]->is_heal() && items[i]->use()) {	//Se trova un oggetto di cura con
																		// ancora qualche utilizzo
			heal(throw_dice(items[i]->get_value()));	//Cura l'entità in base al valore dell'oggetto
			return true;
		}
	}
	return false;	//Non sono stati trovati oggetti di cura disponibili
}
char* Entity::toString() {
	stats[FORT] = stats[FORT] - get_modifier(stats[CON]);	//Ritorna ai valori base dei tiri salvezza
	stats[REFL] = stats[REFL] - get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] - get_modifier(stats[WIS]);
	char ret[MAX_BUFFER];
	sprintf(ret,"%s",name);
	for(int i=0;i<N_STATS;i++) {
		sprintf(ret,"%s,%d",ret,stats[i]);
	}
	stats[FORT] = stats[FORT] + get_modifier(stats[CON]);	//Reimposta i corretti valori dei tiri salvezza
	stats[REFL] = stats[REFL] + get_modifier(stats[DEX]);
	stats[WILL] = stats[WILL] + get_modifier(stats[WIS]);
	return strdup(ret);		//FREE
}
// Tutti i seguenti metodi restituiscono valori negativi perché il loro scopo è nell'ereditarietà
bool Entity::equip_spell(char*) {
	return false;
}
Spell* Entity::get_spell(int) {
	return NULL;
}
bool Entity::unequip_spell(int)  {
	return false;
}
bool Entity::can_cast() {
	return false;
}
int Entity::cast(Entity*,bool) {
	return 0;
}
bool Entity::heal_with_spell() {
	return false;
}
bool Entity::rage() {
	return false;
}
bool Entity::end_rage() {
	return false;
}
bool Entity::wild_shape(char*) {
	return false;
}
bool Entity::is_animal() {
	return false;
}
int Entity::play() {
	return 0;
}
int Entity::act() {
	return NOTHING;
}

//----------------------------------------------------------------------------------------------------------------------------------
/*
Caster
Questa classe figlia di Entity rappresenta le entità in grado di lanciare incantesimi.
*/
class Caster : public Entity{
protected:Spell* spells[MAX_SPELLS];	//Contiene gli incantesimi conosciuti dall'incantatore
	int spell_uses[SPELL_MAX_LEVEL];	//Segna quanti incantesimi e di che tipo l'incantatore può lanciare
	int spell_type;	//Tipo di incantesimi conosciuti dall'incantatore
	void load_spellslots(int,char*);	//Metodo privato, carica gli spellslots dell'incantatore
public:	Caster(char*);		//Costruttore
	~Caster();	//Distruttore
	bool equip_spell(char*);	//Equipaggia un incantesimo ricevendone la stringa serializzata
	Spell* get_spell(int);	//Restituisce il puntatore ad un incantesimo
	bool unequip_spell(int);	//Cancella un incantesimo dall'elenco di quelli conosciuti
	bool can_cast();	//Controlla se l'incantatore è ancora in grado di castare
	/* Metodi per le azioni */
	int cast(Entity*,int);	//Usa un incantesimo su un bersaglio
	bool heal_with_spell();	//Prova a curare l'entità con un incantesimo di cura
	char* toString();	//Serializator
};
Caster::Caster(char* s) : Entity(s) {
	for(int i=0;i<SPELL_MAX_LEVEL;i++) {	//Imposta tutti gli spellslots a 0
		spell_uses[i] = 0;
	}
	for(int i=0;i<MAX_SPELLS;i++) {	//Imposta tutti gli incantesimi a NULL
		spells[i]=NULL;
	}
}
void Caster::load_spellslots(int class_spell_max_level, char* filename) {	//Riceve il livello massimo
												// degli incantesimi e il file su cui è presente la tabella
	int** table;	//Contenitore per il risultato della split()
	table = get_table(filename,MAX_LEVEL,class_spell_max_level);	//Carica una tabella da file
	for(int i=0;i<class_spell_max_level;i++) {
		spell_uses[i] = table[stats[LEVEL]][i];	//Copia gli spellslots corrispondenti al livello dell'entità
	}
}
Caster::~Caster() {
	for(int i=0;i<MAX_SPELLS;i++) {	//Dealloca tutti gli incantesimi conosciuti
		if(spells[i]!=NULL) {
			delete(spells[i]);
		}
	}
}
bool Caster::equip_spell(char* s) {	//Riceve una stringa serializzata di un incantesimo
	int i;
	for(i=0;i<MAX_SPELLS;i++) {
		if(spells[i]==NULL) {	//se è presente uno slot libero
			spells[i] = new Spell(s);	// aggiunge l'incantesimo alla lista di quelli conosciuti
			return true;
		}
	}
	return false;	//Nessuno slot libero
}
Spell* Caster::get_spell(int id) {	//Riceve la posizione dell'incantesimo nell'inventario
	if(id<MAX_SPELLS) {		//Controlla se l'ID è in range
		return spells[id];	//restituisce il puntatore all'incantesimo
	} else {
		return NULL;
	}
}
bool Caster::unequip_spell(int id) {
	if(id<MAX_SPELLS) {		//Controlla se l'ID è in range
		if(spells[id]!=NULL) {
			delete(spells[id]);	//Dealloca l'incantesimo
			spells[id]=NULL;	//In questo modo equip_spell() segnerà lo slot libero
			return true;
		} else {
			return false;	//L'incantesimo che si sta cercando di togliere non è presente
		}
	}
}
bool Caster::can_cast() {
	for(int i=0;i<SPELL_MAX_LEVEL;i++) {
		if(spell_uses[i]>0) return true;	//Controlla se ci sono ancora spellslot disponibili
	}
	return false;
}
int Caster::cast(Entity* target, int heal) {	//Riceve un bersaglio e un flag che indica se
									// lo si vuole attaccare o curare
	Spell* spell = NULL;
	bool found = false;	//Questa permette di uscire dalla ricerca incantesimi quando se ne trova uno
	for(int i=0;i<MAX_SPELLS && !found;i++) {	//Cerca un incantesimo da poter usare
		if(spells[i]!=NULL && heal==spells[i]->is_heal() && spell_uses[spells[i]->get_level()]>0) {
			//l'incantesimo è disponibile, corrisponde al tipo e ha ancora utilizzi
			spell = new Spell(spells[i]->toString());
			found = true;	//Per uscire dal for()
		}
	}
	if(!found) {	//Se non sono stati trovati incantesimi
		return 0;
	}
	spell_uses[spell->get_level()]--;	//Sottrae 1 agli spellslots per quel livello di incantesimo
	if(spell->is_heal()) {	//Se la spell è di cura
		char* s = spell->get_value();	//Recupera il valore
		int value = throw_dice(s);	//Lancia il dado corrispondente
		target->heal(value);	//Cura il bersaglio
		free(s);
		return 0;
	}
	int cd = spell->get_dc(stats[LEVEL]);	//Recupera la classe difficoltà dell'incantesimo
	int save = throw_dice(ROLL_DESC) + target->get_stat(spell->get_save_type());	//Genera il tiro
															// salvezza per il bersaglio
	if(save <= cd) {	//Se il tiro salvezza non supera la classe difficoltà
		int ret = throw_dice(spell->get_value());	//Genera il danno dell'incantesimo
		target->hit(ret);	//Colpisce il nemico
		return ret;	//restituisce il danno inflitto
	}
	return 0;	//L'incantesimo non ha colpito
}
bool Caster::heal_with_spell() {
	if(this->can_cast()) {	//Se l'incantatore può ancora lanciare incantesimi
		cast(this,1);	// lancia su sé stesso un incantesimo di cura
		return true;
	}
	return false;	//l'incantatore non ha più incantesimi disponibili
}
char* Caster::toString() {	//Metodo attualmente inutile, gli incantatori non hanno attributi da 
							// dover memorizzare
	char* ret;
	ret = Entity::toString();
	return ret;
}
//----------------------------------------------------------------------------------------------------------------------------------
class Barbarian : public Entity{
private:int rage_time;		//Numero di turni in cui permane l'ira del barbaro
		bool is_raging;		//Controlla se il barbaro è andato in ira
public:	Barbarian(char*);	//Costruttore
	~Barbarian();	//Distruttore
	bool rage();		//Gestisce, attiva e disattiva l'ira del barbaro
	int act();		//Restituisce l'azione intrapresa dal barbaro
	char* toString();	//Serializator
};
Barbarian::Barbarian(char* s) : Entity(s) {
	rage_time = 3 + get_modifier(stats[CON]);	//Imposta i turni dell'ira
	is_raging = false;	//Il barbaro non è in ira
}
Barbarian::~Barbarian() {
}
bool Barbarian::rage() {	//Restituisce true se il barbaro è in ira, false se non lo è
	if(rage_time==0 && !is_raging) {
		return false;	//Il barbaro non può più andare in ira
	}
	if(!is_raging) {	//Il barbaro non è in ira ma può andarci
		is_raging = true;	//Manda il barbaro in ira e modifica le sue statistiche
							// in base al livello
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
	if(rage_time==0 && is_raging) {	//Se il barbaro è in ira ma il numero di turni è finito
		is_raging = false;	//Il barbaro esce dall'ira e le sue caratteristiche
							// vengono resettate
		if(stats[LEVEL]<11) {
			current_hp = current_hp - (2*stats[LEVEL]);
			stats[STR] -= 4;
			stats[CON] -= 4;
			stats[WILL] -= 2;
			return false;
		}
		if(stats[LEVEL]<20) {
			current_hp = current_hp - (2*stats[LEVEL]);
			stats[STR] -= 6;
			stats[CON] -= 6;
			stats[WILL] -= 3;
			return false;
		}
		current_hp = current_hp - (2*stats[LEVEL]);
		stats[STR] -= 8;
		stats[CON] -= 8;
		stats[WILL] -= 4;
		return false;
	}
	rage_time--;	//Se il barbaro è in ira e ha ancora dei turni, li diminuisce id 1
	return true;	//Il barbaro è in ira
}
int Barbarian::act() {	//Sceglie l'azione da far intraprendere al barbaro
	int threshold;	//Soglia sopra o sotto la quale cambia il comportamento del barbaro
	if(!is_alive()) {
		return NOTHING;	//Se il barbaro è morto, non può agire
	}
	threshold = stats[HP]/2;	//Imposta la soglia
	if(current_hp>=threshold) {	//Sopra la soglia
		return ATTACK;	// il barbaro attacca
	} else {	//sotto la soglia
		rage();	//Il barbaro entra in ira se può
		if(heal_with_item()) {	//Tenta di curarsi con un oggetto
			return HEAL;	//Se ci riesce, non deve più agire
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
private:int entertain;		//Contiene il valore di intrattenimento del bardo
public:	Bard(char*);		//Receives serialized bard string
	~Bard();
	int play();	//Il bardo suona e genera il risultato della prova di intrattenere
	int act();	//Decide l'azione del bardo
	char* toString();	//Serializator
};
Bard::Bard(char* very_bardic_string) : Caster(very_bardic_string) {
	spell_type = S_BARD;
	load_spellslots(BARD_SPELL_MAX_LEVEL,BARD_SPELLSLOTS);	//Carica gli spellslots del bardo
	entertain = stats[LEVEL] + get_modifier(stats[CHA]) + 3;//Imposta il valore di intrattenimento
}
Bard::~Bard() {
}
int Bard::play() {	//Suona della musica per tentare di bloccare i mostri
	int ret;
	ret = throw_dice(ROLL_DESC) + get_modifier(stats[CHA]) + entertain;	//Genera la potenza della musica
	if(ret<1) {	//Se il valore è minore di 1, lo imposta a 1
		ret = 1;
	}
	return ret;
}
int Bard::act() {	//Decide l'azione del bardo
	int thrashold;	//Soglia sopra o sotto la quale cambia il comportamento del bardo
	if(!is_alive()) {
		return NOTHING;
	}
	thrashold = stats[HP]/2;	//Imposta la soglia
	if(current_hp>thrashold) {	//Sopra la soglia
		if((rand()%2)!=0) {	// decide randomicamente se attaccare o suonare
			return ATTACK;
		} else {
			return MUSIC;
		}
	} else {	//sotto la soglia
		if(heal_with_spell() || heal_with_item()) {	//tenta di curarsi con oggetti o incantesimi
			return HEAL;
		}
		return ATTACK;
	}
}
char* Bard::toString() {
	return Caster::toString();
}
//----------------------------------------------------------------------------------------------------------------------------------
class Cleric : public Caster{
public:	Cleric(char*);
	~Cleric();
	int act();	//Decide l'azione del chierico
	char* toString();
};
Cleric::Cleric(char* s) : Caster(s) {
	spell_type = S_CLERIC;
	load_spellslots(CLERIC_SPELL_MAX_LEVEL,CLERIC_SPELLSLOTS);	//Carica gli spellslots del bardo
}
Cleric::~Cleric() {
}
int Cleric::act() {	//Decide l'azione del chierico
	int threshold;	//Soglia sopra o sotto la quale cambia il comportamento del chierico
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;	//Imposta la soglia
	if(current_hp>threshold) {	//Sopra la soglia
		return ATTACK;	// il chierico attacca
	} else {	//sotto la soglia
		if(heal_with_spell() || heal_with_item()) {	// tenta di curarsi con oggetti o incantesimi
			return HEAL;
		}
		return ATTACK;	//altrimenti attacca
	}
}
char* Cleric::toString() {
	return Caster::toString();	
}
//----------------------------------------------------------------------------------------------------------------------------------
class Animal : public Entity{	//Questa classe serve per la forma animale del druido
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
private:int wild_shape_time;		//Numero di turni di durata della forma selvatica
		Animal* shape;		//Contiene l'animale in cui il druido si trasforma in forma selvatica
public:	Druid(char*);
	~Druid();
	bool wild_shape(char*);		//Trasforma il druido in animale
	bool is_animal();	//Controlla se il druido è in forma animale
	int attack(Entity*);	//Chiama il metodo attack() dell'animale se è in forma selvatica, altrimenti il proprio
	int act();	//Decide l'azione del druido
	char* toString();
};
Druid::Druid(char* s) : Caster(s) {
	spell_type = S_DRUID;
	load_spellslots(DRUID_SPELL_MAX_LEVEL,DRUID_SPELLSLOTS);	//Carica gli spellslots del druido
	shape = NULL;
	wild_shape_time = 10*stats[LEVEL];	//Imposta la durata della forma selvatica
}
Druid::~Druid() {
	if(shape!=NULL) {
		delete(shape);	//Dealloca la forma selvatica
	}
}
bool Druid::wild_shape(char* s) {
	if(wild_shape_time == 0 || stats[LEVEL]<5) {	//Il druido non può trasformarsi
		shape = NULL;
		return false;
	}
	if(shape == NULL) {	//Il druido non si è ancora trasformato
		shape = new Animal(s);
		return true;
	}
	wild_shape_time--;	//Il druido è già trasformato
	return true;
}
bool Druid::is_animal() {
	return (shape!=NULL);
}
int Druid::attack(Entity* target) {
	if(is_animal()) {
		return shape->attack(target);
	}
	return Entity::attack(target);
}
int Druid::act() {
	int threshold;	//Soglia sopra o sotto la quale cambia il comportamento del druido
	if(!is_alive()) {
		return NOTHING;
	}
	threshold = stats[HP]/2;
	if(current_hp>threshold) {
		if(shape!=NULL) {
			this->wild_shape("");
		} else {
			char* animal = "Orso,11,1,10,16,10,14,8,10,6,0,0,0,2";
			this->wild_shape(animal);
			return NOTHING;
		}
		return ATTACK;
	} else {
		if(heal_with_spell()) {
			return HEAL;
		}
		if(current_hp<5 && shape!=NULL) {
			wild_shape_time = 0;
		}
		if(shape==NULL) {
			if(heal_with_item()) {
				return HEAL;
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
			return HEAL;
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
			return HEAL;
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
			return HEAL;
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
			return HEAL;
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
			return HEAL;
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
	bool attack(Entity*);
	char* toString();
};
Rogue::Rogue(char* s) : Entity(s) {
	hidden = false;
}
Rogue::~Rogue() {
}
int Rogue::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	int threshold = stats[HP]/2;
	if(current_hp<threshold) {
		if(heal_with_item()) {
			return HEAL;
		}
	}
	if(!hidden) {
		if(rand()%3==2) {
			hidden = true;
			stats[DEX] = stats[DEX] + 1000;
			return HIDDEN;
		}
		return NOTHING;
	}
	return ATTACK;
}
bool Rogue::attack(Entity* target) {
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
	int scale;
	if(attack->get_scaling()!=0) {
		scale = get_modifier(stats[attack->get_scaling()]);
	} else {
		scale = 0;
	}
	attack->make_attack(&attack_roll,&damage_roll,stats[BAB],scale);
	if(hidden) {
		hidden = false;
		int bonus_dices = (stats[LEVEL]-1)/2+1;
		for(int k=0;k<bonus_dices;k++) {
			damage_roll = damage_roll + throw_dice(FURTIVE_DESC);
		}
	}
	int critical = attack_roll - stats[BAB] - scale;
	if(attack_roll > target->get_ac() || critical == 20) {
		if(damage_roll<1)
			damage_roll = 1;
		target->hit(damage_roll);
		return damage_roll;
	}
	return 0;
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
			return HEAL;
		}
		if(heal_with_spell() || heal_with_item()) {
			return HEAL;
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
		int cd;
public:	Monster(char*);
	~Monster();
	int get_ac();
	int get_natural_ac();
	int get_cd();
	int act();
	char* toString();
};
Monster::Monster(char* s) : Entity(s) {
	char** temp;
	int count = split(s,',',&temp);
	natural_ac = atoi(temp[NATURAL_AC+1]);
	cd = atoi(temp[CD+1]);
}
Monster::~Monster() {
}
int Monster::get_ac() {
	int ret;
	ret = BASE_AC;
	for(int i=0;i<MAX_ARMOR;i++) {	//Deletes all armor objects
		if(armors[i]!=NULL) {
			ret = ret + armors[i]->get_ac_bonus();
		}
	}
	ret = ret + get_modifier(stats[DEX]) + natural_ac;
	return ret;
}
int Monster::get_natural_ac() {
	return natural_ac;
}
int Monster::get_cd() {
	return cd;
}
int Monster::act() {
	if(!is_alive()) {
		return NOTHING;
	}
	return ATTACK;
}
char* Monster::toString() {
	char* ret;
	char* s = Entity::toString();
	sprintf(ret,"%s,%d,%d",Entity::toString(),natural_ac,cd);
	return ret;
}

