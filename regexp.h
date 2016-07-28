
/********************************************
regexp.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


#include <stdio.h>

PTR   REcompile(const char *, size_t)  ;
int   REtest (const char *, size_t, PTR)  ;
char *REmatch(const char *, size_t, PTR, size_t*, Bool)  ;
void  REmprint(PTR , FILE*) ;

extern  int  REerrno ;
extern  const char* const REerrlist[] ;


