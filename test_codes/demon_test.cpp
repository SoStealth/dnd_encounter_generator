#include "balancer.hpp"

int main(int argc,char* argv[]) {
	init_random();
	Demon* demon = new Demon();
	char* monster; 
	int id;
	monster = demon->get_random_monster_from_cd(&id,1);
	printf("Risultato finale = %s\n",monster);
	free(monster);
}