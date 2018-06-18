#include "demon.hpp"
#include "simulation.hpp"

#define C_VALUE 0
#define M_VALUE 1
#define TURNS 2
#define DEATHS 3
#define N_FACTORS 4

#define STRONG_THRESHOLD 15
#define WEAK_THRESHOLD -15
#define	CHANGE_UP 200
#define CHANGE_DOWN -200
#define MODIFY_UP 75
#define MODIFY_DOWN -75
#define TURNS_THRESHOLD 5

#define MAX_MONSTERS 120
#define MAX_PLAYERS 30
#define MAX_CD 22

#define N_SIMULATIONS 1000

#define RESULT_FILE "result.txt"

int initialize(Entity* *monsters,Entity* *players,int n_players) {
	int p_difficulty = 0;
	for(int i=0;i<n_players;i++) {
		p_difficulty = p_difficulty + players[i]->get_stat(LEVEL);
	}
	p_difficulty = (p_difficulty/n_players)+n_players-1;
	int n_monsters = rand()%n_players+1;
	int m_difficulty = p_difficulty - n_monsters;
	int monster_id;
	char* monster = get_random_monster_from_cd(&monster_id,m_difficulty);
	for(int i=0;i<n_monsters;i++) {
		monsters[i] = new Monster(monster);
		monsters[i]->equip_attack(get_attack_from_monster_id(monster_id));
	}
	return n_monsters;
}

void adjuste(Entity* *monsters,int n_monsters,int cd) {
	int monster_id;
	char* monster = get_random_monster_from_cd(&monster_id,cd);
	for(int k=0;k<n_monsters;k++) {
		monsters[k] = new Monster(monster);
		monsters[k]->equip_attack(get_attack_from_monster_id(monster_id));
	}
	return;
}

/*
>200 o <-200 : cambiare il nemico
*/
bool balance(char* balance_value, Entity* *monsters, int* n_monsters) {
	char** temp;
	int flag = 0;
	int factors[N_FACTORS];
	split(balance_value,',',&temp);
	for(int i=0;i<N_FACTORS;i++)
		factors[i] = atoi(temp[i]);
	int result = factors[C_VALUE] - factors[M_VALUE];
	if(result>WEAK_THRESHOLD && result<STRONG_THRESHOLD)
		return true;
	if(result>=CHANGE_UP) {		//Wrong monsters
		int cd = dynamic_cast<Monster*>(monsters[0])->get_cd();
		cd--;
		if(cd>=0) {
			adjuste(monsters,*n_monsters,cd);
			return false;
		} else {
			flag++;
		}
	}
	if((result>=MODIFY_UP && result<CHANGE_UP) || flag>0) {	//Right monsters but too many
		int id = *n_monsters-1;
		if(id>0) {
			delete(monsters[id]);
			monsters[id] = NULL;
			*n_monsters = *n_monsters - 1;
			return false;
		} else {
			flag++;
		}
	}
	if((result>=STRONG_THRESHOLD && result<MODIFY_UP) || flag>0) {	//Right monsters but a little too strong
		char* myattack = monsters[0]->get_attack(0)->toString();
		Attack* attack = new Attack(myattack);
		int nerf_pls = monsters[0]->get_stat(attack->get_scaling());
		if(nerf_pls>2) {
			for(int i=0;i<*n_monsters;i++) {
				myattack = monsters[i]->get_attack(0)->toString();
				attack = new Attack(myattack);
				nerf_pls = monsters[i]->get_stat(attack->get_scaling());
				nerf_pls = nerf_pls-2;
				monsters[i]->set_stat(attack->get_scaling(),nerf_pls);
			}
			return false;
		} else {
			return true;
		}
	}
	if(result<=CHANGE_DOWN) {		//Wrong monsters
		int cd = dynamic_cast<Monster*>(monsters[0])->get_cd();
		cd++;
		if(cd<=MAX_CD) {
			adjuste(monsters,*n_monsters,cd);
			return false;
		} else {
			flag--;
		}
	}
	if((result<=MODIFY_DOWN && result>CHANGE_DOWN) || flag<0) {	//Right monsters but too many
		int id = *n_monsters;
		if(id<MAX_MONSTERS) {
			char* mymonster = dynamic_cast<Monster*>(monsters[0])->toString();
			monsters[id] = new Monster(mymonster);
			return false;
		} else {
			flag--;
		}
	}
	if((result<=WEAK_THRESHOLD && result>MODIFY_DOWN) || flag<0) {	//Right monsters but a little too strong
		for(int i=0;i<*n_monsters;i++) {
			char* myattack = monsters[i]->get_attack(0)->toString();
			Attack* attack = new Attack(myattack);
			int buff_pls = monsters[i]->get_stat(attack->get_scaling());
			buff_pls = buff_pls+2;
			monsters[i]->set_stat(attack->get_scaling(),buff_pls);
		}
		return false;
	}
}

void create_balanced_encounter(Entity* *players,int n_players) {
	Entity* monsters[MAX_MONSTERS];
	int n_monsters;
	char* balance_value;
	int c_value = 0;
	int m_value = 0;
	int turns = 0;
	int casualties = 0;

	printf("Initializing generator...\n");
	init_random();
	printf("Generator ready!\n");

	printf("Creating the first group of monsters...\n");
	n_monsters = initialize(monsters,players,n_players);
	printf("Group created!\n");

	printf("Starting simulation...\n");
	do{
		printf("Engage in battle...\n");
		for(int c=0;c<N_SIMULATIONS;c++) {
			balance_value = simulate(players,n_players,monsters,n_monsters);
			char** temp;
			split(balance_value,',',&temp);
			c_value = c_value + atoi(temp[0]);
			m_value = m_value + atoi(temp[1]);
			turns = turns + atoi(temp[2]);
			casualties = casualties + atoi(temp[3]);
		}
		printf("Battle ended, checking result...\n");
		c_value = c_value/N_SIMULATIONS;
		m_value = m_value/N_SIMULATIONS;
		turns = turns/N_SIMULATIONS;
		casualties = casualties/N_SIMULATIONS;
		sprintf(balance_value,"%d,%d,%d,%d\n",c_value,m_value,turns,casualties);
	} while(!balance(balance_value,monsters,&n_monsters));

	printf("Balanced group created! Printing on file...\n");
	char entry[MAX_BUFFER];
	start_log(RESULT_FILE);
	for(int i=0;i<n_monsters;i++) {
		sprintf(entry,"%s\n",monsters[i]->toString());
		write_log(RESULT_FILE,entry);
	}

	printf("Done. Good luck!\n");
	return;

}
