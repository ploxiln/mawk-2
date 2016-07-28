
/********************************************
memory.c
copyright 1991,1992,2014-2016  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/



/* memory.c */

#include "mawk.h"
#include "memory.h"

STRING null_str =
{0, 1, ""} ;
STRING* const the_empty_str = &null_str ;

static STRING *
xnew_STRING(size_t len)
{
   STRING *sval = (STRING *) zmalloc(STRING_SIZE(len)) ;

   sval->len = len ;
   sval->ref_cnt = 1 ;
   sval->str[len] = 0 ;
   return sval ;
}

/* allocate space for a STRING */

STRING *
new_STRING0(size_t len)
{
   if (len == 0) {
       return STRING_dup(the_empty_str) ;
   }
   else
   {
      return xnew_STRING(len) ;
   }
}

/* memcpy into a STRING */

STRING*
new_STRING2(const char* s, size_t len)
{
    if (len == 0) {
        return STRING_dup(the_empty_str) ;
    }
    else {
        STRING* ret = xnew_STRING(len) ;
	memcpy(ret->str, s, len) ;
	return ret ;
    }
}

/* convert char* to STRING* */

STRING *
new_STRING(const char* s)
{
    size_t len = strlen(s) ;
    return new_STRING2(s,len) ;
}

/* compare two strings in manner of strcmp  */
int STRING_cmp(STRING* s1, STRING* s2) 
{
    int ret ;
    size_t len1 = s1->len ;
    size_t len2 = s2->len ;
    size_t len = len1 <= len2 ? len1 : len2 ;

    ret = memcmp(s1->str, s2->str, len) ;
    if (ret == 0) {
        if (len1 > len) {
	    ret = 1 ;
	}
	else if (len2 > len) {
	    ret = -1 ;
	}
    }
    return ret ;
}
        

#ifdef	 DEBUG

void
DB_free_STRING(STRING* sval)
{
    if (--sval->ref_cnt == 0)  {
        zfree(sval, STRING_SIZE(sval->len)) ;
    }
}

#endif
