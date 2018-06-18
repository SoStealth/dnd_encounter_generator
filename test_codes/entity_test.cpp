#include "entities.hpp"

int main(int argc, char* argv[]) {
	char* myentity = "Lino Mendol,1,10,8,10,14,8,14,16,0,0,0,2";
	Entity entity(myentity);
	printf("Hai incontrato %s\n",entity.get_name());
	for(int i=0;i<N_STATS;i++) {
		printf("Stat[%d]=%d\n",i,entity.get_stat(i));
	}
	printf("La sua classe armatura vale %d\n",entity.get_ac());
	printf("I suoi hp attuali sono %d\n",entity.get_current_hp());
	if(entity.is_alive()) {
		printf("%s è vivo\n",entity.get_name());
	} else {
		printf("%s è morto\n",entity.get_name());
	}
	printf("\n");
	printf("Attacco %s con una padella infliggendo 5 danni\n",entity.get_name());
	entity.hit(5);
	printf("I suoi hp attuali adesso sono %d\n",entity.get_current_hp());
	printf("Attacco di nuovo infliggendo 20 danni\n");
	entity.hit(20);
	printf("I suoi hp attuali adesso sono %d\n",entity.get_current_hp());
	if(entity.is_alive()) {
		printf("%s è vivo\n",entity.get_name());
	} else {
		printf("%s è morto\n",entity.get_name());
	}
	printf("Proviamo a curarlo per 50 hp\n");
	entity.heal(50);
	printf("I suoi hp attuali adesso sono %d\n",entity.get_current_hp());
	printf("\n");
	char* myattack = "Spada scheggiata da senzatetto,8,2,2,2";
	Attack attack(myattack);
	printf("Hai trovato: %s\n",attack.get_name());
	printf("Proviamo ad equipaggiare la nuova arma sul pg\n");
	if(entity.equip_attack(attack.toString())) {
		printf("Abbiamo equipaggiato %s\n",attack.get_name());
		printf("Controlliamo se possiamo recuperarla\n");
		Attack* x = entity.get_attack(0);
		if(x!=NULL) {
			printf("Siamo riusciti a recuperare l'arma!\n");
		} else {
			printf("Non sono riuscito a recuperare l'arma\n");
		}
		printf("Adesso disequipaggiamo l'arma\n");
		if(entity.unequip_attack(0)) {
			printf("Siamo riusciti a disequipaggiare l'arma!\n");
		} else {
			printf("Non siamo riusciti a disequipaggiare l'arma\n");
		}
	} else {
		printf("Non sono riuscito ad equipaggiare l'arma\n");
	}
	printf("\n");
	char* myarmor = "Armatura in prestito,3,3,20";
	Armor armor(myarmor);
	printf("Hai trovato: %s\n",armor.get_name());
	printf("Proviamo ad equipaggiare la nuova armatura sul pg\n");
	if(entity.equip_armor(armor.toString())) {
		printf("Abbiamo equipaggiato %s\n",armor.get_name());
		printf("Testiamo la nuova classe armatura: %d\n",entity.get_ac());
		printf("Controlliamo se possiamo recuperarla\n");
		Armor* y = entity.get_armor(0);
		if(y!=NULL) {
			printf("Siamo riusciti a recuperare l'armatura!\n");
		} else {
			printf("Non sono riuscito a recuperare l'armatura\n");
		}
		printf("Adesso disequipaggiamo l'armatura\n");
		if(entity.unequip_armor(0)) {
			printf("Siamo riusciti a disequipaggiare l'armatura!\n");
		} else {
			printf("Non siamo riusciti a disequipaggiare l'armatura\n");
		}
	} else {
		printf("Non sono riuscito ad equipaggiare l'armatura\n");
	}
	printf("\n");
	char* myitem = "Pozione annacquata di Levin,8,1";
	Item item(myitem);
	printf("Hai trovato: %s\n",item.get_name());
	printf("Proviamo ad equipaggiare il nuovo item sul pg\n");
	if(entity.equip_item(item.toString())) {
		printf("Abbiamo equipaggiato %s\n",item.get_name());
		printf("Controlliamo se possiamo recuperarlo\n");
		Item* z = entity.get_item(0);
		if(z!=NULL) {
			printf("Siamo riusciti a recuperare l'item!\n");
		} else {
			printf("Non sono riuscito a recuperare l'item\n");
		}
		printf("Adesso disequipaggiamo l'item\n");
		if(entity.unequip_item(0)) {
			printf("Siamo riusciti a disequipaggiare l'item!\n");
		} else {
			printf("Non siamo riusciti a disequipaggiare l'item\n");
		}
	} else {
		printf("Non sono riuscito ad equipaggiare l'item\n");
	}
	printf("\n");
	char* myenemy = "Homo politecnicus,1,30,6,2,14,8,14,16,0,0,0,2";
	Entity enemy(myenemy);
	printf("Oh noes! Appare %s selvatico! (HP: %d)\n",enemy.get_name(),enemy.get_current_hp());
	printf("E' il momento di duellare! Evoco %s in posizione di attacco\n",attack.get_name());
	init_random();
	for(int i=0;i<3;i++) {
		if(entity.attack(&enemy,&attack)) {
			printf("Danno diretto! HP rimanenti del nemico: %d\n",enemy.get_current_hp());
		} else {
			printf("Oh no, la sua aura è potentissima!!1!!1!1\n");
		}
	}
	printf("\n");
	printf("%s ci saluta. Alla prossima!\n",entity.get_name());
	printf("Serializator: %s\n",entity.toString());
	return 0;
}