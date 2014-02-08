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
	state->phase=SELECTION;
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
					redraw(state); game(state,state->x,state->y);
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
		printw("Presiona C para insertar carta, V para cambiar de carta\n");
		redraw(state);
		move(31,10);
		//printw("Carta: %d, %d\n", state->selection/5, state->selection%5);
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

struct game_t game(struct state_t *state,int x,int y)
{
	enum phase_t phase_old=state->phase;
	state->phase=GAME;
	int eq,num;
	tbtocard (state, &eq, &num, x, y);
	int eq_old=eq;
	int i,j,k;
	int r=0;
	for(i=0;i<8;i++)
	// Comprueba si hay cartas enemigas a su alrededor
	{
		state->game.a_arr=i;
		if(((state->cards[eq][num].arrows&1<<i)>>i) & itojkxy(i, &j, &k, x, y))
		//Comprueba cada flecha que tiene la carta y si la casilla adyacente es una casilla jugable
		{
			if (state->table[x+j][y+k]>1)
			//La pongo aquí no vaya a ser que vaya a intentar evaluarla para valores mas grandes de 4 o mas chicos que 0. Comprueba que en la posición adyacente hay una carta.
			{
				int eq_en, num_en, l, m, n;
				int a=0;
				tbtocard(state, &eq_en, &num_en, x+j, y+k);
				
				if (state->cards[eq][num].eq!=state->cards[eq_en][num_en].eq)
				//Continua evaluando si son de diferentes equipos
				{
					state->game.x=x;
					state->game.y=y;
					if (eq==eq_old)
					//Comprueba si el atacante sigue siendo del mismo equipo que al principio
					{
						for(l=0;l<8;l++)
						//comprueba si hay flechas que apunten de la defensora a la carta de ataque
						{
						state->game.d_arr=l;
							if(((state->cards[eq_en][num_en].arrows&1<<l)>>l) & itojk(l, &m, &n))
							//Va comprobando que tiene cada flecha (itoxy es siempre 1)
							{
								if(j+m==0 & k+n==0)
								//Comprueba si las flechas son opuestas y coincidentes
								{
									a++;
									int resul=battle(state, eq, num, eq_en, num_en).resul;
									if (resul==1)
									//Puede que haya que hacer una función aparte para ganar o perder
									{
										state->cards[eq_en][num_en].eq=state->cards[eq][num].eq; r=2;
									}
									else if (resul==0) {
										state->cards[eq][num].eq=state->cards[eq_en][num_en].eq; r=3;
									}
									else if (resul==-1) {
										printf("Error!!"); r=-1;
									}
								}
							}
						}
						if (a==0)
						//En caso de no tener flechas con las que defenderse, la carta se gana automáticamente.
						{
							state->cards[eq_en][num_en].eq=state->cards[eq][num].eq; r=1;
						}
					}
				}
			}
		}
	}
	state->game.resul=r;
	state->phase=phase_old;
	return state->game;
}
struct battle_t battle(struct state_t *state,int eq,int num,int eq_en,int num_en)
{
	enum phase_t phase_old=state->phase;
	state->phase=BATTLE;
	struct card_t att,def;
	att=state->cards[eq][num];
	def=state->cards[eq_en][num_en];
	
	state->battle.cards[0]=att;
	state->battle.cards[1]=def;
	state->battle.type=att.type;
	state->battle.param_att=att.stats[0];
	/*move(20,0);
	printw("Param: %X %X %X %X, EQ: %x",att.stats[0],att.type,att.stats[1],att.stats[2],att.eq);
	move(21,0);
	printw("Param: %X %X %X %X, EQ: %x",def.stats[0],def.type,def.stats[1],def.stats[2],def.eq);*/
	switch (att.type) {
		case P:
			state->battle.param_def=def.stats[1];
			if (att.stats[0]>def.stats[1]) {
				state->battle.resul=1; return state->battle;
			}
			break;
		case M:
			state->battle.param_def=def.stats[2];
			if (att.stats[0]>def.stats[2]) {
				state->battle.resul=1; return state->battle;
			}
			break;
		case X:
			state->battle.param_def=min(def.stats[1], def.stats[2]);
			if (att.stats[0]>min(def.stats[1], def.stats[2])) {
				state->battle.resul=1; return state->battle;
			}
			break;
		case A:
			state->battle.param_def=min(min(def.stats[0],def.stats[1]),def.stats[2]);
			if (att.stats[0]>min(min(def.stats[0], def.stats[1]), def.stats[2])) {
				state->battle.resul=1; return state->battle;
			}
			break;
		default:
			state->battle.resul=-1; return state->battle;
			break;
	}
	state->battle.resul=0;
	redraw(state);
	state->phase=phase_old;
	return state->battle;
}

int min(int a, int b)
{
	if (a>b)
	{
		return b;
	}
	return a;
}