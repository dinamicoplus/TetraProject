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
int winx, winy;

#endif
