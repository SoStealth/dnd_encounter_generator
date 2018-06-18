#include "balancer.hpp"

#define NUMBER 1000
#define N_CHARACTERS 4

int main(int argc, char* argv[]) {
	Entity* characters[1000];
	Entity* enemies[1000];
	int n_monsters = 2;

	init_random();
	
	char balance_values[MAX_BUFFER];
	int c_value = 0;
	int m_value = 0;
	int turns = 0;
	int casualties = 0;

	Bard* pg0 = new Bard("Lino Mendol,1,1,8,8,12,10,14,14,16,0,0,0,2");
	Fighter* pg1 = new Fighter("Sir Iano,7,1,12,16,14,14,6,10,8,1,2,2,0");
	Wizard* pg2 = new Wizard("Enrico Vasaio,5,1,6,6,14,8,16,14,10,0,0,0,2");
	Cleric* pg3 = new Cleric("Padre Pio,2,1,11,8,12,10,14,14,16,1,2,0,2");

	pg0->equip_item("Pozione annacquata di Levin,1d8,1");
	pg1->equip_item("Pozione annacquata di Levin,1d8,1");

	pg0->equip_attack("Balestra,1d8,2,1,0");
	pg1->equip_attack("Spadone,2d6,3,1,2");
	pg2->equip_attack("Pugnale,1d4,2,2,2");
	pg3->equip_attack("Mazza pesante,1d8,2,1,2");

	pg0->equip_armor("Armatura di cuoio,1,5,0");
	pg1->equip_armor("Armatura di cuoio borchiato,3,3,20");
	pg3->equip_armor("Armatura a scaglie,4,1,30");

	pg2->equip_spell("Dardo magico,1,1d4+1,11,0");
	pg2->equip_spell("Raggio di gelo,0,1d3,10,0");
	pg3->equip_spell("Cura ferite,1,1d8+1,11,1");
	pg3->equip_spell("Cura ferite minori,0,1,11,1");

	Monster* en0 = new Monster("Skum,12,1,11,19,13,13,10,10,6,1,1,1,3,2,2");
	Monster* en1 = new Monster("Skum,12,1,11,19,13,13,10,10,6,1,1,1,3,2,2");

	en0->equip_attack("Morso,2d6,0,0,3");
	en1->equip_attack("Morso,2d6,0,0,3");

	characters[0] = pg0;
	characters[1] = pg1;
	characters[2] = pg2;
	characters[3] = pg3;
	
	enemies[0] = en0;
	enemies[1] = en1;

	do{
		for(int i=0;i<NUMBER;i++) {
			for(int j=0;j<N_CHARACTERS;j++) {
				characters[j]->refull_hp();
				characters[j]->refull_items();
			}
			for(int j=0;j<n_monsters;j++) {
				enemies[j]->refull_hp();
				enemies[j]->refull_items();
			}
			char* value;
			value = simulate(characters,N_CHARACTERS,enemies,n_monsters);
			char** temp;
			split(value,',',&temp);
			c_value = c_value + atoi(temp[0]);
			m_value = m_value + atoi(temp[1]);
			turns = turns + atoi(temp[2]);
			casualties = casualties + atoi(temp[3]);
		}

		c_value = c_value/NUMBER;
		m_value = m_value/NUMBER;
		turns = turns/NUMBER;
		casualties = casualties/NUMBER;
		printf("C_VALUE MEDIO = %d\nM_VALUE MEDIO = %d\nTURNI MEDI = %d\nMORTI MEDIE = %d\n",c_value,m_value,turns,casualties);
		sprintf(balance_values,"%d,%d,%d,%d",c_value,m_value,turns,casualties);

	} while(!balance(balance_values,enemies,&n_monsters));

	for(int i=0;i<n_monsters;i++) {
		printf("%s\n",enemies[i]->toString());	
	}

	return 0;
}