/***************************************************************************************
 main.c 1.0
 (C) 01/02/2014 15:42:56 by MARCOS BRITO, marbri91@gmail.com
 Description: Tetramaster online, primeras pruebas
 **************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>


struct card_t {
    int stats[3];
    enum {P,M,X,A} type;
    int arrows;
    int eq;
};

struct state_t {
    int x,y;
    struct card_t cards1[5];
    struct card_t cards2[5];
    int table[4][4];
};
int manage(struct state_t *state);

int init_drawing();
int draw_table();
int draw_card(struct card_t *card,int x,int y);
int draw_cursor_a(int x, int y);
int draw_cursor_b(int x, int y);
int redraw(struct state_t *state);
int winx=0, winy=0;

int main()
{
    init_drawing();
    srand((int)time(NULL));
    winx=(COLS)/2-21;
    struct state_t state;
    state.x=0;
    state.y=0;
    int i;
    for(i=0;i<4;i++)
    {
        int j;
        for(j=0;j<4;j++)
        {
            state.table[i][j]=0;
        }
    }
    for (i=0;i<5;i++)
    {
        state.cards1[i].arrows=rand()%256;
        state.cards1[i].stats[0]=rand()%16;
        state.cards1[i].stats[1]=rand()%16;
        state.cards1[i].stats[2]=rand()%16;
        state.cards1[i].type=rand()%4;
        state.cards1[i].eq=rand()%2+1;
    }
    redraw(&state);
    manage(&state);
    endwin();
    return 0;
}
int manage(struct state_t *state)
{
    int key = getch();
    int i=0;
    while(key!=0x1B)
    {
        if(key == KEY_RIGHT)
        {
            state->x=(state->x+1)%4;
        }
        if(key == KEY_LEFT)
        {
            state->x=(state->x+7)%4;
        }
        if(key == KEY_UP)
        {
            state->y=(state->y+7)%4;
        }
        if(key == KEY_DOWN)
        {
            state->y=(state->y+1)%4;
        }
        if(key == 0x0A)
        {
            state->table[state->x][state->y]=i+2; //dos estados para vacios
            i=(i+1)%5;  //cinco cartas
        }
        redraw(state);
        key = getch();
    }
    return 0;
}

//-----------------------------DRAWINGS--------------------------------
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