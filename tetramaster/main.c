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
	state->turn=rand()%2;
	state->selection=5*state->turn;
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
			state->cards[j][i].arrows=rand()%256;
			state->cards[j][i].stats[0]=rand()%16;
			state->cards[j][i].stats[1]=rand()%16;
			state->cards[j][i].stats[2]=rand()%16;
			state->cards[j][i].type=rand()%4;
			state->cards[j][i].eq=j;
			state->cards[j][i].played=0;
		}
    }
	return 0;
}
int manage(struct state_t *state)
{
    int key = getch();
    while(key!=0x1B)
    {
		switch (key) {
			case KEY_RIGHT:	state->x=(state->x+1)%4; break;
			case KEY_LEFT: state->x=(state->x+3)%4; break;
			case KEY_DOWN: state->y=(state->y+1)%4; break;
			case KEY_UP: state->y=(state->y+3)%4; break;
			case KEY_V: selection(state); break;
			case KEY_C:
				if(insertcard(state, state->selection/5, state->selection%5))
				{
					redraw(state); if(game(state,state->x,state->y)>1) getch();
					if (state->turn==0)
					{
						state->turn=1; state->selection=9; selection(state);
					}
					else
					{
						state->turn=0; state->selection=4; selection(state);
					}
				} break;
		}
		redraw(state);
		move(31,10);
		//printw("Carta: %d, %d\n", state->selection/5, state->selection%5);
		printw("Presiona C para insertar carta, V para cambiar de carta\n");
		key = getch();
		
    }
	// printw("%X",key);
    return 0;
}
int selection(struct state_t *state)
{
	do state->selection=(state->selection+1)%5+5*state->turn; while (state->cards[state->selection/5][state->selection%5].played==1);
	return state->selection;
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

	return 1;
}

int itojkxy(int i, int *j, int *k,int x, int y)
{
	*j = 1 - (i+i/4)%3; //x
	*k = -((i+i/5)/3-1); //y
	
	if((y+*k)>=0 & (x+*j)>=0 & (y+*k)<4 & (x+*j)<4)
	{
		return 1;
	}
	return 0;
}
int tbtocard (struct state_t *state, int *eq, int *num, int x, int y)
{
	*eq = (state->table[x][y]-2)/5;
	*num = (state->table[x][y]-2)%5;
	return 0;
}

int game(struct state_t *state,int x,int y)
{
	int eq,num;
	tbtocard (state, &eq, &num, x, y);
	int eq_old=eq;
	int i,j,k;
	int r=0;
	for(i=0;i<8;i++) // Comprueba si hay cartas enemigas a su alrededor
	{
		if(((state->cards[eq][num].arrows&1<<i)>>i) & itojkxy(i, &j, &k, x, y)) //Comprueba cada flecha que tiene la carta y si la casilla adyacente es una casilla jugable
		{
			if (state->table[x+j][y+k]>1) //La pongo aquí no vaya a ser que vaya a intentar evaluarla para valores mas grandes de 4 o mas chicos que 0. Comprueba que en la posición adyacente hay una carta.
			{
				int eq_en, num_en, l, m, n;
				int a=0;
				tbtocard(state, &eq_en, &num_en, x+j, y+k);
				
				/*move(20,0);
				printw("Param: %X %X %X %X, EQ:%X",state->cards[eq][num].stats[0],state->cards[eq][num].type,state->cards[eq][num].stats[1],state->cards[eq][num].stats[2],state->cards[eq][num].eq);
				move(21,0);
				printw("Param: %X %X %X %X, EQ:%X",state->cards[eq_en][num_en].stats[0],state->cards[eq_en][num_en].type,state->cards[eq_en][num_en].stats[1],state->cards[eq_en][num_en].stats[2],state->cards[eq_en][num_en].eq);
				getch();*/
				
				if (state->cards[eq][num].eq!=state->cards[eq_en][num_en].eq) //Continua evaluando si son de diferentes equipos
				{
					//int eq_old=eq;
					for(l=0;l<8;l++) //comprueba si hay flechas que apunten de la defensora a la carta de ataque
					{
						if (eq==eq_old)
						{

							if(((state->cards[eq_en][num_en].arrows&1<<l)>>l) & itojk(l, &m, &n)) //Va comprobando que tiene cada flecha (itoxy es siempre	1)
							{
								if(j+m==0 & k+n==0)	//Comprueba si las flechas son opuestas y coincidentes
								{
									a++;
									redraw(state);
									draw_arrows(i, winx+2+10*x, winy+2+6*y);
									int resul=battle(state, eq, num, eq_en, num_en);
									move(21,10);
								
									if (resul==1) {		//Puede que haya que hacer una función aparte para ganar o perder
										state->cards[eq_en][num_en].eq=state->cards[eq][num].eq; r=2;
									}
									else if (resul==0) {
										state->cards[eq][num].eq=state->cards[eq_en][num_en].eq; r=3;
									}
									else if (resul==-1) {
										printw("Error!!"); r=-1;
									}
								}
							}
							if (a==0) {	//En caso de no tener flechas con las que defenderse, la carta se gana automáticamente.
								state->cards[eq_en][num_en].eq=state->cards[eq][num].eq; r=1;
							}
						}
					}
				}
			}
		}
	}
	return r;
}
int battle(struct state_t *state,int eq,int num,int eq_en,int num_en)
{
	struct card_t att,def;
	att=state->cards[eq][num];
	def=state->cards[eq_en][num_en];
	/*move(20,0);
	printw("Param: %X %X %X %X",att.stats[0],att.type,att.stats[1],att.stats[2]);
	move(21,0);
	printw("Param: %X %X %X %X",def.stats[0],def.type,def.stats[1],def.stats[2]);*/
	move(20,10);
	switch (att.type) {
		case P:
			if (att.stats[0]>def.stats[1]) {
				printw("Battalla fisica: %X es mayor que %X",att.stats[0],def.stats[1]);
				return 1;
			}
			else printw("Batalla fisica: %X es menor que %X",att.stats[0],def.stats[1]);
			break;
		case M:
			if (att.stats[0]>def.stats[2]) {
				printw("Batalla magica: %X es mayor que %X",att.stats[0],def.stats[2]);
				return 1;
			}
			else printw("Batalla magica: %X es menor que %X",att.stats[0],def.stats[2]);
			break;
		case X:
			if (att.stats[0]>min(def.stats[1], def.stats[2])) {
				printw("Batalla X: %X es mayor que %X",att.stats[0],min(def.stats[1], def.stats[2]));
				return 1;
			}
			else printw("Batalla X: %X es menor que %X",att.stats[0],min(def.stats[1], def.stats[2]));
			break;
		case A:
			if (att.stats[0]>min(min(def.stats[0], def.stats[1]), def.stats[2])) {
				printw("Batalla Alfa: %X es mayor que %X",att.stats[0], min(min(def.stats[0],def.stats[1]),def.stats[2]));
				return 1;
			}
			else printw("Batalla Alfa: %X es menor que %X",att.stats[0], min(min(def.stats[0],def.stats[1]),def.stats[2]));
			break;
		default:
			return -1;
			break;
	}
	return 0;
}

int min(int a, int b)
{
	if (a>b)
	{
		return b;
	}
	return a;
}