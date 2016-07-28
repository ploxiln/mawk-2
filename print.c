
/********************************************
print.c
copyright 1991-1993,2014-2016  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


#include "mawk.h"
#include "bi_vars.h"
#include "bi_funct.h"
#include "memory.h"
#include "field.h"
#include "scan.h"
#include "files.h"
#include "int.h"
#include "printf.h"

static void  print_cell(CELL *, FILE *) ;


/* Once execute() starts the sprintf code is (belatedly) the only
   code allowed to use string_buff  */

static void
print_cell(CELL* p, FILE* fp)
{
   size_t len ;

   switch (p->type)
   {
      case C_NOINIT:
	 break ;
      case C_MBSTRN:
      case C_STRING:
      case C_STRNUM:
	 switch (len = string(p)->len)
	 {
	    case 0:
	       break ;
	    case 1:
	       putc(string(p)->str[0], fp) ;
	       break ;

	    default:
	       fwrite(string(p)->str, 1, len, fp) ;
	 }
	 break ;

      case C_DOUBLE:
	 {
	    double d = p->dval ;
	    if (is_int_double(d)) {
#if   LONG64
		fprintf(fp, "%ld", (int64_t) d) ;
#else
                fprintf(fp, "%lld", (int64_t) d) ;
#endif
	    }
	    else {
	        fprintf(fp, string(OFMT)->str, p->dval) ;
	    }
	 }
	 break ;

      default:
	 bozo("bad cell passed to print_cell") ;
   }
}

/* on entry to bi_print or bi_printf the stack is:

   sp[0] = an integer k
       if ( k < 0 )  output is to a file with name in sp[-1]
       { so open file and sp -= 2 }

   sp[0] = k >= 0 is the number of print args
   sp[-k]   holds the first argument
*/

CELL *
bi_print(CELL *sp)		 /* stack ptr passed in */
{
   register CELL *p ;
   register int k ;
   FILE *fp ;

   k = sp->type ;
   if (k < 0)
   {
      /* k holds redirection */
      if ((--sp)->type < C_STRING)  cast1_to_s(sp) ;
      fp = (FILE *) file_find(string(sp), k) ;
      free_STRING(string(sp)) ;
      k = (--sp)->type ;
      /* k now has number of arguments */
   }
   else	 fp = stdout ;

   if (k)
   {
      p = sp - k ;		 /* clear k variables off the stack */
      sp = p - 1 ;
      k-- ;

      while (k > 0)
      {
	 print_cell(p,fp) ; print_cell(OFS,fp) ;
	 cell_destroy(p) ;
	 p++ ; k-- ;
      }

      print_cell(p, fp) ;  cell_destroy(p) ;
   }
   else
   {				/* print $0 */
      sp-- ;
      print_cell(&field[0], fp) ;
   }

   print_cell(ORS, fp) ;
   if (ferror(fp)) {
       write_error(fp) ;
       mawk_exit(2) ;
   }
   return sp ;
}

/* first argument is the format as a Form* */
CELL *
bi_printf(CELL* sp)
{
   int k ;
   FILE *fp ;
   const Form* form ;

   k = sp->type ;
   if (k < 0) {
      /* k has redirection */
      if ((--sp)->type < C_STRING)  cast1_to_s(sp) ;
      fp = (FILE *) file_find(string(sp), k) ;
      free_STRING(string(sp)) ;
      k = (--sp)->type ;
      /* k is now number of args including format */
   }
   else	 fp = stdout ;

   sp -= k ;			 /* sp points at the format string */
   k-- ;

   form = (const Form*) sp->ptr ;
   do_xprintf(fp, form, sp+1) ;

   /* cleanup arguments on eval stack */
    {
        CELL *p ;
        for (p = sp + 1; k>0; k--, p++)  cell_destroy(p) ;
    }
    return sp - 1 ;
}

/* The format was not a constant string so must be parsed at run-time */
CELL *
bi_printf1(CELL* sp)
{
    int k ;
    FILE *fp ;
    const Form* form ;

    k = sp->type ;
    if (k < 0) {
        /* k has redirection */
        if ((--sp)->type < C_STRING)  cast1_to_s(sp) ;
        fp = (FILE *) file_find(string(sp), k) ;
        free_STRING(string(sp)) ;
        k = (--sp)->type ;
        /* k is now number of args including format */
    }
    else	 fp = stdout ;

    sp -= k ;			 /* sp points at the format string */
    k-- ;

    if (sp->type < C_STRING)  cast1_to_s(sp) ;
    /* parse form doesn't return on error */
    form = parse_form(string(sp)) ;
    if (form->num_args > k) {
        rt_error("not enough arguments passed to printf(\"%s\")",
                 string(sp)->str) ;
    }

    do_xprintf(fp, form, sp+1) ;
    free_STRING(string(sp)) ;

    /* cleanup arguments on eval stack */
    {
        CELL* p ;
        for (p = sp + 1; k; k--, p++)  cell_destroy(p) ;
    }
    return sp - 1 ;
}

/* format is not constant string */
CELL *
bi_sprintf1(CELL* sp)
{
    int argcnt = sp->type ;
    STRING *sval ;
    const Form* form ;

    sp -= argcnt ;		 /* sp points at the format string */
    argcnt-- ;

    if (sp->type != C_STRING)  cast1_to_s(sp) ;
    form = parse_form(string(sp)) ;
    if (form->num_args > argcnt) {
        rt_error("not enough arguments passed to sprintf(\"%s\")",
                 string(sp)->str) ;
    }
    sval = do_xprintf(0, form, sp + 1) ;
    free_STRING(string(sp)) ;
    sp->ptr = (PTR) sval ;  /* sp->type == C_STRING */

    /* cleanup */
    {
        CELL* p ;
        for (p = sp + 1; argcnt; argcnt--, p++)  cell_destroy(p) ;
    }
    return sp ;
}

/* format is constant string parsed to Form* */
CELL *
bi_sprintf(CELL* sp)
{
    int argcnt = sp->type ;
    STRING *sval ;

    sp -= argcnt ;		 /* sp points at the format string */
    argcnt-- ;

    sval = do_xprintf(0, (const Form*) sp->ptr, sp + 1) ;
    sp->type = C_STRING ;
    sp->ptr = (PTR) sval ;

    /* cleanup */
    {
        CELL* p ;
        for (p = sp + 1; argcnt; argcnt--, p++)  cell_destroy(p) ;
    }
    return sp ;
}
