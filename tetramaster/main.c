/***************************************************************************************
 main.c 1.0
 (C) 01/02/2014 15:42:56 by MARCOS BRITO, marbri91@gmail.com
 Description: Tetramaster online, primeras pruebas
 **************************************************************************************/
#include "main.h"

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