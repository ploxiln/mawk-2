/* printf.h */
/*
copyright 2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

printf.c and printf.h were generated with the commands

   notangle -R'"printf.c"' printf.w > printf.c 
   notangle -R'"printf.h"' printf.w > printf.h 

Notangle is part of Norman Ramsey's noweb literate programming package.
Noweb home page: http://www.cs.tufts.edu/~nr/noweb/

It's easiest to read or modify this file by working with printf.w.
*/

#ifndef PRINTF_H
#define PRINTF_H 1
#include "mawk.h"
#include "types.h"
#include "memory.h"
#include "files.h"

typedef struct form Form ;
const Form* parse_form(const STRING*) ;

typedef struct spec Spec ;
struct form {
    Form* link ;
    STRING* form ;
    unsigned num_args ;  /* number of args to satisfy form */
    Spec* specs ;  /* each  %..C  is one Spec */
    STRING* ending ;   /* after last Spec */
} ;
STRING* do_xprintf(FILE*, const Form*, CELL*) ;

void da_Form(FILE*,const Form*) ;

#endif /* PRINTF_H */

