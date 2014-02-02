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
    
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_BLACK,COLOR_WHITE);
    init_pair(4, COLOR_WHITE,COLOR_BLACK);
    return 0;
}
int draw_table()
{
    attron(COLOR_PAIR(3));
    move(winy,winx);
    int i,j;
    for (i=0;i<41;i++)
    {
        for(j=0;j<27;j++)
        {
            addch('+');
            move(winy+j,winx+i);
        }
    }
    addch('+');
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            draw_cursor_a(i,j);
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
    move(winy+1+6*y,winx+1+10*x);
    addstr("+-------+");
    for(i=0;i<5;i++)
    {
        move(winy+2+i+6*y,1+winx+10*x);
        addstr("|       |");
    }
    move(winy+7+6*y,winx+1+10*x);
    addstr("+-------+");
    move(0,0);
    attroff(COLOR_PAIR(3));
    refresh();
    return 0;
}
int draw_cursor_b(int x, int y)
{
    attron(COLOR_PAIR(4));
    int i=0;
    move(winy+1+6*y,winx+1+10*x);
    addstr("+-------+");
    for(i=0;i<5;i++)
    {
        move(winy+2+i+6*y,1+winx+10*x);
        addstr("|       |");
    }
    move(winy+7+6*y,winx+1+10*x);
    addstr("+-------+");
    move(0,0);
    attroff(COLOR_PAIR(4));
    refresh();
    return 0;
}
int draw_card(struct card_t *card,int x, int y)
{
    attron(COLOR_PAIR(card->eq));
    int i,j;
    for(i=0;i<7;i++)
    {
        for(j=0;j<5;j++)
        {
            move(winy+2+6*y+j,winx+2+10*x+i);
            addch(' ');
        }
    }
    
    move(winy+5+6*y,winx+2+10*x);
    printw(" %X  %X%X",card->stats[0],card->type,card->stats[1],card->stats[2]);
    move(winy+5+6*y,winx+4+10*x);
    switch (card->type)
    {
        case 0: addch('P'); break;
        case 1: addch('M'); break;
        case 2: addch('X'); break;
        case 3: addch('A'); break;
    }
    move(winy+2+6*y,winx+2+10*x);
    if (card->arrows&128)
    {
        addch('\\');
    }
    move(winy+2+6*y,winx+5+10*x);
    if (card->arrows&64)
    {
        addch('|');
    }
    move(winy+2+6*y,winx+8+10*x);
    if (card->arrows&32)
    {
        addch('/');
    }
    move(winy+4+6*y,winx+2+10*x);
    if (card->arrows&16)
    {
        addch('-');
    }
    move(winy+4+6*y,winx+8+10*x);
    if (card->arrows&8)
    {
        addch('-');
    }
    move(winy+6+6*y,winx+2+10*x);
    if (card->arrows&4)
    {
        addch('/');
    }
    move(winy+6+6*y,winx+5+10*x);
    if (card->arrows&2)
    {
        addch('|');
    }
    move(winy+6+6*y,winx+8+10*x);
    if (card->arrows&1)
    {
        addch('\\');
    }
    move(0,0);
    attroff(COLOR_PAIR(card->eq));
    refresh();
    return 0;
}
int redraw(struct state_t *state)
{
    clear();
    draw_table();
    draw_cursor_b(state->x, state->y);
    int i,j;
    for (i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            if(state->table[i][j]>1)
            {
                draw_card(&state->cards1[state->table[i][j]-2], i, j);
            }
        }
    }
    refresh();
    return 0;
}