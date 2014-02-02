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
	winy=1;
    struct state_t state;
    state.x=0;
    state.y=0;
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            state.table[i][j]=0;
        }
    }
    for (i=0;i<5;i++)
    {
		for(j=0;j<2;j++)
		{
			state.cards[j][i].arrows=rand()%256;
			state.cards[j][i].stats[0]=rand()%16;
			state.cards[j][i].stats[1]=rand()%16;
			state.cards[j][i].stats[2]=rand()%16;
			state.cards[j][i].type=rand()%4;
			state.cards[j][i].eq=j+1;
			state.cards[j][i].played=0;
		}
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
            state->x=(state->x+3)%4;
        }
        if(key == KEY_UP)
        {
            state->y=(state->y+3)%4;
        }
        if(key == KEY_DOWN)
        {
            state->y=(state->y+1)%4;
        }
        if(key == 0x76) //V - Selecciona la carta a jugar
        {
			i=(i+1)%10;
        }
        if(key == 0x63) //C - iserta la carta
        {
			insertcard(state, i/5, i%5);
        }
		redraw(state);
		move(30,10);
		printw("Carta: %d, %d, %d",i+1, i/5, i%5);
		key = getch();
    }
	// printw("%X",key);
    return 0;
}
int insertcard(struct state_t *state,int eq,int num)
{
	if(state->table[state->x][state->y]==0&state->cards[eq][num].played==0)
	{
		state->table[state->x][state->y]=num+2+5*eq; //dos estados para vacios
		state->cards[eq][num].played=1;
	}
	return 0;
}