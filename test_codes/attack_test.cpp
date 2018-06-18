#include "equipments.hpp"

int main(int argc, char* argv[]) {
	init_random();
	char* myattack = "Spada scheggiata da senzatetto,8,2,2,2";
	Attack attack(myattack);
	printf("Hai trovato: %s\n",attack.get_name());
	printf("L'arma scala in %d\n",attack.get_scaling());
	int attack_roll;
	int damage_roll;
	int bab = 3;
	int str = 18;
	for(int i=0;i<3;i++) {
		printf("\nTento di eseguire un attacco con l'arma...\n");
		attack.make_attack(&attack_roll,&damage_roll,bab,get_modifier(str));
		printf("Il tiro per colpire è %d\n",attack_roll);
		printf("Il danno è %d\n",damage_roll);
	}
	char* serializator = attack.toString();
	printf("\nIl serializzatore per l'arma è:\n%s\n",serializator);
	free(serializator);
	return 0;
}