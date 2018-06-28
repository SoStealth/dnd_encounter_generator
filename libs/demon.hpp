#include <stdio.h>
#include "sqlite3.h"
#include <string.h>

#ifndef __ENTITIES
#define __ENTITIES
#include "entities.hpp"
#endif

#define MONSTERS_DB "libs/databases/monsters"
#define ANIMALS_DB "libs/databases/animals"
#define CHARACTERS_DB "libs/databases/characters"

#define INSERT_QUERY "INSERT INTO Characters(Name,Class,Level,HP,STR,DEX,CON,INT,WIS,CHA,BAB,FORT,REFL,WILL,Party) VALUES "

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
//---------------------------------------------------------------------------------------------
void save_group(Entity* *characters, int n_characters, char* group_name) {
	sqlite3* db_characters;
	sqlite3_open(CHARACTERS_DB,&db_characters);
	char query[MAX_BUFFER];
	sprintf(query,"SELECT MAX(ID) FROM Party");
	char value[CALLBACK_BUFFER];
	char* error;
	sqlite3_exec(db_characters,query,callback,value,&error);
	int id = atoi(value);
	id++;
	for(int i=0;i<n_characters;i++) {
		char buffer[MAX_BUFFER];
		sprintf(buffer,"\'%s\'",characters[i]->get_name());
		for(int k=0;k<N_STATS;k++) {
			sprintf(buffer,"%s,\'%d\'",buffer,characters[i]->get_stat(k));
		}
		sprintf(buffer,"%s,\'%d\'",buffer,id);
		sprintf(query,"%s(%s)",INSERT_QUERY,buffer);
		sqlite3_exec(db_characters,query,NULL,NULL,&error);
	}
	fflush(stdin);
	sprintf(query,"INSERT INTO Party(ID,Name) VALUES (\'%d\',\'%s\')",id,group_name);
	sqlite3_exec(db_characters,query,NULL,NULL,&error);
	sqlite3_close(db_characters);
	sprintf(value,"");
}
void load_group(Entity* *characters,int* n_characters,char* name) {
	sqlite3* db_characters;
	char query[MAX_BUFFER];
	sprintf(query,"SELECT ID FROM Party WHERE Name LIKE \"%s\"",name);
	char value[CALLBACK_BUFFER];
	char* error;
	fflush(stdin);
	sqlite3_open(CHARACTERS_DB,&db_characters);
	sqlite3_exec(db_characters,query,callback,value,&error);
	sqlite3_close(db_characters);
	int id = atoi(value);
	for(int i=0;i<*n_characters;i++) {
		delete(characters[i]);
	}
	*n_characters = 0;
	char pgs[CALLBACK_BUFFER];
	sprintf(query,"SELECT * FROM Characters WHERE Party = %d",id);
	fflush(stdin);
	sqlite3_open(CHARACTERS_DB,&db_characters);
	sqlite3_exec(db_characters,query,callback,pgs,&error);
	sqlite3_close(db_characters);
	char** roaster;
	int count = split(pgs,';',&roaster);
	for(int i=0;i<count;i++) {
		char character[MAX_BUFFER];
		char** temp;
		split(roaster[i],',',&temp);
		sprintf(character,"%s",temp[1]);
		for(int k=2;k<=N_STATS+1;k++) {
			sprintf(character,"%s,%s",character,temp[k]);
		}
		int classe = atoi(temp[CLASS+2]);
		switch(classe) {
			case BARBARIAN:
				characters[*n_characters] = new Barbarian(character);
				//equipaggiamento
				break;
			case BARD:
				characters[*n_characters] = new Bard(character);
				//equipaggiamento
				break;
			case CLERIC:
				characters[*n_characters] = new Cleric(character);
				//equipaggiamento
				break;
			case DRUID:
				characters[*n_characters] = new Druid(character);
				//equipaggiamento
				break;
			case SORCERER:
				characters[*n_characters] = new Sorcerer(character);
				//equipaggiamento
				break;
			case WIZARD:
				characters[*n_characters] = new Wizard(character);
				//equipaggiamento
				break;
			case MONK:
				characters[*n_characters] = new Monk(character);
				//equipaggiamento
				break;
			case FIGHTER:
				characters[*n_characters] = new Fighter(character);
				//equipaggiamento
				break;
			case RANGER:
				characters[*n_characters] = new Ranger(character);
				//equipaggiamento
				break;
			case ROGUE:
				characters[*n_characters] = new Rogue(character);
				//equipaggiamento
				break;
			case PALADIN:
				characters[*n_characters] = new Paladin(character);
				//equipaggiamento
				break;
		}
		*n_characters = *n_characters + 1;
	}
	sprintf(value,"");
	sprintf(pgs,"");
}
//----------------------------------------------------------------------------
void save_character(Entity* character) {
	sqlite3* db_characters;
	sqlite3_open(CHARACTERS_DB,&db_characters);
	char query[MAX_BUFFER];
	char buffer[MAX_BUFFER];
	char* error;
	sprintf(buffer,"\'%s\'",character->get_name());
	for(int k=0;k<N_STATS;k++) {
		sprintf(buffer,"%s,\'%d\'",buffer,character->get_stat(k));
	}
	sprintf(buffer,"%s,\'0\'",buffer);
	sprintf(query,"%s(%s)",INSERT_QUERY,buffer);
	sqlite3_exec(db_characters,query,NULL,NULL,&error);
	fflush(stdin);
	sqlite3_close(db_characters);
	sprintf(buffer,"");
}
void load_character(Entity* *characters,int* n_characters,char* name) {
	sqlite3* db_characters;
	char query[MAX_BUFFER];
	char pg[CALLBACK_BUFFER];
	char* error;
	sprintf(query,"SELECT * FROM Characters WHERE Name LIKE \"%s\"",name);
	fflush(stdin);
	sqlite3_open(CHARACTERS_DB,&db_characters);
	sqlite3_exec(db_characters,query,callback,pg,&error);
	sqlite3_close(db_characters);
	char** roaster;
	int count = split(pg,';',&roaster);
	char** temp;
	split(roaster[0],',',&temp);
	char character[MAX_BUFFER];
	sprintf(character,"%s",temp[1]);
	for(int k=2;k<=N_STATS+1;k++) {
		sprintf(character,"%s,%s",character,temp[k]);
	}
	int classe = atoi(temp[CLASS+2]);
	switch(classe) {
		case BARBARIAN:
			characters[*n_characters] = new Barbarian(character);
			//equipaggiamento
			break;
		case BARD:
			characters[*n_characters] = new Bard(character);
			//equipaggiamento
			break;
		case CLERIC:
			characters[*n_characters] = new Cleric(character);
			//equipaggiamento
			break;
		case DRUID:
			characters[*n_characters] = new Druid(character);
			//equipaggiamento
			break;
		case SORCERER:
			characters[*n_characters] = new Sorcerer(character);
			//equipaggiamento
			break;
		case WIZARD:
			characters[*n_characters] = new Wizard(character);
			//equipaggiamento
			break;
		case MONK:
			characters[*n_characters] = new Monk(character);
			//equipaggiamento
			break;
		case FIGHTER:
			characters[*n_characters] = new Fighter(character);
			//equipaggiamento
			break;
		case RANGER:
			characters[*n_characters] = new Ranger(character);
			//equipaggiamento
			break;
		case ROGUE:
			characters[*n_characters] = new Rogue(character);
			//equipaggiamento
			break;
		case PALADIN:
			characters[*n_characters] = new Paladin(character);
			//equipaggiamento
			break;
	}
	*n_characters = *n_characters + 1;
	sprintf(pg,"");
}
//-----------------------------------------------------------------------------
void load_character_equipment(Entity* character) {
	sqlite3* db_characters;
	char query[MAX_BUFFER];
	int classe = character->get_stat(CLASS);
	sqlite3_open(CHARACTERS_DB,&db_characters);
	sprintf(query,"SELECT * FROM Armors WHERE Class = %d",classe);
	char buffer[CALLBACK_BUFFER];
	char* error;
	sprintf(buffer,"");
	sqlite3_exec(db_characters,query,callback,buffer,&error);
	char** temp;
	int count;
	if(strlen(buffer)>0) {
		count = split(buffer,';',&temp);
		for(int i=0;i<count;i++) {
			char armor[MAX_BUFFER];
			char** temp2;
			int count2 = split(temp[i],',',&temp2);
			sprintf(armor,"%s",temp2[1]);
			for(int k=2;k<5;k++) {
				sprintf(armor,"%s,%s",armor,temp2[k]);
			}
			printf("%s\n",armor);
			character->equip_armor(armor);
		}
	}
	sprintf(query,"SELECT * FROM Weapons WHERE Class = %d",classe);
	sprintf(buffer,"");
	sqlite3_exec(db_characters,query,callback,buffer,&error);
	if(strlen(buffer)>0) {
		count = split(buffer,';',&temp);
		for(int i=0;i<count;i++) {
			char attack[MAX_BUFFER];
			char** temp2;
			int count2 = split(temp[i],',',&temp2);
			sprintf(attack,"%s",temp2[1]);
			for(int k=2;k<6;k++) {
				sprintf(attack,"%s,%s",attack,temp2[k]);
			}
			printf("%s\n",attack);
			character->equip_attack(attack);
		}
	}
	sprintf(query,"SELECT * FROM Items WHERE Class = %d",classe);
	sprintf(buffer,"");
	sqlite3_exec(db_characters,query,callback,buffer,&error);
	if(strlen(buffer)>0) {
		count = split(buffer,';',&temp);
		for(int i=0;i<count;i++) {
			char item[MAX_BUFFER];
			char** temp2;
			int count2 = split(temp[i],',',&temp2);
			sprintf(item,"%s",temp2[1]);
			for(int k=2;k<4;k++) {
				sprintf(item,"%s,%s",item,temp2[k]);
			}
			printf("%s\n",item);
			character->equip_item(item);
		}
	}
	sprintf(query,"SELECT * FROM Spells WHERE Class = %d",classe);
	sprintf(buffer,"");
	sqlite3_exec(db_characters,query,callback,buffer,&error);
	if(strlen(buffer)>0) {
		count = split(buffer,';',&temp);
		for(int i=0;i<count;i++) {
			char spell[MAX_BUFFER];
			char** temp2;
			int count2 = split(temp[i],',',&temp2);
			sprintf(spell,"%s",temp2[1]);
			for(int k=2;k<6;k++) {
				sprintf(spell,"%s,%s",spell,temp2[k]);
			}
			printf("%s\n",spell);
			dynamic_cast<Caster*>(character)->equip_spell(spell);
		}
	}
	sqlite3_close(db_characters);
}