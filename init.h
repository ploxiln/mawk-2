
/********************************************
init.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/* init.h  */


#ifndef  INIT_H
#define  INIT_H

#include "symtype.h"

/* nodes to link file names for multiple
   -f option */

typedef struct pfile {
struct pfile *link ;
char *fname ;
} PFILE ;

extern PFILE *pfile_list ;

void   initialize(int, char **) ;
void   code_init(void) ;
void   code_cleanup(void) ;
void   compile_cleanup(void) ;
void  scan_init(const char *) ;
void  bi_vars_init(void) ;
void  bi_funct_init(void) ;
void  print_init(void) ;
void  kw_init(void) ;
void   field_init(void) ;
void   fpe_init(void) ;
void   load_environ(ARRAY) ;
void   set_stdoutput(void) ;

#endif   /* INIT_H  */
