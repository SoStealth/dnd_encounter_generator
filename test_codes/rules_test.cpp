#include "rules.hpp"
#include <stdio.h>

int main(int argc, char* argv[]) {
	init_random();
	printf("Here's a dice: D%d\n",D100);
	printf("Rolling the dice 5 times...\n");
	for(int i=0;i<5;i++) {
		printf("%d\n",throw_dice(D100));
	}
	printf("Here's the modifier for a 17 stat: %d\n",get_modifier(17));
	printf("Here's the modifier for a 6 stat: %d\n",get_modifier(6));
	printf("Here's the modifier for a 10 stat: %d\n",get_modifier(10));
	printf("End\n");
}