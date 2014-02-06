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


struct card_t {
    int stats[3];
    enum {P,M,X,A} type;
    int arrows;
    int eq;
    int played;
};

struct state_t {
    int x,y;
    struct card_t cards[2][5];
    int table[4][4];
};
int initgame(struct state_t *state);
int manage(struct state_t *state);
int insertcard(struct state_t *state, int eq, int num);
int itojk(int i, int *j, int *k);
int itojkxy(int i, int *j, int *k, int x, int y);
int tbtocard (struct state_t *state, int *eq, int *num, int x, int y);
int game(struct state_t *state, int x, int y);
int battle(struct state_t *state,int eq,int num,int eq_en,int num_en);

int init_drawing();
int draw_table(int x,int y);
int draw_card(struct card_t *card,int x,int y);
int draw_cursor_a(int x, int y);
int draw_cursor_b(int x, int y);
int redraw(struct state_t *state);
int end_drawing();
int winx, winy;
int var_heces[8][2];

#define KEY_V 0x76
#define KEY_C 0x63

#endif
