#include "balancer.hpp"

#define MAX_CHARACTERS 30
#define MAX_NAME 40
#define MAX_LEVEL 30

#define HEADER "***********************************\n\
* GENERATORE OTTIMALE DI INCONTRI *\n\
* made by Lorenzo Damiano         *\n\
***********************************\n\n"

#define MENU_START "-- Menù principale --\n\n\
1. Genera incontro\n\
2. Esci\n\
\n\
Seleziona un'opzione > "

#define MENU_GENERATOR "-- Genera incontro --\n\n\
1. Gestione party\n\
2. Visualizza party\n\
3. Avvia il generatore\n\
4. Indietro\n\
\n\
Seleziona un'opzione > "

#define MENU_CREATE_PARTY "-- Creazione party --\n\n\
1. Aggiungi personaggio\n\
2. Carica personaggio\n\
3. Elimina personaggio\n\
4. Visualizza party\n\
5. Carica party\n\
6. Salva party\n\
7. Indietro\n\
\n\
Seleziona un\'opzione > "

#define CLASSES "0. Barbaro\t6. Monaco\n\
1. Bardo\t7. Guerriero\n\
2. Chierico\t8. Ranger\n\
3. Druido\t9. Ladro\n\
4. Stregone\t10. Paladino\n\
5. Mago\n\
\n\
Inserisci la classe del personaggio > "
//------------------------------------------------------------------------------------------
void show_group(Entity* *characters,int n_characters) {
	char c;
	clear();
	if(n_characters<1) {
		addstr("Il party è vuoto :(\nPremere un tasto per continuare...");
		c = getch();
		return;
	}
	addstr("Membri del party:\n\n");
	refresh();
	for(int i=0;i<n_characters;i++) {
		printw("%s, livello %d\n",characters[i]->get_name(),characters[i]->get_stat(LEVEL));
		printw("Statistiche:\n- HP: %d\n- CA: %d\n- FORZA: %d\n- DESTREZZA: %d\n- COSTITUZIONE: %d\n- INTELLIGENZA: %d\n- SAGGEZZA: %d\n- CARISMA: %d\n\n",
				characters[i]->get_stat(HP),
				characters[i]->get_ac(),
				characters[i]->get_stat(STR),
				characters[i]->get_stat(DEX),
				characters[i]->get_stat(CON),
				characters[i]->get_stat(CLE),
				characters[i]->get_stat(WIS),
				characters[i]->get_stat(CHA));
		addstr("\nPremere un tasto per continuare...\n");
		refresh();
		getch();
		clear();
	}
	return;
}
//------------------------------------------------------------------------------------------
void delete_character(Entity* *characters,int* n_characters) {
	char c;
	clear();
	if(*n_characters>0) {
		int id = *n_characters - 1;
		delete(characters[id]);
		*n_characters = *n_characters - 1;
		addstr("L'ultimo personaggio è stato eliminato.\nPremere un tasto per continuare...");
	} else {
		addstr("Non sono presenti personaggi nel gruppo.\nPremere un tasto per continuare...");
	}
	refresh();
	c = getch();

	return;
}
//------------------------------------------------------------------------------------------
void add_character(Entity* *characters,int* n_characters) {
	char character[MAX_BUFFER];
	char buffer[MAX_BUFFER];
	bool correct = false;

	echo();

	while(!correct) {
		clear();
		addstr("Inserisci il nome del personaggio (massimo 40 caratteri) \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		if(strlen(buffer)<=MAX_NAME) {
			correct = true;
		}
	}
	sprintf(character,"%s",buffer);
	clear();

	correct = false;
	int classe;
	while(!correct) {
		addstr(CLASSES);
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		classe = atoi(buffer);
		if(classe>=0 && classe<11) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,classe);
	clear();

	correct = false;
	int level;
	while(!correct) {
		addstr("Inserisci il livello del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		level = atoi(buffer);
		if(level<MAX_LEVEL && level>0) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,level);
	clear();

	correct = false;
	int hp;
	while(!correct) {
		addstr("Inserisci i punti vita del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		hp = atoi(buffer);
		if(hp>0) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,hp);
	clear();

	correct = false;
	int str;
	while(!correct) {
		addstr("Inserisci la forza del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		str = atoi(buffer);
		if(str>2) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,str);
	clear();

	correct = false;
	int dex;
	while(!correct) {
		addstr("Inserisci la destrezza del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		dex = atoi(buffer);
		if(dex>2) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,dex);
	clear();

	correct = false;
	int cos;
	while(!correct) {
		addstr("Inserisci la costituzione del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		cos = atoi(buffer);
		if(cos>2) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,cos);
	clear();

	correct = false;
	int intel;
	while(!correct) {
		addstr("Inserisci l'intelligenza del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		intel = atoi(buffer);
		if(intel>2) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,intel);
	clear();

	correct = false;
	int wis;
	while(!correct) {
		addstr("Inserisci la saggezza del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		wis = atoi(buffer);
		if(wis>2) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,wis);
	clear();

	correct = false;
	int cha;
	while(!correct) {
		addstr("Inserisci il carisma del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		cha = atoi(buffer);
		if(cha>2 && cha<100) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,cha);
	clear();

	correct = false;
	int bab;
	while(!correct) {
		addstr("Inserisci il bonus di attacco base del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		bab = atoi(buffer);
		if(bab>=0) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,bab);
	clear();

	correct = false;
	int fort;
	while(!correct) {
		addstr("Inserisci il tiro di salvezza base sulla tempra del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		fort = atoi(buffer);
		if(fort>=0) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,fort);
	clear();

	correct = false;
	int refl;
	while(!correct) {
		addstr("Inserisci il tiro di salvezza base sui riflessi del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		refl = atoi(buffer);
		if(refl>=0) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,refl);
	clear();

	correct = false;
	int will;
	while(!correct) {
		addstr("Inserisci il tiro di salvezza base sulla volontà del personaggio \n> ");
		refresh();
		getnstr(buffer,sizeof(buffer)-1);
		will = atoi(buffer);
		if(will>=0) {
			correct = true;
		}
	}
	sprintf(character,"%s,%d",character,will);
	clear();

	switch(classe) {
		case BARBARIAN:
			characters[*n_characters] = new Barbarian(character);
			break;
		case BARD:
			characters[*n_characters] = new Bard(character);
			break;
		case CLERIC:
			characters[*n_characters] = new Cleric(character);
			break;
		case DRUID:
			characters[*n_characters] = new Druid(character);
			break;
		case SORCERER:
			characters[*n_characters] = new Sorcerer(character);
			break;
		case WIZARD:
			characters[*n_characters] = new Wizard(character);
			break;
		case MONK:
			characters[*n_characters] = new Monk(character);
			break;
		case FIGHTER:
			characters[*n_characters] = new Fighter(character);
			break;
		case RANGER:
			characters[*n_characters] = new Ranger(character);
			break;
		case ROGUE:
			characters[*n_characters] = new Rogue(character);
			break;
		case PALADIN:
			characters[*n_characters] = new Paladin(character);
			break;
	}
	load_character_equipment(characters[*n_characters]);
	clear();
	addstr("Vuoi aggiungere il personaggio al database?\n\n1. Sì\n2. No\n\n> ");
	getnstr(buffer,sizeof(buffer)-1);
	if(atoi(buffer)==1) {
		save_character(characters[*n_characters]);
	}
	if(atoi)
	addstr("\nIl personaggio è stato aggiunto al party!\nPremere un tasto per continuare...");
	refresh();
	getch();
	noecho();
	*n_characters = *n_characters + 1;
	return;
}
//------------------------------------------------------------------------------------------
void create_group(Entity* *characters,int* n_characters) {
	bool exit = false;
	char buffer[MAX_BUFFER];

	while(!exit){
		char c;
		clear();
		addstr(HEADER);
		addstr(MENU_CREATE_PARTY);
		refresh();

		c = getch();
		switch(c) {
			case '1':
				add_character(characters,n_characters);
			break;
			case '2':
				clear();
				echo();
				addstr("Inserisci il nome del personaggio > ");
				refresh();
				getnstr(buffer,sizeof(buffer)-1);
				load_character(characters,n_characters,buffer);
				noecho();
			break;
			case '3':
				delete_character(characters,n_characters);
			break;
			case '4':
				show_group(characters,*n_characters);
			break;
			case '5':
				clear();
				echo();
				addstr("Inserisci il nome del gruppo > ");
				refresh();
				getnstr(buffer,sizeof(buffer)-1);
				load_group(characters,n_characters,buffer);
				noecho();
			break;
			case '6':
				clear();
				echo();
				addstr("Inserisci il nome del gruppo > ");
				refresh();
				getnstr(buffer,sizeof(buffer)-1);
				save_group(characters,*n_characters,buffer);
				noecho();
			break;
			case '7':
				exit = true;
			break;
			default:
			break;
		}
	}

	return;
}
//------------------------------------------------------------------------------------------
void generator() {
	bool exit = false;
	bool loaded = false;
	Entity* characters[MAX_CHARACTERS];
	int n_characters = 0;
	char buffer[MAX_BUFFER];

	while(!exit){
		char c;
		clear();
		addstr(HEADER);
		addstr(MENU_GENERATOR);
		refresh();

		c = getch();
		switch(c) {
			case '1':
				create_group(characters,&n_characters);
			break;
			case '2':
				show_group(characters,n_characters);
			break;
			case '3':
				if(n_characters>0) {
					create_balanced_encounter(characters,n_characters);
				} else {
					clear();
					addstr("ATTENZIONE: nessun party inserito.\nVai nella sezione \"Gestione party\" per impostare il party.\nPremere un tasto per continuare...");
					getch();
				}	
			break;
			case '4':
				exit = true;
			break;
			default:
			break;
		}
	}

	return;
}
//------------------------------------------------------------------------------------------