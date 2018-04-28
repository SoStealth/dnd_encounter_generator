/*
Author: Lorenzo Damiano, aka Zeren
Github: https://github.com/SoStealth
*/
//----------------------------------------------------------------------------------------------------------------------------------
/*
Functions
This library contains functions that will be of use in the code.
*/

#include <stdio.h>
#include <string.h>

void get_table(FILE* file, int rows, int columns, int table[rows][columns]) {
	char* line;
	size_t len = 0;
	for(int i=0;i<rows;i++) {
		getline(&line, &len, file);
		char* temp;
		temp = strtok(line,",");
		for(int j=0;j<columns;j++) {
			table[i][j] = atoi(temp);
			temp = strtok(NULL,",");
		}
	}
	return;
}
