#include "functions.h"
#include <stdio.h>

int main() {
	printf("Testing get_table function\n");
	printf("Creating a table to contain data...\n");
	int rows = 20;
	int columns = 10;
	int** table;
	printf("Loading table from file...\n");
	table = get_table("tabs/bard_spellslots.csv",rows,7);
	printf("Printing out table:\n\n");
	for(int i=0;i<rows;i++) {
		for(int j=0;j<columns;j++) {
			printf("%d,",table[i][j]);
		}
		printf("\n");
	}
	printf("End\n");

	printf("\nTesting out expressions...\n");
	char* test = "7d7";
	int times,damage,bonus;
	expression(test,&times,&damage,&bonus);
	printf("%s expression returns:\ntimes = %d\ndamage = %d\nbonus = %d\n",test,times,damage,bonus);
}