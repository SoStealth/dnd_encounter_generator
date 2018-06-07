#include "entities.hpp"
//-------------------------------------------------------------------------------------------
int init_balance_value(Entity** a1, int n1) {
	int value = 0;
	for(int v=0;v<n1;v++) {
		value = value + a1[v]->get_current_hp(); 
	}
	return value;
}
//-------------------------------------------------------------------------------------------
int fight(Entity* battler, Entity* *enemies, int n_enemies) {
	int target = rand()%n_enemies;
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
		return ret;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------
int magic_fight(Entity* battler, Entity* *enemies, int n_enemies) {
	int target = rand()%n_enemies;
	bool found = false;
	for(int i=0;i<n_enemies && !found;i++) {
		target++;
		target = target%n_enemies;
		if(enemies[target]->is_alive()) {
			found = true;
		}
	}
	if(found) {
		int ret = battler->cast(enemies[target],false);
		return ret;
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

	bool victory = false;
	bool defeat = false;

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
			
			int action = actor->act();
			if( (enemy&&music>0) || (!enemy&&music<0) ) {
				int threshold = music;
				if(threshold<1)
					threshold = threshold * -1;
				int value = throw_dice(D20) + actor->get_stat(WILL);
				if(value<threshold) {
					action = NOTHING;
				}
			}
			switch(action) {
				case ATTACK:
					if(enemy) {
						m_actual_value += fight(actor,characters,n_characters);
					} else {
						c_actual_value += fight(actor,monsters,n_monsters);
					}
					break;
				case CAST:
					if(enemy) {
						m_actual_value += magic_fight();
					} else {
						c_actual_value += magic_fight();
					}
					break;
				case NOTHING:
					break;
				case MUSIC:
					music = actor->play();
					if(enemy)
						music = music * -1;
					break;
			}
		}
		//-----------------------------------------------------------------
		
		//Victory or defeat check --------------------
		for(int i=0;i<n_characters;i++) {
			defeat = true;
			if(characters[i]->is_alive()) {
				defeat = false;
			}
		}
		for(int i=0;i<n_monsters;i++) {
			victory = true;
			if(monsters[i]->is_alive()) {
				victory = false;
			}
		}
		//---------------------------------------------
		
		turns++;
	}

	char balance_values[MAX_BUFFER];
	int c_value = (3*c_actual_value*100)/c_start_value;
	int m_value = (m_actual_value*100)/m_start_value;
	
	sprintf(balance_values,"%d,%d,%d",c_value,m_value,turns);

	return strdup(balance_value);
}
