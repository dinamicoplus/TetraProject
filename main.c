/***************************************************************************************
 main.c 1.0
 (C) 01/02/2014 15:42:56 by MARCOS BRITO, marbri91@gmail.com
 Description: Tetramaster online, primeras pruebas
 **************************************************************************************/
#include "main.h"

int min(int a, int b);

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
	state->phase=SELECTION_P;
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
			case KEY_C: game(state, state->x, state->y); break;
		}
		redraw(state);
		key = getch();
		
    }
    return 0;
}

int game(struct state_t *state,int x, int y)
{
	state->phase=GAME_P;
	int i=0;
	if(insertcard(state, state->selection/5, state->selection%5))
	{
		redraw(state);
		for(i=0;i<8;i++)
		{
			switch(evaluation(state,state->x,state->y,i)){
				case BATTLE:
					state->phase=BATTLE_P;
					switch(state->battle.resul=battle(state, state->battle.att, state->battle.def)){
						case WIN:
							redraw(state);
							state->battle.def->eq=state->battle.att->eq;
							
							state->phase=COMBI_P;
							combi(state, state->battle.def);
							redraw(state);
							break;
						case LOSE:
							redraw(state);
							state->battle.att->eq=state->battle.def->eq;
							
							state->phase=COMBI_P;
							combi(state, state->battle.att);
							redraw(state);
							break;
					}
					state->phase=GAME_P;
					break;
				case AUTOWIN:
					autowin(state, state->battle.att, state->battle.def);
					break;
				case NONE:
					break;
			}
		}
		if (state->turn%2==0)
		{
			state->turn++; state->selection=9; selection(state);
		}
		else
		{
			state->turn++; state->selection=4; selection(state);
		}
	}
	state->phase=SELECTION_P;
	return 0;
}

int selection(struct state_t *state)
{
	do state->selection=(state->selection+1)%5+5*(state->turn%2);
	while (state->cards[state->selection/5][state->selection%5].played==1
		   &state->turn<11);
	return state->selection;
}
int insertcard(struct state_t *state,int eq,int num)
{
	if(state->table[state->x][state->y]==0&state->cards[eq][num].played==0)
	{
		state->table[state->x][state->y]=num+2+5*eq; //dos estados para vacios
		state->cards[eq][num].played=1;
		state->cards[eq][num].x=state->x;
		state->cards[eq][num].y=state->y;
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

enum evaluation_e evaluation(struct state_t *state,int x, int y,int i)
{
	int eq,num;
	tbtocard (state, &eq, &num, x, y);
	int eq_old=eq;
	int j,k;
	// Comprueba si hay cartas enemigas a su alrededor
	state->evaluation.a_arr=i;
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
				if (eq==eq_old)
				//Comprueba si el atacante sigue siendo del mismo equipo que al principio
				{
					state->battle.att=&state->cards[eq][num];
					state->battle.def=&state->cards[eq_en][num_en];
					for(l=0;l<8;l++)
					//comprueba si hay flechas que apunten de la defensora a la carta de ataque
					{
						state->evaluation.d_arr=l;
						if(((state->cards[eq_en][num_en].arrows&1<<l)>>l) & itojk(l, &m, &n))
						//Va comprobando que tiene cada flecha (itoxy es siempre 1)
						{
							if(j+m==0 & k+n==0)
							//Comprueba si las flechas son opuestas y coincidentes
							{
								a++;
								return BATTLE;
							}
						}
					}
					if (a==0)
					//En caso de no tener flechas con las que defenderse, la carta se gana automáticamente.
					{
						return AUTOWIN;
					}
				}
			}
		}
	}
	return NONE;
}

void autowin(struct state_t *state, struct card_t *att, struct card_t *def)
{
	def->eq=att->eq;
}


enum battle_e battle(struct state_t *state,struct card_t *att, struct card_t *def)
{
	
	/*struct card_t att,def;
	att=state->cards[eq][num];
	def=state->cards[eq_en][num_en];*/
	
	state->battle.att=att;
	state->battle.def=def;
	state->battle.type=att->type;
	state->battle.param_att=att->stats[0];
	/*move(20,0);
	printw("Param: %X %X %X %X, EQ: %x",att->stats[0],att->type,att->stats[1],att->stats[2],att->eq);
	move(21,0);
	printw("Param: %X %X %X %X, EQ: %x",def->stats[0],def->type,def->stats[1],def->stats[2],def->eq);*/
	switch (att->type) {
		case P:
			state->battle.param_def=def->stats[1];
			if (att->stats[0]>def->stats[1]) {
				return WIN;
			}
			break;
		case M:
			state->battle.param_def=def->stats[2];
			if (att->stats[0]>def->stats[2]) {
				return WIN;
			}
			break;
		case X:
			state->battle.param_def=min(def->stats[1], def->stats[2]);
			if (att->stats[0]>min(def->stats[1], def->stats[2])) {
				return WIN;
			}
			break;
		case A:
			state->battle.param_def=min(min(def->stats[0],def->stats[1]),def->stats[2]);
			if (att->stats[0]>min(min(def->stats[0], def->stats[1]), def->stats[2])) {
				return WIN;
			}
			break;
		default:
			return LOSE;
			break;
	}

	return LOSE;
}

int combi(struct state_t *state,struct card_t *card)
{
	int i,j,k;
	state->battle.combi=0;
	for(i=0;i<8;i++)
	// Comprueba si hay cartas enemigas a su alrededor
	{
		//state->game.a_arr=i;
		if(((card->arrows&1<<i)>>i) & itojkxy(i, &j, &k, card->x, card->y))
			//Comprueba cada flecha que tiene la carta y si la casilla adyacente es una casilla jugable
		{
			if (state->table[card->x+j][card->y+k]>1)
				//La pongo aquí no vaya a ser que vaya a intentar evaluarla para valores mas grandes de 4 o mas chicos que 0. Comprueba que en la posición adyacente hay una carta.
			{
				int eq_en, num_en;
				tbtocard(state, &eq_en, &num_en, card->x+j, card->y+k);
				
				if (card->eq!=state->cards[eq_en][num_en].eq)
				//Continua evaluando si son el mismo equipo
				{
					state->cards[eq_en][num_en].eq=card->eq;
					state->battle.combi++;
				}
			}
		}
	}
	return state->battle.combi;
}
int min(int a, int b)
{
	if (a>b)
	{
		return b;
	}
	return a;
}