#include "equipments.hpp"

int main(int argc, char* argv[]) {
	init_random();
	char* myitem = "Pozione annacquata di Levin,8,1";
	Item item(myitem);
	printf("Hai trovato: %s\n",item.get_name());
	printf("E' un item di guarigione? %d\n",item.is_heal());
	printf("Il suo valore è 1d%d\n",item.get_value());
	printf("Può essere usato %d volte\n",item.get_uses());
	for(int i=0;i<3;i++) {
		printf("\nTento di utilizzare l'oggetto...\n");
		bool used = item.use();
		if(used) {
			printf("Sono riuscito ad usare l'oggetto!\n");
			printf("Restano %d usi\n",item.get_uses());
		} else {
			printf("Non sono riuscito ad usare l'oggetto.\n");
			printf("Restano %d usi\n",item.get_uses());
		}
	}
	char* serializator = item.toString();
	printf("\nIl serializzatore per l'oggetto è:\n%s\n",serializator);
	free(serializator);
	return 0;
}