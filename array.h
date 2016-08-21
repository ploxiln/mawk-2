/* array.h */
/*
copyright 1991-1996,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

array.c and array.h were generated with the commands

   notangle -R'"array.c"' array.w > array.c 
   notangle -R'"array.h"' array.w > array.h 

Notangle is part of Norman Ramsey's noweb literate programming package.
Noweb home page: http://www.cs.tufts.edu/~nr/noweb/

It's easiest to read or modify this file by working with array.w.
*/

#ifndef ARRAY_H
#define ARRAY_H 1

#include "types.h"
#include "int.h"

typedef struct array {
   void* ptr ;  /* What this points to depends on the type */
   size_t size ; /* number of elts in the table */
   int type ;  /* values in AY_NULL .. AY_SPLIT */
} *ARRAY ;
enum {
   AY_NULL = 0,
   AY_SPLIT,
   AY_STR,
   AY_INT
} ;
#define new_ARRAY()  ((ARRAY)memset(ZMALLOC(struct array),0,sizeof(struct array)))
#define NO_CREATE  0
#define CREATE     1
#define DELETE_   2
typedef struct aloop {
    struct aloop* link ;
    int type ;  /* AY_NULL .. AY_INT */
    unsigned size ;
    unsigned next ;
    CELL* cp ;
    union {
        STRING** sval ;  /* for AY_STR */
        int64_t*  ival ; /* for AY_INT */
    } ptr ;
} ALoop ;
CELL* array_find(ARRAY, CELL*, int);
void  array_delete(ARRAY, CELL*);
void  array_load(ARRAY, size_t);
void  array_clear(ARRAY);
CELL* array_cat(CELL*, int);

ALoop* make_aloop(ARRAY, CELL*) ;
void aloop_free(ALoop*) ;
int aloop_next(ALoop*) ;


#endif /* ARRAY_H */

