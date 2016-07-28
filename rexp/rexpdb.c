
/********************************************
rexpdb.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/



#include "rexp.h"
#include <ctype.h>

/*  print a machine for debugging  */

static  const char *xlat[] = {
"M_STR"  ,
"M_CLASS" ,
"M_ANY" ,
"M_START" ,
"M_END" ,
"M_U",
"M_1J" ,
"M_2JA" ,
"M_2JB" ,
"M_WAIT",
"M_ACCEPT" } ;

/* print string visibly */
static
void visible_string(FILE* fp, const char* s, size_t len)
{
    const char* const s_end = s + len ;

    fputc('"',fp) ;
    while(s < s_end) {
	int c = *s++ ;

	if (c == '"') {
	    fprintf(fp, "\\\"") ;
	}
	else if (c == '\\') {
	    fprintf(fp, "\\\\") ;
	}
	else if(c >= 32 && c < 127) {
	    fprintf(fp, "%c", c) ;
	}
	else {
	    fprintf(fp, "\\x%02x", c) ;
	}
    }
    fputc('"', fp) ;
}


void  REmprint(void* m, FILE* f)
{ STATE *p = (STATE *) m ;
  const char *end_on_string ;
  STATE* last = 0 ;

  while ( 1 )
  {
    if ( p->type >= END_ON )
    { p->type -= END_ON ; end_on_string = "$" ; }
    else end_on_string = "" ;

    if ( p->type < 0 || p->type >= END_ON )
    { fprintf(f, "unknown STATE type\n") ; return ; }

    fprintf(f, "%-10s" , xlat[p->type]) ;
    switch( p->type )
    {
     case M_STR : visible_string(f,p->data.str,p->len) ;
                  break ;

     case M_2JA:
         {
	     STATE* q = p + p->data.jump ;
	     if (q > last) last = q ;
	 }
	 /* fall thru */
     case M_1J:
     case M_2JB : fprintf(f, "%d", p->data.jump) ;
                 break ;
     case M_CLASS:
          {
	      const BV* bvp = p->data.bvp ;
	      unsigned i ;
	      fprintf(f, "%x", (*bvp)[0]) ;
	      for(i=1; i < 8; i++) {
	          fprintf(f, " %x", (*bvp)[i]) ;
	      }
	  }
	  break ;
    }
    fprintf(f, "%s\n" , end_on_string) ;
    if ( end_on_string[0] )  p->type += END_ON ;
    if ( p->type == M_ACCEPT && p >= last)  return ;
    p++ ;
   }
}

