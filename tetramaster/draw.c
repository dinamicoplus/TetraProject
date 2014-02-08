//
//  draw.c
//  tetramaster
//
//  Created by Marcos Brito Santos on 02/02/14.
//  Copyright (c) 2014 Marcos Brito Santos. All rights reserved.
//

#include "main.h"


int init_drawing()
{
    initscr();
    keypad(stdscr, TRUE);
    (void)noecho();
    start_color();
    
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_BLACK,COLOR_WHITE);
    init_pair(4, COLOR_WHITE,COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_CYAN);
    init_pair(6, COLOR_WHITE, COLOR_CYAN);
    return 0;
}
int draw_table(int x,int y)
{
    attron(COLOR_PAIR(3));
    move(y,x);
    int i,j;
    for (i=0;i<41;i++)
    {
        for(j=0;j<27;j++)
        {
            addch('+');
            move(y+j,x+i);
        }
    }
    addch('+');
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            draw_cursor_a(x+1+10*i,y+1+6*j);
        }
    }
    attroff(COLOR_PAIR(3));
    refresh();
    return 0;
}
int draw_cursor_a(int x, int y)
{
    attron(COLOR_PAIR(3));
    int i=0;
    move(y,x);
    addstr("+-------+");
    for(i=0;i<5;i++)
    {
        move(1+y+i,x);
        addstr("|       |");
    }
    move(6+y,x);
    addstr("+-------+");
    move(0,0);
    attroff(COLOR_PAIR(3));
    refresh();
    return 0;
}
int draw_cursor_b(int x, int y,int color)
{
    attron(COLOR_PAIR(color));
    int i=0;
    move(y,x);
    addstr("+-------+");
    for(i=0;i<5;i++)
    {
        move(1+i+y,x);
        addstr("|       |");
    }
    move(6+y,x);
    addstr("+-------+");
    move(0,0);
    attroff(COLOR_PAIR(color));
    refresh();
    return 0;
}
int draw_card(struct card_t *card,int x, int y)
{
    attron(COLOR_PAIR(card->eq+1));
    int i,j;
    for(i=0;i<7;i++)
    {
        for(j=0;j<5;j++)
        {
            move(y+j,x+i);
            addch(' ');
        }
    }
    
    move(3+y,x);
    printw(" %X  %X%X",card->stats[0],card->stats[1],card->stats[2]);
    move(3+y,2+x);
    switch (card->type)
    {
        case P: addch('P'); break;
        case M: addch('M'); break;
        case X: addch('X'); break;
        case A: addch('A'); break;
    }
    for (i=0; i<8; i++) {
		if(card->arrows&(1<<i))
		{
			draw_arrows(i, x, y, card->eq+1);
		}
    }
    move(0,0);
    attroff(COLOR_PAIR(card->eq+1));
    refresh();
    return 0;
}

int draw_arrows(int i,int x, int y, int color)
{
    char arrows[]="\\|/--/|\\";
    int j=(((6*(7-i)/5)/3)*2);
    int k=(((5*(7-i)/4)%3)*3);
    attron(COLOR_PAIR(color));
    move(j+y,k+x);
    addch(arrows[i]);
    move(0,0);
    attroff(COLOR_PAIR(color));
    refresh();
    return 0;
}
int redraw(struct state_t *state)
{
	
	if (state->phase==SELECTION)
	{
		clear();
		draw_table(winx,winy);
		draw_cursor_b(winx+1+10*state->x,winy+1+6*state->y,4);
		draw_cursor_b(winx+44-(44+12)*(state->selection/5)-1,winy+6*(state->selection%5)-1,6);
		int i,j;
		for (i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				if(state->table[i][j]>1)
				{
					draw_card(&state->cards[0][state->table[i][j]-2],winx+2+10*i,winy+2+6*j);
				}
				if(state->table[i][j]>6)
				{
					draw_card(&state->cards[1][state->table[i][j]-7],winx+2+10*i,winy+2+6*j);
				}
			}
		}
		for (i=0;i<5;i++)
		{
			if(state->cards[0][i].played==0) {draw_card(&state->cards[0][i],winx+44,winy+6*i);}
			if(state->cards[1][i].played==0) {draw_card(&state->cards[1][i],winx-12,winy+6*i);}
		}
	}
	if (state->phase==BATTLE)
	{
		draw_arrows(state->game.a_arr, winx+2+10*state->game.x, winy+2+6*state->game.y, 5);
		move(20,10);
		printw("Batalla ");
		switch (state->battle.cards[0].type) {
			case P:
				printw("física: ");
				break;
			case M:
				printw("mágica: ");
				break;
			case X:
				printw("X: ");
				break;
			case A:
				printw("Alfa: ");
				break;
			default:
				break;
		}
		printw("%X ",state->battle.param_att);
		if (state->battle.resul==1) {
			printw("es mayor que ");
		} else {
			printw("es menor que ");
		}
		printw("%X",state->battle.param_def);
		getch();
		draw_card(&state->cards[0][state->table[state->game.x][state->game.y]-2],winx+2+10*state->game.x, winy+2+6*state->game.y);
	}
	refresh();
	return 0;
}
int end_drawing()
{
    endwin();
    return 0;
}