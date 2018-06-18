#include "entities.hpp"

int main(int argc, char* argv[]) {
	char* myentity = "Enrico Vasaio,1,10,8,10,14,8,14,16,0,0,0,2";
	Wizard entity(myentity);
	printf("Hai incontrato %s\n",entity.get_name());
	printf("\n");
	char* myattack = "Spada scheggiata da senzatetto,8,2,2,2";
	Attack attack(myattack);
	printf("Hai trovato: %s\n",attack.get_name());
	printf("Proviamo ad equipaggiare la nuova arma sul pg\n");
	if(entity.equip_attack(attack.toString())) {
		printf("Abbiamo equipaggiato %s\n",attack.get_name());
	} else {
		printf("Non sono riuscito ad equipaggiare l'arma\n");
	}
	printf("\n");
	char* myitem = "Pozione annacquata di Levin,8,1";
	Item item(myitem);
	printf("Hai trovato: %s\n",item.get_name());
	printf("Proviamo ad equipaggiare il nuovo item sul pg\n");
	if(entity.equip_item(item.toString())) {
		printf("Abbiamo equipaggiato %s\n",item.get_name());
	} else {
		printf("Non sono riuscito ad equipaggiare l'item\n");
	}
	printf("\n");
	char* myenemy = "Homo politecnicus,1,30,6,2,14,8,14,16,0,0,0,2";
	Entity enemy(myenemy);
	printf("Oh noes! Appare %s selvatico! (HP: %d)\n",enemy.get_name(),enemy.get_current_hp());
	printf("E' il momento di duellare! Evoco %s in posizione di attacco\n",attack.get_name());
	init_random();
	if(entity.attack(&enemy)) {
		printf("Danno diretto! HP rimanenti del nemico: %d\n",enemy.get_current_hp());
	} else {
		printf("Oh no, la sua aura è potentissima!!1!!1!1\n");
	}
	printf("\n");
	printf("%s colpisce %s per 6 danni\n",enemy.get_name(),entity.get_name());
	entity.hit(5);
	printf("HP rimanenti: %d\n",entity.get_current_hp());
	printf("Proviamo a curarci\n");
	entity.heal_with_item();
	printf("HP rimanenti: %d\n",entity.get_current_hp());
	printf("Adesso proviamo a colpire il nemico con una spell\n");
	char* myspell = "Dardo dandy,3,6,10,0";
	Spell spell(myspell);
	entity.equip_spell(myspell);
	if(entity.cast(&enemy,false)) {
		printf("Danno diretto! HP rimanenti del nemico: %d\n",enemy.get_current_hp());
	} else {
		printf("Oh no, la sua aura è potentissima!!1!!1!1\n");
	}
	printf("\n");
	printf("%s ci saluta. Alla prossima!\n",entity.get_name());
	printf("Serializator: %s\n",entity.toString());
	return 0;
}