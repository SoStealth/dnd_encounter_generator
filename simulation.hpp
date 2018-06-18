#ifndef __ENTITIES
#define __ENTITIES
#include "entities.hpp"
#endif

#define LOG "log.txt"

//-------------------------------------------------------------------------------------------
int init_balance_value(Entity** a, int n) {
	int value = 0;
	for(int v=0;v<n;v++) {
		value = value + a[v]->get_stat(HP); 
	}
	return value;
}
//-------------------------------------------------------------------------------------------
int fight(Entity* battler, Entity* *enemies, int n_enemies) {
	int target = rand()%n_enemies;
	char entry[MAX_BUFFER];
	bool found = false;
	for(int i=0;i<n_enemies && !found;i++) {
		target++;
		target = target%n_enemies;
		if(enemies[target]->is_alive()) {
			found = true;
		}
	}
	if(found) {
		int ret = battler->attack(enemies[target]);
		sprintf(entry,"%s attacca %s infliggendo %d danni\n",battler->get_name(),enemies[target]->get_name(),ret);
		write_log(LOG,entry);
		return ret;
	} else {
		sprintf(entry,"Tutti gli avversari di %s sono morti\n",battler->get_name());
		write_log(LOG,entry);
	}
	return 0;
}
//-------------------------------------------------------------------------------------------
int magic_fight(Entity* battler, Entity* *enemies, int n_enemies) {
	int target = rand()%n_enemies;
	char entry[MAX_BUFFER];
	bool found = false;
	for(int i=0;i<n_enemies && !found;i++) {
		target++;
		target = target%n_enemies;
		if(enemies[target]->is_alive()) {
			found = true;
		}
	}
	if(found) {
		int ret = dynamic_cast<Caster*>(battler)->cast(enemies[target],0);
		sprintf(entry,"%s lancia una magia contro %s infliggendo %d danni\n",battler->get_name(),enemies[target]->get_name(),ret);
		write_log(LOG,entry);
		return ret;
	} else {
		sprintf(entry,"Tutti gli avversari di %s sono morti\n",battler->get_name());
		write_log(LOG,entry);
	}
	return 0; 
}
//-------------------------------------------------------------------------------------------
char* simulate(Entity* *characters, int n_characters, Entity* *monsters, int n_monsters) {
	int c_start_value = 0;
	int m_start_value = 0;
	int c_actual_value = 0;
	int m_actual_value = 0;
	int turns = 0;
	FILE* file = fopen(LOG,"w");
	fclose(file);
	char entry[MAX_BUFFER];

	bool victory = false;
	bool defeat = false;

	for(int i=0;i<n_characters;i++) {
		characters[i]->reset();
	}
	for(int i=0;i<n_monsters;i++) {
		monsters[i]->reset();
	}

	c_start_value = init_balance_value(characters,n_characters);
	m_start_value = init_balance_value(monsters,n_monsters);
	
	while(!victory && !defeat) { 	//THIS IS A FULL ROUND
		int c_counter = 0;
		int m_counter = 0;
		int music = 0;
		Entity* actor = NULL;
		
		//Repeats until every character and every monster has acted -----
		while(c_counter<n_characters || m_counter<n_monsters) {		//THIS IS A TURN
			
			//Randomly selects which entities go first --------------
			bool enemy;
			while(actor==NULL) {
				if(rand()%2==1) {
					if(c_counter < n_characters) {
						actor = characters[c_counter];
						c_counter++;
						enemy = false;
					}
				} else {
					if(m_counter < n_monsters) {
						actor = monsters[m_counter];
						m_counter++;
						enemy = true;
					}
				}
			}
			//--------------------------------------------------------
			
			sprintf(entry,"E' il turno di %s\n",actor->get_name());
			write_log(LOG,entry);

			int action;
			switch(actor->get_stat(CLASS)) {
				case BARBARIAN:
					action = dynamic_cast<Barbarian*>(actor)->act();
					break;
				case BARD:
					action = dynamic_cast<Bard*>(actor)->act();
					break;
				case CLERIC:
					action = dynamic_cast<Cleric*>(actor)->act();
					break;
				case DRUID:
					action = dynamic_cast<Druid*>(actor)->act();
					break;
				case SORCERER:
					action = dynamic_cast<Sorcerer*>(actor)->act();
					break;
				case WIZARD:
					action = dynamic_cast<Wizard*>(actor)->act();
					break;
				case MONK:
					action = dynamic_cast<Monk*>(actor)->act();
					break;
				case FIGHTER:
					action = dynamic_cast<Fighter*>(actor)->act();
					break;
				case RANGER:
					action = dynamic_cast<Ranger*>(actor)->act();
					break;
				case ROGUE:
					action = dynamic_cast<Rogue*>(actor)->act();
					break;
				case PALADIN:
					action = dynamic_cast<Paladin*>(actor)->act();
					break;
				case ANIMAL:
					action = dynamic_cast<Animal*>(actor)->act();
					break;
				case MONSTER:
					action = dynamic_cast<Monster*>(actor)->act();
					break;
			}
			if( (enemy&&music>0) || (!enemy&&music<0) ) {
				int threshold = music;
				if(threshold<1)
					threshold = threshold * -1;
				int value = throw_dice(ROLL_DESC) + actor->get_stat(WILL);
				if(value<threshold) {
					sprintf(entry,"%s non può agire a causa della musica\n",actor->get_name());
					write_log(LOG,entry);
					action = NOTHING;
				}
			}
			switch(action) {
				case ATTACK:
					if(enemy) {
						c_actual_value += fight(actor,characters,n_characters);
					} else {
						m_actual_value += fight(actor,monsters,n_monsters);
					}
					break;
				case CAST:
					if(enemy) {
						c_actual_value += magic_fight(actor,characters,n_characters);
					} else {
						m_actual_value += magic_fight(actor,monsters,n_monsters);
					}
					break;
				case NOTHING:
					sprintf(entry,"%s non agisce\n",actor->get_name());
					write_log(LOG,entry);
					break;
				case MUSIC:
					music = dynamic_cast<Bard*>(actor)->play();
					if(enemy)
						music = music * -1;
					sprintf(entry,"%s inizia a suonare (potenza della musica: %d)\n",actor->get_name(),music);
					write_log(LOG,entry);
					break;
			}
			actor = NULL;
		}
		//-----------------------------------------------------------------
		//Victory or defeat check --------------------
		defeat = true;
		victory = true;
		for(int i=0;i<n_characters;i++) {
			if(characters[i]->is_alive()) {
				defeat = false;
			} else {
				sprintf(entry,"%s è morto\n",characters[i]->get_name());
				write_log(LOG,entry);
			}
		}
		for(int i=0;i<n_monsters;i++) {
			if(monsters[i]->is_alive()) {
				victory = false;
			} else {
				sprintf(entry,"%s è morto\n",monsters[i]->get_name());
				write_log(LOG,entry);
			}
		}
		//---------------------------------------------
		turns++;
		sprintf(entry,"Fine del turno %d\n\n",turns);
		write_log(LOG,entry);
	}

	if(victory) {
		sprintf(entry,"Il gruppo di eroi vince!\n");
		write_log(LOG,entry);
	} else {
		sprintf(entry,"Il gruppo di mostri vince!\n");
		write_log(LOG,entry);
	}

	char balance_values[MAX_BUFFER];
	int c_value = (3*c_actual_value*100)/c_start_value;
	int m_value = (m_actual_value*100)/m_start_value;
	int casualties = 0;
	for(int v=0;v<n_characters;v++) {
		if(!characters[v]->is_alive()) {
			casualties++;
		} 
	}
	
	sprintf(balance_values,"%d,%d,%d,%d",c_value,m_value,turns,casualties);
	//printf("C_VALUE = %d\nM_VALUE = %d\n",c_value,m_value);

	return strdup(balance_values);
}
