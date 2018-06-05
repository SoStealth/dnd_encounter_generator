#include "entities.hpp"

int simulate(Entity* *characters, int n_characters, Entity* *monsters, int n_monsters) {
	int balance_value;
	bool victory = false;
	bool defeat = false;
	
	while(!victory && !defeat) {
		int c_counter = 0;
		int m_counter = 0;
		Entity* actor = NULL;
		
		//Repeats until every character and every monster has acted -----
		while(c_counter<n_characters || m_counter<n_monsters) {
			
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
			switch(action) {
				case ATTACK:
					
					break;
				case NOTHING:
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
	}	
	return balance_value;
}
