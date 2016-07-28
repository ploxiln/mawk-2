
/********************************************
wait.h
copyright 2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/

#ifndef RE_WAIT_H
#define RE_WAIT_H

#include "rexp.h"

typedef struct rts_node {
    struct rts_node* link ;
    RT_STATE  state ;
} RTS_Node ;

extern RTS_Node* wait_queue ;
extern RTS_Node* wait_free_list ;

void wait_queue_insert(STATE* m, const char* s, int u, const char* ss) ;
void empty_wait_queue(void) ;

#endif





