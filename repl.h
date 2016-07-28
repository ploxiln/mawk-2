/********************************************
repl.h
copyright 1991,1993,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/

/* repl.h */
/* replacement */

#ifndef  REPL_H
#define  REPL_H

#include "types.h"

PTR re_compile(const STRING *);
const STRING* re_uncompile(PTR);

typedef struct {
    unsigned cnt;		/* number of pieces */
    unsigned amp_cnt;		/* number of & */
    STRING **pieces;
    size_t piece_len;
} Replv_Data;

void replacement_scan(const STRING *, CELL *);
const STRING *repl_unscan(CELL *);

/* only C_REPL needs anything freed */
#define repl_destroy(cp) do{ \
     if((cp)->type == C_REPL) { \
         free_STRING(string(cp)) ; \
     }} while(0)

#endif
