
/********************************************
memory.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/*  memory.h  */

#ifndef  MEMORY_H
#define  MEMORY_H

#include "types.h"
#include "zmalloc.h"


STRING *new_STRING(const char*) ;
STRING *new_STRING0(size_t) ;
STRING* new_STRING2(const char*,size_t) ;

#ifdef   DEBUG
void  DB_free_STRING(STRING *) ;

#define  free_STRING(s)  DB_free_STRING(s)

#else

#define  free_STRING(sval)   do { \
                                if ( -- (sval)->ref_cnt == 0 )\
                                    zfree(sval, STRING_SIZE((sval)->len)) ;\
			     } while(0)
#endif


#endif   /* MEMORY_H */
