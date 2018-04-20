#define N_STATS 8		//Number of stats for a single entity
#define HP 1			//Hit points, the "life" of an entity
#define AC 2			//Armor class, defence of the enemy
#define STR 3			//Strength attribute
#define DEX 4			//Dexterity attribute
#define CON 5			//Constitution attribute
#define INT 6			//Intelligence attribute
#define WIS 7			//Wisdom attribute
#define CHA 8			//Charisma attribute

//MAX constants
#define MAX_NAME 30		//Maximum length allowed for the entity name
#define MAX_ARMOR 2		//Max number of armor an entity can have
#define MAX_ATTACKS 5		//Max number of attacks an entity can have

class Equipment{};

class Armor{};

class Attack{};

class Entity{
private:char name[MAX_NAME];
	int stats[N_STATS];
	Armor armors[MAX_ARMOR];
	Attack attack[MAX_ATTACKS];
public:	Entity(char*,bool);	//Receives name identifier for the entity OR filename and a boolean to identify which one
	~Entity();
	int get_parameter(int);	//Receives parameter identifier
	char* get_name();	//Receives name of entity
	int attack
};
Entity::Entity(char* filename) {
	FILE* file = fopen(filename,"r");
	//structure: "name,HP,AC,STR,DEX,CON,INT,WIS,CHAR"
	char c = fgetc(file);
	int i = 0;
	while(c!=EOF && c!=',') {
		name[i]=c;
		i++;
	}
	c = fgetc(file);
	for(i=0;i<N_STATS;i++) {
		
	}
}
