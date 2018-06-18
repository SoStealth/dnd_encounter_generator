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
#include <stdlib.h>
#include "split.h"

int** get_table(char* filename, int rows, int columns);
void expression(char* s, int* times, int* damage, int* bonus);

/*
Loads a table from a csv file and returns it
*/
int** get_table(char* filename, int rows, int columns) {
	char* line;			//stores a line from the file
	size_t len = 0;		//required by readline() function
	char** temp = NULL;	//stores the splitted file line
	FILE* file = fopen(filename,"r");	//opens tab file
	if (file == NULL) {		//Error exit
        printf("Error: file pointer is null.");
        exit(1);
    }
	int** table = (int**)malloc(sizeof(int)*rows*columns);	//creates space for matrix
	for(int k=0;k<rows;k++) table[k]=(int*)malloc(sizeof(int)*columns);	//creates space for every line
	for(int i=0;i<rows;i++) {	//for every line of the matrix
		getline(&line, &len, file);	//read a line from the file
		split(line,',',&temp);		//split the line by ','
		for(int j=0;j<columns;j++) {	//for every value found in the line
			table[i][j] = atoi(temp[j]);	//store the value in the matrix
		}
		free(temp);	//split() uses malloc()
	}
	fclose(file);	//closes file
	return table;	//returns table
}

void expression(char* s, int* times, int* damage, int* bonus) {
	char** temp;
	int count = split(s,'d',&temp);
	if(count == 1) {
		*times = 0;
		*damage = 0;
		*bonus = atoi(temp[0]);
		return;
	}
	if(strlen(temp[0])==0) {
		*times = 1;
	} else {
		*times = atoi(temp[0]);
	}
	char** temp2;
	char* s2 = strdup(temp[1]);
	count = split(s2,'+',&temp2);
	if(count==2) {
		*damage = atoi(temp2[0]);
		*bonus = atoi(temp2[1]);
		return;
	}
	count = split(s2,'-',&temp2);
	if(count==2) {
		*damage = atoi(temp2[0]);
		*bonus = atoi(temp2[1]);
		*bonus = *bonus * -1;
		return;
	}
	*damage = atoi(temp2[0]);
	*bonus = 0;
	free(s2);
	return;
}
//-------------------------------------------------------------------------------------------
void start_log(char* filepath) {
	FILE* file = fopen(filepath,"w");
	fclose(file);
}
//-------------------------------------------------------------------------------------------
void write_log(char* filepath,char* s) {
	FILE* file = fopen(filepath,"a");
	fputs(s,file);
	fclose(file);
}