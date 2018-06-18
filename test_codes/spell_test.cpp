#include "equipments.hpp"

int main(int argc, char* argv[]) {
	init_random();
	char* myspell = "Dardo dandy,3,6,10,0";
	Spell spell(myspell);
	printf("Hai trovato: %s\n",spell.get_name());
	printf("E' una spell di livello %d\n",spell.get_level());
	printf("E' una spell di guarigione? %d\n",spell.is_heal());
	printf("Il suo valore è 1d%d\n",spell.get_value());
	printf("Il tiro di salvezza è %d\n",spell.get_save_type());
	printf("La CD di questa spell è %d\n",spell.get_dc(1));
	char* serializator = spell.toString();
	printf("\nIl serializzatore per la spell è:\n%s\n",serializator);
	free(serializator);
	return 0;
}