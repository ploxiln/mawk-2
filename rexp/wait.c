
/********************************************
wait.c
copyright 2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


#include "wait.h"

RTS_Node* wait_queue ;
RTS_Node* wait_free_list ;

RTS_Node* rts_node_malloc(RT_STATE rts)
{
    RTS_Node* ret ;
    if (wait_free_list) {
        ret = wait_free_list ;
        wait_free_list = ret->link ;
    }
    else {
        ret = (RTS_Node*) RE_malloc(sizeof(RTS_Node)) ;
    }
    ret->state = rts ;
    ret->link = 0 ;
    return ret ;
}

/* move the wait_queue to the wait_free_list */
void empty_wait_queue()
{
    RTS_Node* p = wait_queue ;
    wait_queue = 0 ;
    if (wait_free_list == 0) wait_free_list = p ;
    else { /* find the tail and append */
        RTS_Node* q = wait_free_list ;
	while(q->link) q = q->link ;
	q->link = p ;
    }
}

int rt_state_lt(RT_STATE* r1, RT_STATE* r2) {
    if (r1->s < r2->s) return 1 ;
    if (r1->s > r2->s) return 0 ;
    if (r1->m < r2->m) return 1 ;
    if (r1->m > r2->m) return 0 ;
    if (r1->u < r2->u) return 1 ;
    if (r1->u > r2->u) return 0 ;
    return r1->ss < r2->ss ;
}

/* wait queue is priority queue on ordering rt_state_lt()
   Currently is ordered singley linked list.  Appears it
   doesn't get big enough require a binary tree implementation
*/
void wait_queue_insert(STATE* m, const char* s, int u, const char* ss)
{

    RT_STATE  rts ;
    RTS_Node* p = wait_queue ;
    RTS_Node* q = 0 ;

    rts.m = m ;
    rts.s = s ;
    rts.u = u ;
    rts.ss = ss ;

    while(p) {
        if (rt_state_lt(&rts, &p->state)) {
	    /* insert in front of p */
	    RTS_Node* x = rts_node_malloc(rts) ;
	    x->link = p ;
	    if (q == 0) {
	        wait_queue = x ;
	    }
	    else {
	       q->link = x ;
	    }
	    return ;
	}
	else if (rts.m == p->state.m &&
	         rts.s == p->state.s &&
		 rts.u == p->state.u &&
		 rts.ss == p->state.ss) {
	    /* already in the list */
	    return ;
	}
	q = p ;
	p = p->link ;
    }
    /* add to tail of list */
    p  = rts_node_malloc(rts) ;
    if (wait_queue == 0) {
        wait_queue = p ;
    }
    else q->link = p ;
}

