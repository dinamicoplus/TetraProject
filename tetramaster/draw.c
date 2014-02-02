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
int draw_cursor_b(int x, int y)
{
    attron(COLOR_PAIR(4));
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
            move(y+j,x+i);
            addch(' ');
        }
    }
    
    move(3+y,x);
    printw(" %X  %X%X",card->stats[0],card->type,card->stats[1],card->stats[2]);
    move(3+y,2+x);
    switch (card->type)
    {
        case 0: addch('P'); break;
        case 1: addch('M'); break;
        case 2: addch('X'); break;
        case 3: addch('A'); break;
    }
    move(y,x);
    if (card->arrows&128)
    {
        addch('\\');
    }
    move(y,3+x);
    if (card->arrows&64)
    {
        addch('|');
    }
    move(y,6+x);
    if (card->arrows&32)
    {
        addch('/');
    }
    move(2+y,x);
    if (card->arrows&16)
    {
        addch('-');
    }
    move(2+y,6+x);
    if (card->arrows&8)
    {
        addch('-');
    }
    move(4+y,x);
    if (card->arrows&4)
    {
        addch('/');
    }
    move(4+y,3+x);
    if (card->arrows&2)
    {
        addch('|');
    }
    move(4+y,6+x);
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
    draw_table(winx,winy);
    draw_cursor_b(winx+1+10*state->x,winy+1+6*state->y);
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
    refresh();
    return 0;
}