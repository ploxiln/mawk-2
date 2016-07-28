
/********************************************
types.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/



/*  types.h  */

#ifndef  MAWK_TYPES_H
#define  MAWK_TYPES_H

#include  "sizes.h"


/*  CELL  types  */
enum {
    C_NOINIT ,
    C_DOUBLE ,
    C_STRING ,
    C_STRNUM ,
    C_MBSTRN , /*could be STRNUM, has not been checked */
    C_RE ,
    C_SPACE , /* split on space */
    C_SNULL , /* split on the empty string  */
    C_REPL , /* a replacement string   '\&' changed to &  */
    C_REPLV , /* a vector replacement -- broken on &  */
    NUM_CELL_TYPES 
} ;

/* these defines are used to check types for two
   CELLs which are adjacent in memory */

#define  TWO_NOINITS  (2*(1<<C_NOINIT))
#define  TWO_DOUBLES  (2*(1<<C_DOUBLE))
#define  TWO_STRINGS  (2*(1<<C_STRING))
#define  TWO_STRNUMS  (2*(1<<C_STRNUM))
#define  TWO_MBSTRNS  (2*(1<<C_MBSTRN))
#define  NOINIT_AND_DOUBLE  ((1<<C_NOINIT)+(1<<C_DOUBLE))
#define  NOINIT_AND_STRING  ((1<<C_NOINIT)+(1<<C_STRING))
#define  NOINIT_AND_STRNUM  ((1<<C_NOINIT)+(1<<C_STRNUM))
#define  DOUBLE_AND_STRING  ((1<<C_DOUBLE)+(1<<C_STRING))
#define  DOUBLE_AND_STRNUM  ((1<<C_STRNUM)+(1<<C_DOUBLE))
#define  STRING_AND_STRNUM  ((1<<C_STRING)+(1<<C_STRNUM))
#define  NOINIT_AND_MBSTRN  ((1<<C_NOINIT)+(1<<C_MBSTRN))
#define  DOUBLE_AND_MBSTRN  ((1<<C_DOUBLE)+(1<<C_MBSTRN))
#define  STRING_AND_MBSTRN  ((1<<C_STRING)+(1<<C_MBSTRN))
#define  STRNUM_AND_MBSTRN  ((1<<C_STRNUM)+(1<<C_MBSTRN))

typedef  struct {
size_t len ;
unsigned ref_cnt ;
char str[4] ;
} STRING ;

/* number of bytes for a STRING of length len */
#define  STRING_SIZE(len)  ((len)<=3?sizeof(STRING):sizeof(STRING)+(len)-3)

/* compare two strings for == */
#define STRING_eq(s1,s2) ((s1)->len==(s2)->len && \
                          memcmp((s1)->str,(s2)->str,(s1)->len)==0)

/* make copy of STRING */
#define STRING_dup(s)  (((STRING*)(s))->ref_cnt++, (STRING*)(s))

/* like strcmp() */
int STRING_cmp(STRING*,STRING*) ;


typedef  struct cell {
    int type ;
    PTR   ptr ;
    double  dval ;
}  CELL ;


/* all builtins are passed the evaluation stack pointer and
   return its new value, here is the type */

typedef CELL *(*PF_CP)(CELL*) ;

/* an element of code (instruction) */
typedef  union {
int  op ;
PTR  ptr ;
}  INST ;

#endif  /* MAWK_TYPES_H */
