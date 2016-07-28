
/********************************************
zmalloc.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


#ifndef  ZMALLOC_H
#define  ZMALLOC_H

#include <stddef.h>

void* emalloc(size_t) ;
void* erealloc(void*,size_t) ;

void* zmalloc(size_t) ;
void* zrealloc(void*,size_t,size_t) ;
void  zfree(void*,size_t) ;

#define ZMALLOC(type)  ((type*)zmalloc(sizeof(type)))
#define ZFREE(p)	zfree(p,sizeof(*(p)))


#endif  /* ZMALLOC_H */
