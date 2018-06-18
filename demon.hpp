#include <stdio.h>
#include "sqlite3.h"
#include <string.h>

#ifndef __ENTITIES
#define __ENTITIES
#include "entities.hpp"
#endif

#define MONSTERS_DB "databases/monsters"
#define ANIMALS_DB "databases/animals"
#define CHARACTERS_DB "databases/characters"

#define CALLBACK_BUFFER 10500

//--------------------------------------------------------------------------------------
int callback(void* s, int count, char** data, char** columns) {
	char* candidates = (char*)s;
	char record[MAX_BUFFER];
	sprintf(record,"%s",data[0]);
	for(int i=1;i<count;i++) {
		sprintf(record,"%s,%s",record,data[i]);
	}
	if(strlen(candidates)==0) {
		sprintf(candidates,"%s",record);
	} else {
		sprintf(candidates,"%s;%s",candidates,record);	
	}
	return 0;
}
//---------------------------------------------------------------------------------------
char* get_random_monster_from_cd(int* id,int cd) {	//restituisce l'ID del mostro trovato
	sqlite3* db_monsters;
	sqlite3_open(MONSTERS_DB,&db_monsters);
	char query[MAX_BUFFER];
	sprintf(query,"SELECT * FROM Monsters WHERE CD = %d\n",cd);
	char candidates[CALLBACK_BUFFER];
	char* error;
	sqlite3_exec(db_monsters,query,callback,candidates,&error);
	char** roaster;
	int count = split(candidates,';',&roaster);
	int chosen_one_id = rand()%count;
	char** temp;
	count = split(roaster[chosen_one_id],',',&temp);
	char buffer[MAX_BUFFER];
	sprintf(buffer,"%s",temp[1]);
	for(int i=2;i<=N_STATS_MONSTER+1;i++) {
		sprintf(buffer,"%s,%s",buffer,temp[i]);
	}
	*id = atoi(temp[0]);
	sqlite3_close(db_monsters);
	sprintf(candidates,"");
	return strdup(buffer);
}
char* get_attack_from_monster_id(int id) {	//L'ID serve per trovare gli attacchi del mostro
	sqlite3* db_monsters;
	sqlite3_open(MONSTERS_DB,&db_monsters);
	char query[MAX_BUFFER];
	sprintf(query,"SELECT * FROM Attacks WHERE ID = %d\n",id);
	char attacks[CALLBACK_BUFFER];
	char* error;
	sqlite3_exec(db_monsters,query,callback,attacks,&error);
	char** roaster;
	int count = split(attacks,';',&roaster);
	int chosen_one_id = rand()%count;
	char** temp;
	count = split(roaster[chosen_one_id],',',&temp);
	char buffer[MAX_BUFFER];
	sprintf(buffer,"%s",temp[1]);
	for(int i=2;i<count;i++) {
		sprintf(buffer,"%s,%s",buffer,temp[i]);
	}
	sqlite3_close(db_monsters);
	sprintf(attacks,"");
	return strdup(buffer);
}