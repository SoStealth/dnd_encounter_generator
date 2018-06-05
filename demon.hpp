#include <stdio.h>
#include "sqlite3.h"
#include <string.h>

#define SELECT "SELECT * FROM "
#define WHERE " WHERE "
#define MONSTERS "Monsters"
#define ANIMALS "Animals"
#define CHARACTERS "Characters"

class Demon{
private:sqlite3* db_monsters;
	sqlite3* db_animals;
	sqlite3* db_characters;
public:	Demon();
	~Demon();
	char* get_monster_from_id();
	char* get_animal_from_id();
	char* get_character_from_id();
	char* get_monster_from_cd();
	char* get_character_from_name();
	bool insert_monster();
	bool insert_animal();
	bool insert_character();
}
Demon::Demon() {
	int ret;
	ret = sqlite3_open("monsters.sqlite",&db_monsters);
	if(ret) {
		printf("Errore nell'apertura del database monsters.sqlite\n");
		return -1;
	}
	ret = sqlite3_open("animals.sqlite",&db_animals);
	if(ret) {
		printf("Errore nell'apertura del database animals.sqlite\n");
		return -2;
	}
	ret = sqlite3_open("characters.sqlite",&db_characters);
	if(ret) {
		printf("Errore nell'apertura del database characters.sqlite\n");
		return -3;
	}
}
Demon::~Demon() {
	sqlite3_close(db_monsters);
	sqlite3_close(db_animals);
	sqlite3_close(db_characters); 
}
char* Demon::get_monster_from_id() {
	
}
