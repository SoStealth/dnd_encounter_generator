#include "balancer.hpp"

int main(int argc, char* argv[]) {
	Entity* enemies[1000];
	Entity* characters[1000];
	int n_monsters = 2;

	init_random();

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

	characters[0] = pg0;
	characters[1] = pg1;
	characters[2] = pg2;
	characters[3] = pg3;

	/*
	Monster* en0 = new Monster("Skum,12,1,11,19,13,13,10,10,6,1,1,1,3,2,2");
	Monster* en1 = new Monster("Skum,12,1,11,19,13,13,10,10,6,1,1,1,3,2,2");

	en0->equip_attack("Morso,2d6,0,0,3");
	en1->equip_attack("Morso,2d6,0,0,3");

	enemies[0] = en0;
	enemies[1] = en1;

	adjuste(enemies,n_monsters,1);
	*/

	n_monsters = initialize(enemies,characters,4);

	for(int e=0;e<n_monsters;e++) {
		printf("%s\n",dynamic_cast<Monster*>(enemies[e])->toString());	
		printf("%s\n",enemies[e]->get_attack(0)->toString());
	}
}