
/********************************************
bi_vars.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/



/* bi_vars.c */

#include "mawk.h"
#include "symtype.h"
#include "bi_vars.h"
#include "field.h"
#include "init.h"
#include "memory.h"

/* the builtin variables */
CELL  bi_vars[NUM_BI_VAR] ;

/* the order here must match the order in bi_vars.h */

static const char *bi_var_names[NUM_BI_VAR] = {
"NR" ,
"FNR" ,
"ARGC" ,
"FILENAME" ,
"OFS" ,
"ORS" ,
"RLENGTH" ,
"RSTART" ,
"SUBSEP"
#if MSDOS
, "BINMODE"
#endif
} ;

/* insert the builtin vars in the hash table */

void  bi_vars_init(void)
{ register int i ;
  register SYMTAB *s ;


  for ( i = 0 ; i < NUM_BI_VAR ; i++ )
  { s = insert( bi_var_names[i] ) ;
    s->type = i <= 1 ? ST_NR : ST_VAR ;
    s->stval.cp = bi_vars + i ;
    /* bi_vars[i].type = 0 which is C_NOINIT */
  }

  s = insert("ENVIRON") ;
  s->type = ST_ENV ;

  /* set defaults */

  FILENAME->type = C_STRING ;
  FILENAME->ptr = (PTR) new_STRING( "" ) ;

  OFS->type = C_STRING ;
  OFS->ptr = (PTR) new_STRING( " " ) ;

  ORS->type = C_STRING ;
  ORS->ptr = (PTR) new_STRING( "\n" ) ;

  SUBSEP->type = C_STRING ;
  SUBSEP->ptr =  (PTR) new_STRING( "\034" ) ;

  NR->type = FNR->type = C_DOUBLE ;
  /* dval is already 0.0 */

#if  MSDOS
  BINMODE->type = C_DOUBLE ;
#endif
}
