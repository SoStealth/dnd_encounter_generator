#include "simulation.hpp"

#define NUMBER 1000
#define N_CHARACTERS 4
#define N_MONSTERS 2

int main(int argc, char* argv[]) {
	Entity* characters[N_CHARACTERS];
	Entity* enemies[N_MONSTERS];

	init_random();
	
	int c_value = 0;
	int m_value = 0;
	int turns = 0;
	int casualties = 0;

	for(int i=0;i<NUMBER;i++) {
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

		/* GRUPPO ESAGERATO, MEDIA ~1060,0,2
		enemies[0] = new Monster("Dragone d'oro,12,25,60,22,20,28,25,25,20,20,10,10,12,20");
		enemies[1] = new Monster("Dragone d'oro,12,25,60,22,20,28,25,25,20,20,10,10,12,20");
		enemies[2] = new Monster("Dragone d'oro,12,25,60,22,20,28,25,25,20,20,10,10,12,20");
		*/
		
		/* GRUPPO EASY, MEDIA ~120, 150
		Monster* en0 = new Monster("Scheletro umano,12,1,6,13,13,0,0,10,1,0,0,1,2,2");
		Monster* en1 = new Monster("Scheletro umano,12,1,6,13,13,0,0,10,1,0,0,1,2,2");
		Monster* en2 = new Monster("Scheletro umano,12,1,6,13,13,0,0,10,1,0,0,1,2,2");

		en0->equip_attack("Scimitarra,1d6+1,2,2,3");
		en1->equip_attack("Scimitarra,1d6+1,2,2,3");
		en2->equip_attack("Scimitarra,1d6+1,2,2,3");
		*/

		/*Gruppo medio, MEDIA ~180, 130
		Monster* en0 = new Monster("Ghoul,12,1,13,13,15,0,13,14,12,1,0,2,5,2");
		Monster* en1 = new Monster("Ghoul,12,1,13,13,15,0,13,14,12,1,0,2,5,2");
		Monster* en2 = new Monster("Ghoul,12,1,13,13,15,0,13,14,12,1,0,2,5,2");

		en0->equip_attack("Morso,1d6+1,0,0,0");
		en1->equip_attack("Morso,1d6+1,0,0,0");
		en2->equip_attack("Morso,1d6+1,0,0,0");
		*/

		//Gruppo forte, MEDIA ~300, 130
		Monster* en0 = new Monster("Skum,12,1,11,19,13,13,10,10,6,1,1,1,3,2");
		Monster* en1 = new Monster("Skum,12,1,11,19,13,13,10,10,6,1,1,1,3,2");
		Monster* en2 = new Monster("Skum,12,1,11,19,13,13,10,10,6,1,1,1,3,2");

		en0->equip_attack("Morso,2d6+4,0,0,0");
		en1->equip_attack("Morso,2d6+4,0,0,0");
		en2->equip_attack("Morso,2d6+4,0,0,0");

		characters[0] = pg0;
		characters[1] = pg1;
		characters[2] = pg2;
		characters[3] = pg3;
		
		enemies[0] = en0;
		enemies[1] = en1;
		//enemies[2] = en2;

		char* value = simulate(characters,N_CHARACTERS,enemies,N_MONSTERS);
		char** temp;
		split(value,',',&temp);
		c_value = c_value + atoi(temp[0]);
		m_value = m_value + atoi(temp[1]);
		turns = turns + atoi(temp[2]);
		casualties = casualties + atoi(temp[3]);

		for(int i=0;i<N_CHARACTERS;i++) {
			delete(characters[i]);
			characters[i] = NULL;
		}
		for(int i=0;i<N_MONSTERS;i++) {
			delete(enemies[i]);
			enemies[i] = NULL;
		}
	}

	c_value = c_value/NUMBER;
	m_value = m_value/NUMBER;
	turns = turns/NUMBER;
	casualties = casualties/NUMBER;
	printf("C_VALUE MEDIO = %d\nM_VALUE MEDIO = %d\nTURNI MEDI = %d\nMORTI MEDIE = %d\n",c_value,m_value,turns,casualties);

	return 0;
}