#define N_STATS 8
#define HP 1
#define AC 2
#define STR 3
#define DEX 4
#define CON 5
#define INT 6
#define WIS 7
#define CHA 8
#define MAX_NAME 30

class Entity{
private:char name[MAX_NAME];
		int stats[N_STATS];
public:	Entity(filename);
		~Entity();
		void set_parameter(char*, int);	//Id, value
		int get_parameter(char*);	//Id
		void set_name(char*);
		char* get_name();
		int get_modifier(int);
		int throw(char*, int);	//Id, dice_faces
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
