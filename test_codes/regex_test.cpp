#include "simulation.hpp"
#include <regex.h>


int main() {
	regex_t regex;
	int reti;
	char buffer[4096];

	char* test = ",56";
	char** temp;
	split(test,',',&temp);
	if(temp[1]==NULL)
		printf("Quack\n",temp[0]);
	printf("%s\n",temp[1]);

	reti = regcomp(&regex, test, 0);
	if(reti) {
		printf("Could not compile regex\n");
		exit(1);
	}

	printf("Testing 1d20...\n");
	reti = regexec(&regex, "1d20", 0, NULL, 0);
	if(!reti) {
		printf("It matches!\n");
	} else if(reti == REG_NOMATCH) {
		printf("It does not match\n");
	} else {
		regerror(reti, &regex, buffer, sizeof(buffer));
		printf("Regex match failed: %s\n",buffer);
		exit(1);
	}

	printf("Testing 1ad40...\n");
	reti = regexec(&regex, "1ad40", 0, NULL, 0);
	if(!reti) {
		printf("It matches!\n");
	} else if(reti == REG_NOMATCH) {
		printf("It does not match\n");
	} else {
		regerror(reti, &regex, buffer, sizeof(buffer));
		printf("Regex match failed: %s\n",buffer);
		exit(1);
	}

	printf("Testing d20...\n");
	reti = regexec(&regex, "d20", 0, NULL, 0);
	if(!reti) {
		printf("It matches!\n");
	} else if(reti == REG_NOMATCH) {
		printf("It does not match\n");
	} else {
		regerror(reti, &regex, buffer, sizeof(buffer));
		printf("Regex match failed: %s\n",buffer);
		exit(1);
	}

	printf("Testing 2d20+5...\n");
	reti = regexec(&regex, "2d20+5", 0, NULL, 0);
	if(!reti) {
		printf("It matches!\n");
	} else if(reti == REG_NOMATCH) {
		printf("It does not match\n");
	} else {
		regerror(reti, &regex, buffer, sizeof(buffer));
		printf("Regex match failed: %s\n",buffer);
		exit(1);
	}

	printf("Testing 4d5+...\n");
	reti = regexec(&regex, "4d5+", 0, NULL, 0);
	if(!reti) {
		printf("It matches!\n");
	} else if(reti == REG_NOMATCH) {
		printf("It does not match\n");
	} else {
		regerror(reti, &regex, buffer, sizeof(buffer));
		printf("Regex match failed: %s\n",buffer);
		exit(1);
	}

	printf("Testing 4d20+r...\n");
	reti = regexec(&regex, "4d20+r", 0, NULL, 0);
	if(!reti) {
		printf("It matches!\n");
	} else if(reti == REG_NOMATCH) {
		printf("It does not match\n");
	} else {
		regerror(reti, &regex, buffer, sizeof(buffer));
		printf("Regex match failed: %s\n",buffer);
		exit(1);
	}

	printf("Testing 7d105...\n");
	reti = regexec(&regex, "7d105", 0, NULL, 0);
	if(!reti) {
		printf("It matches!\n");
	} else if(reti == REG_NOMATCH) {
		printf("It does not match\n");
	} else {
		regerror(reti, &regex, buffer, sizeof(buffer));
		printf("Regex match failed: %s\n",buffer);
		exit(1);
	}
	return 0;
}

