
/********************************************
mawk.h
copyright 1991-94,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/




/*  mawk.h  */

#ifndef  MAWK_H
#define  MAWK_H

typedef void* PTR ;
typedef int Bool ;

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "types.h"

#ifdef   DEBUG
#define  YYDEBUG  1
extern  int   yydebug ;  /* print parse if on */
extern  int   dump_RE ;
#endif

extern  int  posix_space_flag , interactive_flag ;
extern  int  posix_repl_scan_flag ;

/*----------------
 *  GLOBAL VARIABLES
 *----------------*/

/* a well known string */
extern STRING  null_str ;
extern STRING* const the_empty_str ;

/* a useful scratch area */
extern char* string_buff ;
extern char* string_buff_end ;

char* enlarge_string_buff(char*) ;

/* help with casts */
extern int mpow2[] ;


 /* these are used by the parser, scanner and error messages
    from the compile  */

extern  const char *pfile_name ; /* program input file */
extern  int current_token ;
extern  unsigned  token_lineno ; /* lineno of current token */
extern  unsigned  compile_error_count ;
extern  int  paren_cnt, brace_cnt ;
extern  int  print_flag, getline_flag ;
extern  int mawk_state ;
#define EXECUTION       1  /* other state is 0 compiling */


extern  const char *progname ; /* for error messages */
extern  unsigned rt_nr , rt_fnr ; /* ditto */

/* macro to test the type of two adjacent cells */
#define TEST2(cp)  (mpow2[(cp)->type]+mpow2[((cp)+1)->type])

/* macro to get at the string part of a CELL */
#define string(cp) ((STRING *)(cp)->ptr)

#ifdef   DEBUG
#define cell_destroy(cp)  DB_cell_destroy(cp)
#else

#define cell_destroy(cp)   \
       do { \
           if ((cp)->type >= C_STRING && (cp)->type <= C_MBSTRN) {\
	       free_STRING(string(cp)) ;\
           }\
	} while(0)
#endif

/*  prototypes  */

void   cast1_to_s(CELL *) ;
void   cast1_to_d(CELL *) ;
void   cast_to_RE(CELL *) ;
void   cast_for_split(CELL *) ;
void   check_strnum(CELL *) ;
void   cast_to_REPL(CELL *) ;
int    d_to_I(double) ;

#define cast2_to_s(p)  do{cast1_to_s(p);cast1_to_s(p+1);} while(0)
#define cast2_to_d(p)  do{cast1_to_d(p);cast1_to_d(p+1);} while(0)


int    test(CELL *) ; /* test for null non-null */
CELL * cellcpy(CELL *, CELL *) ;
CELL * repl_cpy(CELL *, CELL *) ;
void   DB_cell_destroy(CELL *) ;
void  overflow(const char *, unsigned) ;
void  rt_overflow(const char *, unsigned) ;
void  rt_error(const char*, ...) ;
void  mawk_exit(int) ;
void  da(INST *, FILE *) ;
char * str_str(const char*, size_t , const char*, size_t) ;
size_t rm_escape(char *) ;
char * re_pos_match(const char *, size_t ,PTR, size_t*, Bool) ;
int    binmode(void) ;

void  parse(void) ;
int   yylex(void) ;
int  yyparse(void) ;
void yyerror(const char *) ;
void scan_cleanup(void) ;

void bozo(const char*) ;
void errmsg(int, const char*, ...) ;
void compile_error(const char*, ...) ;
void call_error(unsigned, const char*, ...) ;
void compile_or_rt_error(const char*, ...) ;

void   execute(INST *, CELL *, CELL *) ;
const char* find_kw_str(int) ;

#endif  /* MAWK_H */
