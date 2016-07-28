
/********************************************
rexp1.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/*  re machine	operations  */

#include  "rexp.h"


#define is_MU(mp) ((mp)->start->type == M_U && (mp)->stop - (mp)->start == 1)

/* of the form A? */
static Bool is_Q(MACHINE* mp)
{
    if (mp->start->type == M_2JA) {
        STATE* p = mp->start + mp->start->data.jump ;
	return p->type == M_ACCEPT ;
    }
    return 0 ;
}

/* convert A? to A, caller has verified is_Q(mp) */
static void remove_Q(MACHINE* mp)
{
    unsigned sz = mp->stop - mp->start + 1 ;
    memmove(mp->start, mp->start + 1, (sz-1) * sizeof(STATE)) ;
    mp->stop-- ;
}


/* no jumps
   or has an M_WAIT at the end */
static Bool is_simple(MACHINE* mp)
{
    if ((mp->stop - 1)->type == M_WAIT) return 1 ;
    else {
	STATE* p = mp->start ;
	while(p->type != M_ACCEPT) {
	    if (p->type == M_2JA || p->type == M_2JB) {
		return 0 ;
	    }
	    p++ ;
	}
	return 1 ;
    }
}


/* construct a two state machine of type at *mp */

static void new_TWO(int type, MACHINE* mp)
{
   mp->start = (STATE *) RE_malloc(2 * STATESZ) ;
   mp->stop = mp->start + 1 ;
   mp->start->type = type ;
   mp->stop->type = M_ACCEPT ;
}

/*  build a machine that recognizes any	 */
MACHINE
RE_any()
{
   MACHINE x ;

   new_TWO(M_ANY, &x) ;
   return x ;
}

/*  build a machine that recognizes the start of string	 */
MACHINE
RE_start()
{
   MACHINE x ;

   new_TWO(M_START, &x) ;
   return x ;
}

MACHINE
RE_end()
{
   MACHINE x ;

   new_TWO(M_END, &x) ;
   return x ;
}

/*  build a machine that recognizes a class  */
MACHINE
RE_class(const BV* bvp)
{
   MACHINE x ;

   new_TWO(M_CLASS, &x) ;
   x.start->data.bvp = bvp ;
   return x ;
}

MACHINE
RE_str(char* str, size_t len)
{
   MACHINE x ;

   new_TWO(M_STR, &x) ;
   x.start->len = len ;
   x.start->data.str = str ;
   return x ;
}


/*  replace m and n by a machine that recognizes  mn

    .* N? --> .*
    M?.*  --> .*

    to avoid epsilon loop:
    M?N?  -->  (MN|M|N)?

    if M has multiple paths and N has multiple paths
    M_WAIT goes on the end
*/

static void RE_catQQ(MACHINE*,MACHINE*) ;

/* $ only */
#define is_END(np) ((np)->stop-(np)->start == 1 && (np)->start->type==M_END)
static Bool is_endable(MACHINE*) ;

void
RE_cat(MACHINE* mp, MACHINE* np)
{
    if (is_MU(mp) && is_Q(np)) {
        free(np->start) ;
    }
    else if (is_MU(np) && is_Q(mp)) {
        free(mp->start) ;
	*mp = *np ;
    }
    else if (is_Q(mp) && is_Q(np)) {
	/* M?N? */
        RE_catQQ(mp,np) ;
    }
    else if (is_END(np) && is_endable(mp)) {
       /* tack $ onto end of mp */
       mp->stop[-1].type += END_ON ;
       free(np->start) ;
    }
    else {
        unsigned sz1, sz2, sz ;
	Bool not_simple = ! is_simple(mp) && ! is_simple(np) ;

        sz1 = mp->stop - mp->start ;   /* size minus M_ACCEPT at end */
        sz2 = np->stop - np->start ;   /* ditto */
        sz = sz1 + sz2 + 1 + not_simple ;
	           /* one for M_ACCEPT and possible M_WAIT */

        mp->start = (STATE *) RE_realloc(mp->start, sz * STATESZ) ;
        mp->stop = mp->start + (sz - 1) ;
        memcpy(mp->start + sz1, np->start, sz2 * STATESZ) ;
        free(np->start) ;
	if (not_simple) {
	    (mp->stop - 1)->type = M_WAIT ;
	}
	mp->stop->type  = M_ACCEPT ;
    }
}
/* shallow copy */
static void machine_copy(MACHINE* target, MACHINE* src)
{
     unsigned sz = src->stop - src->start + 1 ;
     target->start = (STATE *)RE_malloc(sz * STATESZ) ;
     memcpy(target->start, src->start, sz * STATESZ) ;
     target->stop = target->start + (sz-1) ;
}

