
/********************************************
bi_vars.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/* bi_vars.h */

#ifndef  BI_VARS_H
#define  BI_VARS_H  1


/* builtin variables NF, RS, FS, OFMT are stored
   internally in field[], so side effects of assignment can
   be handled
*/

/* NR and FNR must be next to each other */
#define  NR	   bi_vars
#define  FNR	   (bi_vars+1)
#define  ARGC      (bi_vars+2)
#define  FILENAME  (bi_vars+3)
#define  OFS       (bi_vars+4)
#define  ORS       (bi_vars+5)
#define  RLENGTH   (bi_vars+6)
#define  RSTART    (bi_vars+7)
#define  SUBSEP    (bi_vars+8)

#if  MSDOS
#define  BINMODE   (bi_vars+9)
#define  NUM_BI_VAR  10
#else
#define  NUM_BI_VAR  9
#endif

extern CELL bi_vars[NUM_BI_VAR] ;


#endif
