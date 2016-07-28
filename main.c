
/********************************************
main.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/



/*  main.c  */

#include "mawk.h"
#include "init.h"
#include "code.h"
#include "files.h"


int mawk_state ;		 /* 0 is compiling */
int exit_code ;

int
main(int argc, char **argv)
{

   initialize(argc, argv) ;

   parse() ;

   mawk_state = EXECUTION ;
   execute(execution_start, eval_stack - 1, 0) ;
   /* never returns */
   return 0 ;
}

void
mawk_exit(int x)
{
    if (mawk_state == EXECUTION) {
        if (close_all_output() < 0) {
            x = 2 ;
        }
    }
    exit(x) ;
}