/*    M?N? -->  (MN|M|N)?   */
void RE_catQQ(MACHINE* mp, MACHINE* np)
{
    MACHINE m_copy ;
    MACHINE n_copy ;

    remove_Q(mp) ;
    remove_Q(np) ;
    machine_copy(&m_copy,mp) ;
    machine_copy(&n_copy,np) ;

    RE_cat(mp,np) ;
    if (mp->stop[-1].type == M_WAIT) {
        /* remove it */
	mp->stop-- ;
	mp->stop->type = M_ACCEPT ;
    }
    RE_or(mp,&m_copy) ;
    RE_or(mp,&n_copy) ;

    /* add an M_WAIT and ?  */
    {
         unsigned nsz = mp->stop - mp->start + 3 ;
	 STATE* p = (STATE *)RE_malloc(nsz * STATESZ) ;
	 memcpy(p+1, mp->start, (nsz - 2)*STATESZ) ;
	 free(mp->start) ;
	 mp->start = p ;
	 mp->stop = p + nsz - 1 ;
	 p->type = M_2JA ;
	 p->data.jump = nsz-1 ;
	 mp->stop->type = M_ACCEPT ;
	 mp->stop[-1].type = M_WAIT ;
    }
}

/* can tack $ on end */
Bool is_endable(MACHINE* mp)
{
    switch(mp->stop[-1].type) {
        case M_STR:
	case M_CLASS:
	case M_ANY:
	case M_START:
	    break ;
	default:
	    return 0 ;
    }
    /* stop[-1] is of right type, next check if can jump over it */
    {
        STATE* p = mp->start ;
	while(p != mp->stop - 1) {
	    switch(p->type) {
	        case M_1J:
		case M_2JA:
		case M_2JB:
		    if (p + p->data.jump == mp->stop) return 0 ;
		    break ;
		default:
		    break ;
	    }
	    p++ ;
	}
	return 1 ;
    }
}

 /*  replace m by a machine that recognizes m|n

     to avoid epsilon loops:
     A?|B  --> (A|B)?
     A|B?  --> (A|B)?
     A?|B?  --> (A|B)?
     .*|A -->  .*
     A|.* -->  .*

*/
void
RE_or(MACHINE* mp, MACHINE* np)
{
   STATE *p ;
   STATE *top ;
   unsigned szm, szn ;
   unsigned totsz ;
   Bool q_flag = 0 ;

   if (is_MU(mp)) {
       /* MU|n --> MU */
       free(np->start) ;
       return ;
   }
   if (is_MU(np)) {
       free(mp->start) ;
       *mp = *np ;
       return ;
   }

   if (is_Q(mp)) {
       q_flag = 1 ;
       remove_Q(mp) ;
   }
   if (is_Q(np)) {
       q_flag = 1 ;
       remove_Q(np) ;
   }

   szm = mp->stop - mp->start + 1 ;
   szn = np->stop - np->start + 1 ;
   totsz = szm + szn + 1 + q_flag ;

   top = (STATE *) RE_malloc(totsz * STATESZ) ;
   p = top + q_flag ;
   memcpy(p + 1, mp->start, szm * STATESZ) ;  /* copies m */
   free(mp->start) ;
   mp->start = top ;
   mp->stop = top + totsz - 1 ;
   mp->stop->type = M_ACCEPT ;
   p->type = M_2JA ;
   p->data.jump = szm + 1 ;
   memcpy(p + szm + 1, np->start, szn * STATESZ) ;
   free(np->start) ;
   (p += szm)->type = M_1J ;
   p->data.jump = szn ;
   if (q_flag) {
       top->type = M_2JA ;
       top->data.jump = totsz - 1 ;
   }
}

