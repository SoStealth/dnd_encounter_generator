#include "equipments.hpp"

int main(int argc, char* argv[]) {
	char* myarmor = "Armatura in prestito,3,3,20";
	Armor armor(myarmor);
	printf("Hai trovato: %s\n",armor.get_name());
	printf("Il bonus di CA è %d\n",armor.get_ac_bonus());
	printf("La destrezza massima è %d\n",armor.get_dex_max());
	printf("La percentuale di fallimento incantesimi è %d",armor.get_magic_fail());
	char* serializator = armor.toString();
	printf("\nIl serializzatore per l'armatura è:\n%s\n",serializator);
	free(serializator);
	return 0;
}