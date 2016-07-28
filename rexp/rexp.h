
/********************************************
rexp.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


#ifndef  REXP_H
#define  REXP_H


typedef void* PTR ;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include  <setjmp.h>

typedef int Bool ;
PTR   REcompile(const char *,size_t)  ;
int   REtest (const char *, size_t len, PTR)  ;
char *REmatch(const char *, size_t, PTR, size_t*, Bool)  ;
void  REmprint(PTR , FILE*) ;
int   REempty(PTR) ;

extern  int  REerrno ;
extern  const char* const REerrlist[] ;



PTR  RE_malloc(size_t) ;
PTR  RE_realloc(void *,size_t) ;


/*  finite machine  state types  */

#define  M_STR     	0
#define  M_CLASS   	1
#define  M_ANY     	2
#define  M_START   	3
#define  M_END     	4
#define  M_U       	5
#define  M_1J      	6
#define  M_2JA     	7
#define  M_2JB     	8
#define  M_WAIT         9
#define  M_ACCEPT  	10
#define  U_ON      	11

#define  U_OFF     0
#define  END_OFF   0
#define  END_ON    (2*U_ON)


typedef  uint32_t BV[8] ;  /* bit vector */

typedef  struct
{ int type ;
  size_t len ;  /* used for M_STR  */
  union
   {
     char *str  ;  /* string */
     const BV* bvp ;   /*  class  */
     int   jump ;
   }  data ;
}     STATE  ;

#define  STATESZ  (sizeof(STATE))

typedef  struct
{ STATE  *start, *stop ; }   MACHINE ;


/*  tokens   */
#define  T_OR   1       /* | */
#define  T_CAT  2
#define  T_STAR 3       /* * */
#define  T_PLUS 4       /* + */
#define  T_Q    5       /* ? */
#define  T_LP   6       /* ( */
#define  T_RP   7       /* ) */
#define  T_START 8      /* ^ */
#define  T_END  9       /* $ */
#define  T_ANY  10      /* . */
#define  T_CLASS 11     /* starts with [ */
#define  T_SLASH 12     /*  \  */
#define  T_CHAR  13     /* all the rest */
#define  T_STR   14
#define  T_U     15

/*  precedences and error codes  */
#define  L   0
#define  EQ  1
#define  G   2
#define  E1  (-1)
#define  E2  (-2)
#define  E3  (-3)
#define  E4  (-4)
#define  E5  (-5)
#define  E6  (-6)
#define  E7  (-7)

#define  MEMORY_FAILURE      5

#define  ISON(b,x)  ((b)[((unsigned char)(x))>>5] & (1<<((x)&31)))

/* struct for the run time stack */
typedef struct {
STATE *m ;   /*   save the machine ptr */
int    u ;   /*   save the u_flag */
const char  *s ;   /*   save the active string ptr */
const char  *ss ;  /*   save the match start -- only used by REmatch */
} RT_STATE ;   /* run time state */

/*  error  trap   */
extern int REerrno ;
void   RE_error_trap(int) ;


MACHINE    RE_u(void) ;
MACHINE    RE_start(void) ;
MACHINE    RE_end(void) ;
MACHINE    RE_any(void) ;
MACHINE    RE_str(char *, size_t) ;
MACHINE    RE_class(const BV *) ;
void       RE_cat(MACHINE *, MACHINE *) ;
void       RE_or(MACHINE *, MACHINE *) ;
void       RE_close(MACHINE *) ;
void       RE_poscl(MACHINE *) ;
void       RE_01(MACHINE *) ;
void       RE_panic(const char *) ;
char*      str_str(const char *, size_t , const char *, size_t) ;

void       RE_lex_init (const char *,size_t) ;
int        RE_lex (MACHINE *) ;
void       RE_run_stack_init(void) ;
RT_STATE * RE_new_run_stack(void) ;

#endif   /* REXP_H  */
