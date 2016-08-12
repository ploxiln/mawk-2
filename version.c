
/********************************************
version.c
copyright 1991-95,2014-2016  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


#include "mawk.h"

/* mawk 1.9 */
static const char* PATCH_STRING = ".9.5" ;
static const char* DATE_STRING  = "11 Aug 2016" ;

static
const char* const version_string =
  "mawk 1.9%s, %s, Copyright Michael D. Brennan\n\n" ;

static  const char*const fmt = "%-14s%10lu\n";
/* print VERSION and exit */
void
print_version(void)
{

   printf(version_string, PATCH_STRING, DATE_STRING) ;
   exit(0) ;
}