/*  UNARY  OPERATIONS	  */



/*  replace m by m*

    to avoid epsilon loops:
           .** --> .*
	   m?* --> m*

*/


void
RE_close(MACHINE* mp)
{
    if (mp->start->type == M_ANY && mp->stop - mp->start == 1) {
        /* closure of M_ANY is M_U */
        mp->start->type = M_U ;
    }
    else if (mp->start->type == M_U && mp->stop - mp->start == 1) {
        /* closure of M_U is M_U */
	/* this would only happen if someone is trying to break us */
	/* do nothing */
    }
    else {
       STATE *p ;
       unsigned sz ;
       Bool not_simple ;

       if (is_Q(mp)) {
           /* highly unlikely but possible */
	   remove_Q(mp) ;
       }

       not_simple = !is_simple(mp) ;

       sz = mp->stop - mp->start + 1 + not_simple ;
       p = (STATE *) RE_malloc((sz + 2) * STATESZ) ;
       memcpy(p + 1, mp->start, sz * STATESZ) ;
       free(mp->start) ;
       mp->start = p ;
       mp->stop = p + (sz + 1) ;
       p->type = M_2JA ;
       p->data.jump = sz + 1 ;
       (p += sz)->type = M_2JB ;
       p->data.jump = -(sz - 1) ;
       (p + 1)->type = M_ACCEPT ;
       if (not_simple) {
           (p-1)->type = M_WAIT ;
       }
    }
}

/*  replace m  by  m+  (positive closure)   */

void
RE_poscl( MACHINE *mp)
{
    STATE* p ;

    if (mp->start->type == M_ANY && mp->stop - mp->start == 1) {
        /* .+ to ..* */
        p = mp->start = (STATE*) RE_realloc(mp->start, 3 * STATESZ) ;
	mp->stop = p+2 ;
	(p+1)->type = M_U ;
	(p+2)->type = M_ACCEPT ;
    }
    else if (mp->start->type == M_U && mp->stop - mp->start == 1) {
        /* pos closure of M_U is M_U */
	/* this would only happen if someone is trying to break us */
	/* do nothing */
    }
    else if (is_Q(mp)) {
        /* A?+ --> A?* */
	RE_close(mp) ;
    }
    else {
	unsigned sz ;
	Bool not_simple = !is_simple(mp) ;

	sz = mp->stop - mp->start + 1 + not_simple ;
	mp->start = p = (STATE *) RE_realloc(mp->start, (sz + 1) * STATESZ) ;
	p += sz  ;
	mp->stop = p ;
	p->type = M_ACCEPT ;
	p-- ;
	p->type = M_2JB ;
	p->data.jump = mp->start - p ;
	if (not_simple) {
	    (p - 1)->type = M_WAIT ;
	}
    }
}

/* replace  m  by  m? (zero or one)
   .*?  -->  .*
   A??  -->  A?
*/

void
RE_01(MACHINE* mp)
{
    if (is_MU(mp) || is_Q(mp)) {
        /* .*?  is .*  */
	/* A?? is A? */
	/* this would only happen if someone is trying to break us */
	/* do nothing */
    }
    else {
	unsigned sz = mp->stop - mp->start + 1 ;
	STATE* p = (STATE *) RE_malloc((sz + 1) * STATESZ);
	memcpy(p + 1, mp->start, sz * STATESZ);
	free(mp->start);
	mp->start = p;
	mp->stop = p + sz;
	p->type = M_2JA;
	p->data.jump = sz ;
    }
}

/* this is a hook for mawk's split, was the machine generated by 
   a regular expression of "".
*/

int REempty(PTR p)
{
    STATE* s = (STATE*) p ;
    return s->type == M_ACCEPT ;
}

/*===================================
MEMORY	ALLOCATION
 *==============================*/


PTR
RE_malloc(size_t sz)
{
   PTR ret = malloc(sz) ;
   if (!ret) RE_error_trap(MEMORY_FAILURE) ;
   return ret ;
}

PTR
RE_realloc(void* p, size_t sz)
{
    PTR ret = realloc(p,sz) ;
    if (!ret) RE_error_trap(MEMORY_FAILURE) ;
    return ret ;
}
