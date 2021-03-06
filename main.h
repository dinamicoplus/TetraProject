//
//  main.h
//  tetramaster
//
//  Created by Marcos Brito Santos on 02/02/14.
//  Copyright (c) 2014 Marcos Brito Santos. All rights reserved.
//

#ifndef tetramaster_main_h
#define tetramaster_main_h

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>

enum type_t {P,M,X,A};
enum phase_t {SELECTION_P, GAME_P, BATTLE_P, COMBI_P};
struct card_t {
    int stats[3];
    enum type_t type;
    int arrows;
    int eq;
	int x,y;
    int played;
};

enum evaluation_e {BATTLE, AUTOWIN, NONE};
enum battle_e {WIN,LOSE};

struct battle_t {
    enum type_t type;
    struct card_t *att,*def;
    int param_att;
    int param_def;
	int combi;
    enum battle_e resul;
};
struct evaluation_t {
    enum evaluation_e resul;
	int a_arr,d_arr;
};

struct state_t {
    int x,y;
    struct card_t cards[2][5];
    int table[4][4];
    int selection;
    int turn;
    enum phase_t phase;
	
	struct battle_t battle;
	struct evaluation_t evaluation;
};

int initgame(struct state_t *state);
int manage(struct state_t *state);
int selection(struct state_t *state);
void autowin(struct state_t *state, struct card_t *att, struct card_t *def);
int insertcard(struct state_t *state, int eq, int num);
int combi(struct state_t *state,struct card_t *card);
int itojk(int i, int *j, int *k);
int itojkxy(int i, int *j, int *k, int x, int y);
int tbtocard (struct state_t *state, int *eq, int *num, int x, int y);
enum evaluation_e evaluation(struct state_t *state,int x,int y,int i);
enum battle_e battle(struct state_t *state, struct card_t *att, struct card_t *def);
int game(struct state_t *state,int x, int y);
int min(int a, int b);

int init_drawing();
int draw_table(int x,int y);
int draw_card(struct card_t *card,int x,int y);
int draw_cursor_a(int x, int y);
int draw_cursor_b(int x, int y,int color);
int draw_arrows(int i,int x, int y, int color);
int redraw(struct state_t *state);
int end_drawing();
int winx, winy;

#define KEY_V 0x76
#define KEY_C 0x63

#endif
