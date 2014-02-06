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
	initgame(&state);
    redraw(&state);
    manage(&state);
	end_drawing();
    return 0;
}
int initgame(struct state_t *state)
{
	state->x=0;
    state->y=0;
    int i,j;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            state->table[i][j]=0;
        }
    }
    for (i=0;i<5;i++)
    {
		for(j=0;j<2;j++)
		{
			state->cards[j][i].arrows=255;
			state->cards[j][i].stats[0]=rand()%16;
			state->cards[j][i].stats[1]=rand()%16;
			state->cards[j][i].stats[2]=rand()%16;
			state->cards[j][i].type=rand()%4;
			state->cards[j][i].eq=j+1;
			state->cards[j][i].played=0;
		}
    }
	return 0;
}
int manage(struct state_t *state)
{
    int key = getch();
    int i=0,j=0;
    while(key!=0x1B)
    {
		switch (key) {
			case KEY_RIGHT:	state->x=(state->x+1)%4; break;
			case KEY_LEFT: state->x=(state->x+3)%4; break;
			case KEY_DOWN: state->y=(state->y+1)%4; break;
			case KEY_UP: state->y=(state->y+3)%4; break;
			case KEY_V: i=(i+1)%10; break;
			case KEY_C: if(insertcard(state, i/5, i%5)) game(state,state->x,state->y); break;
		}
		redraw(state);
		move(30,10);
		printw("Carta: %d, %d\n", i/5, i%5);
		printw("Presiona C para insertar carta\n");
		for(j=0;j<8;j++)
		{
			printw("x:%d, y:%d\n",var_heces[j][0],var_heces[j][1]);
		}
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
		return 1;
	}
	return 0;
}
int itojk(int i, int *j, int *k)
{
	*j = 1 - (i+i/4)%3; //x
	*k = -((i+i/5)/3-1); //y
	
	var_heces[i][0] = *j;
	var_heces[i][1] = *k;
	return 0;
}

int itojkxy(int i, int *j, int *k,int x, int y)
{
	*j = 1 - (i+i/4)%3; //x
	*k = -((i+i/5)/3-1); //y
	
	var_heces[i][0] = *j;
	var_heces[i][1] = *k;
	
	if((y+*k)>=0 & (x+*j)>=0 & (y+*k)<4 & (x+*j)<4)
	{
		return 1;
	}
	return 0;
}

int game(struct state_t *state,int x,int y)
{
	int eq = (state->table[x][y]-2)/5;
	int num = (state->table[x][y]-2)%5;
	int i,j,k;
	for(i=0;i<8;i++)
	{
		if(((state->cards[eq][num].arrows&1<<i)>>i) & itojkxy(i, &j, &k, x, y))
		{
			state->table[x+j][y+k]=10;
		}
	}
	return 0;
}