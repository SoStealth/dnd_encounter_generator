#include "libs/menu.hpp"

int main() {
	bool exit = false;
	initscr();
	noecho();

	init_random();

	while(!exit){
		char c;
		clear();
		addstr(HEADER);
		addstr(MENU_START);
		refresh();

		c = getch();
		switch(c) {
			case '1':
				generator();
			break;
			case '2':
				exit = true;
			break;
			default:

			break;
		}
	}

	endwin();
	return 0;
}