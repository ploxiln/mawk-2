
/********************************************
kw.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/* kw.c */


#include "mawk.h"
#include "symtype.h"
#include "parse.h"
#include "init.h"


const
static struct kw
{
   const char *text ;
   short kw ;
}
keywords[] =
{
   { "print", PRINT },
   { "printf", PRINTF },
   { "do", DO },
   { "while", WHILE },
   { "for", FOR },
   { "break", BREAK },
   { "continue", CONTINUE },
   { "if", IF },
   { "else", ELSE },
   { "in", IN },
   { "delete", DELETE },
   { "split", SPLIT },
   { "length", LENGTH },
   { "match", MATCH_FUNC },
   { "BEGIN", BEGIN },
   { "END", END },
   { "exit", EXIT },
   { "next", NEXT },
   { "nextfile", NEXTFILE },
   { "return", RETURN },
   { "getline", GETLINE },
   { "sub", SUB },
   { "gsub", GSUB },
   { "sprintf", SPRINTF },
   { "function", FUNCTION },
   { 0, 0 }
} ;

/* put keywords in the symbol table */
void
kw_init(void)
{
   register const struct kw *p = keywords ;
   register SYMTAB *q ;

   while (p->text)
   {
      q = insert(p->text) ;
      q->type = ST_KEYWORD ;
      q->stval.kw = p++->kw ;
   }
}

/* find a keyword to emit an error message */
const char *
find_kw_str(int kw_token)
{
   const struct kw *p ;

   for (p = keywords; p->text; p++)
      if (p->kw == kw_token)  return p->text ;
   /* search failed */
   return (char *) 0 ;
}
