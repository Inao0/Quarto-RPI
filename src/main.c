#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "ncurses.h"

#define RED 0
#define N_CHOICES 5
#define EXIT "EXIT"
#define EXIT_INDEX N_CHOICES+1

char *choices_right[] = { "New local game", "New online game", "Rules", "About", EXIT};

int main()
{	
	WINDOW *menu_win;
	int highlight, input, choice;

	initscr();
	clear(); /* Clear shell */
	noecho(); /* Prevent inputs to be printed */
	cbreak();	/* Buffer has only one input. Signals (ex: ^C) are transmitted. */
	curs_set(0); /* Hide cursor */
	if (has_colors()) { /* Shell supports colors */
		start_color(); /* Allows to use colors */
		use_default_colors(); /* Mandatory to use the right color */
		init_pair(COLOR_RED,     RED,     COLOR_BLACK);
	}

	// Accept user input
	keypad(menu_win, TRUE);

	mvprintw(0, 0, "Use arrow keys to navigate.\nPress ENTER to select a choice.");
	refresh();

	// Displaying choice menu
	wattron(menu_win, A_BOLD); // Bold caracters
	wattron(menu_win, COLOR_PAIR(RED));
	box(menu_win, 0, 0);
	wrefresh(menu_win);

	while(1) {	
		input = wgetch(menu_win); // attends un input utilisateur
		switch(input) {	
			case KEY_UP:
				if(highlight == 1)
					highlight = EXIT_INDEX;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == EXIT_INDEX)
					highlight = 1;
				else 
					++highlight;
				break;
			case 10: // enter
				choice = highlight;
				break;
			default:
				mvprintw(getmaxy(stdscr)-1, 0, "Unauthorized key. You pressed '%c'", input);
				wrefresh(stdscr);
				break;
		}

		if (choice == EXIT_INDEX)	/* On quitte le programme */
			break;
		else if (choice != 0) {
			box(menu_win, 0, 0);
			wrefresh(menu_win);
			choice = 0;
		}
	}
	endwin();
	return 0;
}