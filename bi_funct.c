
/********************************************
bi_funct.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/

#include "config.h"	/* needed to resolve intersystem conflicts on C and C++ declarations of random() and srandom() */

#include "mawk.h"
#include "bi_funct.h"
#include "bi_vars.h"
#include "memory.h"
#include "init.h"
#include "int.h"
#include "files.h"
#include "fin.h"
#include "field.h"
#include "regexp.h"
#include "repl.h"
#include <math.h>
#include <time.h>
#include <sys/time.h>

#if defined(__cplusplus)
#define THROW__ throw()
#else
#define THROW__
#endif

#if !HAVE_DECL_RANDOM
extern long int random (void) THROW__;
#endif

#if !HAVE_DECL_SRANDOM
extern void srandom (unsigned int __seed) THROW__;
#endif

/* global for the disassembler */
BI_REC bi_funct[] =  {  /* info to load builtins */
   { "index", bi_index, 2, 2 },
   { "substr", bi_substr, 2, 3 },
   { "sin", bi_sin, 1, 1 },
   { "cos", bi_cos, 1, 1 },
   { "atan2", bi_atan2, 2, 2 },
   { "exp", bi_exp, 1, 1 },
   { "log", bi_log, 1, 1 },
   { "int", bi_int, 1, 1 },
   { "sqrt", bi_sqrt, 1, 1 },
   { "rand", bi_rand, 0, 0 },
   { "srand", bi_srand, 0, 1 },
   { "close", bi_close, 1, 1 },
   { "system", bi_system, 1, 1 },
   { "toupper", bi_toupper, 1, 1 },
   { "tolower", bi_tolower, 1, 1 },
   { "fflush", bi_fflush, 0, 1 },
   {0,0,0,0}
} ;

double
infty_(void)
{
    volatile double x;
    x = 0.0;
    return ( 1.0 / x );
}

double
nan_(const char *payload)
{
    volatile double x, y;
    x = y = 0.0;
    return ( y / x );
}

/* load built-in functions in symbol table */
void
bi_funct_init(void)
{
   register BI_REC *p ;
   register SYMTAB *stp ;

   for (p = bi_funct ; p->name; p++)
   {
      stp = insert(p->name) ;
      stp->type = ST_BUILTIN ;
      stp->stval.bip = p ;
   }

/*#if 0*/ /*   zombie lock-step */
#if 1   /* NO zombie lock-step */
   /* seed rand() off the clock */
   {
      CELL c ;
      c.type = 0 ;
      bi_srand(&c) ;
   }
#endif

}

/**************************************************
 string builtins (except split (in split.c) and [g]sub (at end))
 **************************************************/

CELL *
bi_length(CELL* sp)
{
   size_t len ;

   if (sp->type < C_STRING)  cast1_to_s(sp) ;
   len = string(sp)->len ;

   free_STRING(string(sp)) ;
   sp->type = C_DOUBLE ;
   sp->dval = (double) len ;

   return sp ;
}

/* length (size) of an array */
CELL*
bi_alength(CELL* sp)
{
    ARRAY ap = (ARRAY) sp->ptr ;
    sp->type = C_DOUBLE ;
    sp->dval = (double) ap->size ;
    return sp ;
}


/* Sedwick calls this "brute-force algorithm"
   Might be worthwhile trying Boyer-Moore or Knuth...

   In 1991 strstr returned 0 for null key.
   Logically, it should return target.

   So if use for something else modify first return.

   However, leaving as is for mawk
*/

char *
str_str(const char* target, size_t target_len,
        const char* key, size_t key_len)
{
    int k ;

    if (key_len == 0) return 0 ;

    k = key[0] ;

    if (key_len == 1) {
	return target_len > 0 ? (char *)memchr(target,k,target_len) : 0 ;
    }
    else {
        const char* const end = target + target_len - key_len + 1 ;

	while(target < end) {
	    target = (const char *)memchr(target,k,end-target) ;
	    if (target == 0) return 0 ;
	    if (memcmp(target+1 , key+1, key_len - 1) == 0) {
	        return (char*) target ;
	    }
	    target++ ;
	}
	return 0 ;
    }
}


CELL *
bi_index(CELL* sp)
{
   register int idx ;
   size_t len ;
   char *p ;

   sp-- ;
   if (TEST2(sp) != TWO_STRINGS)  cast2_to_s(sp) ;

   if ((len = string(sp + 1)->len)) {
      idx = (p = str_str(string(sp)->str, string(sp)->len,
                    string(sp + 1)->str, len))
	 ? p - string(sp)->str + 1 : 0 ;
   }
   else				/* index of the empty string */
      idx = 1 ;

   free_STRING(string(sp)) ;
   free_STRING(string(sp + 1)) ;
   sp->type = C_DOUBLE ;
   sp->dval = (double) idx ;
   return sp ;
}

/*  substr(s, i, n)
    if l = length(s)  then get the characters
    from  max(1,i) to min(l,n-i-1) inclusive */

CELL *
bi_substr(CELL* sp)
{
   int n_args, len ;
   register int i, n ;
   STRING *sval ;		 /* substr(sval->str, i, n) */

   n_args = sp->type ;
   sp -= n_args ;
   if (sp->type != C_STRING)  cast1_to_s(sp) ;
   /* don't use < C_STRING shortcut */
   sval = string(sp) ;

   if ((len = sval->len) == 0)	/* substr on null string */
   {
      if (n_args == 3)	cell_destroy(sp + 2) ;
      cell_destroy(sp + 1) ;
      return sp ;
   }

   if (n_args == 2)
   {
      n = MAX__INT ;
      if (sp[1].type != C_DOUBLE)  cast1_to_d(sp + 1) ;
   }
   else
   {
      if (TEST2(sp + 1) != TWO_DOUBLES)	 cast2_to_d(sp + 1) ;
      n = d_to_int(sp[2].dval) ;
   }
   i = d_to_int(sp[1].dval) - 1 ;	 /* i now indexes into string */

   if ( i < 0 ) { n += i ; i = 0 ; }
   if (n > len - i)  n = len - i ;

   if (n <= 0)			/* the null string */
   {
      sp->ptr = (PTR) & null_str ;
      null_str.ref_cnt++ ;
   }
   else	 /* got something */
   {
      sp->ptr = (PTR) new_STRING0(n) ;
      memcpy(string(sp)->str, sval->str + i, n) ;
   }

   free_STRING(sval) ;
   return sp ;
}

/*
  match(s,r)
  sp[0] holds r, sp[-1] holds s
*/

CELL *
bi_match(CELL* sp)
{
   char *p ;
   size_t length ;

   if (sp->type != C_RE)  cast_to_RE(sp) ;
   if ((--sp)->type < C_STRING)	 cast1_to_s(sp) ;

   cell_destroy(RSTART) ;
   cell_destroy(RLENGTH) ;
   RSTART->type = C_DOUBLE ;
   RLENGTH->type = C_DOUBLE ;

   p = REmatch(string(sp)->str, string(sp)->len, (sp + 1)->ptr, &length, 0) ;

   if (p)
   {
      sp->dval = (double) (p - string(sp)->str + 1) ;
      RLENGTH->dval = (double) length ;
   }
   else
   {
      sp->dval = 0.0 ;
      RLENGTH->dval = -1.0 ;	 /* posix */
   }

   free_STRING(string(sp)) ;
   sp->type = C_DOUBLE ;

   RSTART->dval = sp->dval ;

   return sp ;
}

CELL *
bi_toupper(CELL* sp)
{
   STRING *old ;
   register char *p, *q ;

   if (sp->type != C_STRING)  cast1_to_s(sp) ;
   old = string(sp) ;
   sp->ptr = (PTR) new_STRING0(old->len) ;

   q = string(sp)->str ; p = old->str ;
   while (*p)
   {
      *q = *p++ ;
      if (*q >= 'a' && *q <= 'z')  *q += 'A' - 'a' ;
      q++ ;
   }
   free_STRING(old) ;
   return sp ;
}

CELL *
bi_tolower(CELL* sp)
{
   STRING *old ;
   register char *p, *q ;

   if (sp->type != C_STRING)  cast1_to_s(sp) ;
   old = string(sp) ;
   sp->ptr = (PTR) new_STRING0(old->len) ;

   q = string(sp)->str ; p = old->str ;
   while (*p)
   {
      *q = *p++ ;
      if (*q >= 'A' && *q <= 'Z')  *q += 'a' - 'A' ;
      q++ ;
   }
   free_STRING(old) ;
   return sp ;
}


/************************************************
  arithemetic builtins
 ************************************************/

#if STDC_MATHERR
static void
fplib_err(fname, val, error)
   char *fname ;
   double val;
   char *error ;
{
   rt_error("%s(%g) : %s", fname, val, error) ;
}
#endif


CELL *
bi_sin(CELL* sp)
{
#if ! STDC_MATHERR
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   sp->dval = sin(sp->dval) ;
   return sp ;
#else
   double x;

   errno = 0 ;
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   x = sp->dval ;
   sp->dval = sin(sp->dval) ;
   if (errno)  fplib_err("sin", x, "loss of precision") ;
   return sp ;
#endif
}

CELL *
bi_cos(CELL* sp)
{
#if ! STDC_MATHERR
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   sp->dval = cos(sp->dval) ;
   return sp ;
#else
   double x;

   errno = 0 ;
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   x = sp->dval ;
   sp->dval = cos(sp->dval) ;
   if (errno)  fplib_err("cos", x, "loss of precision") ;
   return sp ;
#endif
}

CELL *
bi_atan2(CELL* sp)
{
#if  !	STDC_MATHERR
   sp-- ;
   if (TEST2(sp) != TWO_DOUBLES)  cast2_to_d(sp) ;
   sp->dval = atan2(sp->dval, (sp + 1)->dval) ;
   return sp ;
#else

   errno = 0 ;
   sp-- ;
   if (TEST2(sp) != TWO_DOUBLES)  cast2_to_d(sp) ;
   sp->dval = atan2(sp->dval, (sp + 1)->dval) ;
   if (errno)  rt_error("atan2(0,0) : domain error") ;
   return sp ;
#endif
}

CELL *
bi_log(CELL* sp)
{
#if ! STDC_MATHERR
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;

   /* Test on many platforms show inconsistent handling of at least
      negative arguments, so we do our own argument filtering here */
   if (sp->dval < 0.0)
      sp->dval = nan_("");
   else if (sp->dval == 0.0)
      sp->dval = -infty_();
   else
      sp->dval = log(sp->dval) ;

   return sp ;
#else
   double x;

   errno = 0 ;
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   x = sp->dval ;
   sp->dval = log(sp->dval) ;
   if (errno)  fplib_err("log", x, "domain error") ;
   return sp ;
#endif
}

CELL *
bi_exp(CELL* sp)
{
#if  ! STDC_MATHERR
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   sp->dval = exp(sp->dval) ;
   return sp ;
#else
   double x;

   errno = 0 ;
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   x = sp->dval ;
   sp->dval = exp(sp->dval) ;
   if (errno && sp->dval)  fplib_err("exp", x, "overflow") ;
   /* on underflow sp->dval==0, ignore */
   return sp ;
#endif
}

CELL *
bi_int(CELL* sp)
{
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   sp->dval = sp->dval >= 0.0 ? floor(sp->dval) : ceil(sp->dval) ;
   return sp ;
}

CELL *
bi_sqrt(CELL* sp)
{
#if  ! STDC_MATHERR
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   sp->dval = sqrt(sp->dval) ;
   return sp ;
#else
   double x;

   errno = 0 ;
   if (sp->type != C_DOUBLE)  cast1_to_d(sp) ;
   x = sp->dval ;
   sp->dval = sqrt(sp->dval) ;
   if (errno)  fplib_err("sqrt", x, "domain error") ;
   return sp ;
#endif
}

#ifdef  USE_INTERNAL_RNG
/* This is the RNG from
 * Park, SK and Miller KW, "Random Number Generators:
   Good Ones are Hard to Find", CACM, 31, 1192-1201, 1988.

   Note this implemenation requires sizeof(int) == 4.
   Should use int32_t, but we only use this if random() is
   missing which means <stdint.h> is missing too.

   In 1993 Park and Miller suggested changing
   A from 16807 to 48271 which we've done
*/
static int seed = 1;

static
double
mawk_random(void)
{
    static const int M = 0x7fffffff;	/* 2^31-1 */
    static const int A = 48271;	/* used to be 16807 */
    static const int Q = 0x7fffffff / 48271;
    static const int R = 0x7fffffff % 48271;

    int s = seed;
    s = A * (s % Q) - R * (s / Q);
    if (s <= 0)
	s += M;
    seed = s;
    return (double) s / (double) M;
}

static void
mawk_srand(unsigned int u)
{
    u %= 0x7fffffff;
    if (u == 0) {
	u = 314159265;
    }
    seed = u;
}

#else /* use random() and srandom() from stdlib */

static double
mawk_random(void)
{
    return (double) random() / (double) RAND_MAX;
}

static void
mawk_srand(unsigned int u)
{
    srandom(u);
}

#endif

static
CELL prev_seed = /* for return value of srand() */
    {C_DOUBLE, 0, 1.0};

CELL *
bi_srand(CELL *sp)
{
    int nargs = sp->type;
    CELL c;

    if (nargs == 0) {
	/* srand() */
	/* construct a random seed using gettimeofday,
	 * fractional bits should be quite random */
	struct timeval tv;
	char buffer[128];
	gettimeofday(&tv, 0);
	sprintf(buffer, "#srand%ld.%ld#", (long) tv.tv_sec, (long) tv.tv_usec);
	sp->type = C_STRING;
	sp->ptr = new_STRING(buffer);
    }
    else {
	sp--;
    }

    /* new seed is in *sp, swap it with prev_seed
     * no ref_cnt adjust needed since a swap ,
     * this puts the return value in *sp */
    c = *sp;
    *sp = prev_seed;
    prev_seed = c;

    cellcpy(&c, &prev_seed);
    /* now use c to compute an unsigned for call to mawk_srand() */
    {
	unsigned uval;
	int done = 0;
	if (c.type == C_DOUBLE) {
	    /* if exact integer, make small minds happy */
	    double d = c.dval;
	    int ival = d_to_int(d);
	    if ((double) ival == d) {
		uval = ival;
		done = 1;
	    }
	}

	if (!done) {
	    cast1_to_s(&c);
	    uval = hash2(string(&c)->str, string(&c)->len);
	    cell_destroy(&c);
	}
	mawk_srand(uval);
    }
    return sp ;
}

CELL *
bi_rand(CELL *sp)
{
    sp++;
    sp->type = C_DOUBLE;
    sp->dval = mawk_random();
    return sp ;
}

/*************************************************
 miscellaneous builtins
 close, system and getline
 fflush
 *************************************************/

static double exit_status_d(int) ;

CELL *
bi_close(CELL* sp)
{
   int x ;

   if (sp->type < C_STRING)  cast1_to_s(sp) ;
   x = file_close((STRING *) sp->ptr) ;
   free_STRING(string(sp)) ;
   sp->type = C_DOUBLE ;
   sp->dval = exit_status_d(x) ;
   return sp ;
}


CELL *
bi_fflush(CELL* sp)
{
   int ret = 0 ;

   if ( sp->type == 0 )  {
       if (fflush(stdout) < 0) {
           write_error(stdout) ;
	   mawk_exit(2) ;
       }
   }
   else
   {
      sp-- ;
      if ( sp->type < C_STRING ) cast1_to_s(sp) ;
      ret = file_flush(string(sp)) ;
      free_STRING(string(sp)) ;
   }

   sp->type = C_DOUBLE ;
   sp->dval = (double) ret ;
   return sp ;
}

/* return from system(), wait(), pclose() is converted
           to a normal exit status or 256 + # of killing signal */
static double exit_status_d(int status)
{
    int ret ;
    if (status == -1) return -1.0 ;
    if (WIFEXITED(status)) {
        ret = WEXITSTATUS(status) ;
    }
    else {
        ret = WTERMSIG(status) + 256 ; /* ok Arnold? */
    }
    return (double) ret ;
}
        


CELL *
bi_system(CELL* sp)
{
    int status ;

    if (sp->type < C_STRING)  cast1_to_s(sp) ;
    flush_all_output() ;
    status = system(string(sp)->str) ;

    cell_destroy(sp) ;
    sp->type = C_DOUBLE ;
    sp->dval = exit_status_d(status) ;
    return sp ;
}



#if   MSDOS


CELL *
bi_system(sp)
   register CELL *sp ;
{
   int retval ;

   if (sp->type < C_STRING)  cast1_to_s(sp) ;
   retval = DOSexec(string(sp)->str) ;
   free_STRING(string(sp)) ;
   sp->type = C_DOUBLE ;
   sp->dval = (double) retval ;
   return sp ;
}

#endif


/*  getline()  */

/*  if type == 0 :  stack is 0 , target address

    if type == F_IN : stack is F_IN, expr(filename), target address
    if type == PIPE_IN : stack is PIPE_IN, target address, expr(pipename)
*/

CELL *
bi_getline(CELL* sp)
{
   CELL tc ;
   CELL* cp = 0 ;
   char *p = 0 ;
   size_t len ;
   FIN *fin_p ;


   switch (sp->type)
   {
      case 0:
	 sp-- ;
	 if (!main_fin)	 open_main() ;

	 if (!(p = FINgets(main_fin, &len)))  goto eof ;

	 cp = (CELL *) sp->ptr ;
	 if (TEST2(NR) != TWO_DOUBLES)	cast2_to_d(NR) ;
	 NR->dval += 1.0 ;  rt_nr++ ;
	 FNR->dval += 1.0 ; rt_fnr++ ;
	 break ;

      case F_IN:
	 sp-- ;
	 if (sp->type < C_STRING)  cast1_to_s(sp) ;
	 fin_p = (FIN *) file_find((STRING *)sp->ptr, F_IN) ;
	 free_STRING(string(sp)) ;
	 sp-- ;

	 if (!fin_p)  goto open_failure ;
	 if (!(p = FINgets(fin_p, &len)))
	 {
	    FINsemi_close(fin_p) ;
	    goto eof ;
	 }
	 cp = (CELL *) sp->ptr ;
	 break ;

      case PIPE_IN:
	 sp -= 2 ;
	 if (sp->type < C_STRING)  cast1_to_s(sp) ;
	 fin_p = (FIN *) file_find((STRING *)sp->ptr, PIPE_IN) ;
	 free_STRING(string(sp)) ;

	 if (!fin_p)  goto open_failure ;
	 if (!(p = FINgets(fin_p, &len)))
	 {
	    FINsemi_close(fin_p) ;
	    goto eof ;
	 }
	 cp = (CELL *) (sp + 1)->ptr ;
	 break ;

      default:
	 bozo("type in bi_getline") ;

   }

   /* we've read a line , store it */

   if (len == 0)
   {
      tc.type = C_STRING ;
      tc.ptr = STRING_dup(the_empty_str) ;
   }
   else
   {
      tc.type = C_MBSTRN ;
      tc.ptr = (PTR) new_STRING2(p,len) ;
   }

   slow_cell_assign(cp, &tc) ;

   cell_destroy(&tc) ;

   sp->dval = 1.0  ;  goto done ;
 open_failure:
   sp->dval = -1.0  ; goto done ;
 eof:
   sp->dval = 0.0 ;		 /* fall thru to done  */

 done:sp->type = C_DOUBLE;
   return sp ;
}

/**********************************************
 sub() and gsub()
 **********************************************/

/* helper function:
   inputs: replv is Replv_Data*
           match and m_len are a string and its length

	   The pieces of replv and & replaced by match
	   are copied to target

   return: the total size copied to target
*/

static size_t
replace_v_copy(char *target, const Replv_Data * replv, const char *match, size_t m_len)
{
    size_t ret = 0;
    unsigned cnt = replv->cnt;
    unsigned i;
    STRING **sv = replv->pieces;
    for (i = 0; i < cnt; i++) {
	STRING *str = sv[i];
	if (str) {
	    memcpy(target, str->str, str->len);
	    target += str->len;
	    ret += str->len;
	} else {
	    memcpy(target, match, m_len);
	    target += m_len;
	    ret += m_len;
	}
    }
    return ret;
}

/* entry:  sp[0] = address of CELL to sub on
	   sp[-1] = substitution CELL
	   sp[-2] = regular expression to match
*/

CELL *
bi_sub(CELL *sp)
{
    CELL *cp;			/* pointer to the replacement target */
    CELL tc;			/* build the new string here */
    CELL sc;			/* copy of the target CELL */
    char *front, *match, *back;	/* pieces */
    size_t front_len, match_len, back_len;
    size_t input_len;
    size_t result_len;

    sp -= 2;
    if (sp->type != C_RE)
	cast_to_RE(sp);
    if (sp[1].type != C_REPL && sp[1].type != C_REPLV)
	cast_to_REPL(sp + 1);
    cp = (CELL *) (sp + 2)->ptr;
    /* make a copy of the target, because we won't change anything
       including type unless the match works */
    cellcpy(&sc, cp);
    if (sc.type < C_STRING)
	cast1_to_s(&sc);
    front = string(&sc)->str;
    input_len = string(&sc)->len;

    if ((match = REmatch(front, string(&sc)->len, sp->ptr, &match_len, 0))) {
	front_len = (size_t) (match - front);
	back = match + match_len;
	back_len = input_len - front_len - match_len;

	if ((sp + 1)->type == C_REPLV) {
	    Replv_Data *rd = (Replv_Data *) (sp + 1)->ptr;
	    result_len = input_len + rd->piece_len
		+ (rd->amp_cnt - 1) * match_len;
	} else {
	    result_len = input_len + string(sp + 1)->len - match_len;
	}

	tc.type = C_STRING;
	tc.ptr = (PTR) new_STRING0(result_len);

	{
	    char *p = string(&tc)->str;

	    if (front_len) {
		memcpy(p, front, front_len);
		p += front_len;
	    }
	    if ((sp + 1)->type == C_REPL) {
		memcpy(p, string(sp + 1)->str, string(sp + 1)->len);
		p += string(sp + 1)->len;
	    } else {		/* C_REPLV  */
		size_t sz = replace_v_copy(p, (Replv_Data *) (sp + 1)->ptr,
					   match, match_len);
		p += sz;
	    }
	    if (back_len) {
		memcpy(p, back, back_len);
	    }
	}

	slow_cell_assign(cp, &tc);

	free_STRING(string(&tc));
    }

    free_STRING(string(&sc));
    repl_destroy(sp + 1);
    sp->type = C_DOUBLE;
    sp->dval = match != (char *) 0 ? 1.0 : 0.0;

    return sp ;
}

/* Gsub collects match information in a sequential container of
 * Match_Point
*/
typedef struct {
    const char *ptr;
    size_t len;
} Match_Point;

/* For 99.999% of uses, an  array would be big enough, but for the
 * other .001% we need something that can grow
*/

#ifdef   MEM_CHECK
#define GS_BUFFER_SZ   4	/* give list a workout */
#else
#define GS_BUFFER_SZ   512
#endif

typedef struct gsub_block {
    Match_Point buffer[GS_BUFFER_SZ];
    struct gsub_block *link;
} Gsub_Block;

static Gsub_Block *
grow_gs_list(Gsub_Block * tail)
{
    tail = tail->link = (Gsub_Block *) emalloc(sizeof(Gsub_Block));
    tail->link = 0;
    return tail;
}

static void
destroy_gs_list(Gsub_Block * p)
{
    while (p) {
	Gsub_Block *hold = p;
	p = p->link;
	free(hold);
    }
}

/*
   The caller owns the data pointed to by the input parms.
   The return is a new STRING allocated here but owned by the
   caller on return with
   replacement replacing each match of re in input_s.

   replacement is a simple replacement not one with &
*/

static STRING *
gsub(const PTR re,
     const STRING * replacement,
     const STRING * input_s,
     unsigned *cnt_p)
{
    Gsub_Block first_block;
    unsigned idx;
    Gsub_Block *const gs_list_base = &first_block;
    Gsub_Block *gs_block = gs_list_base;
    unsigned cnt;		/* number of matches */

    const char *input = input_s->str;
    const char *input_end = input + input_s->len;
    size_t output_len = input_s->len;

    gs_block->link = 0;

    /* get the matches */
    {
	int zero_match_ok = 1;
	/* get the first match */
	size_t m_len;
	const char *match_point =
	REmatch(input, input_s->len, re, &m_len, 0);

	if (match_point == 0) {
	    /* no matches, so no subs
	       add a ref cnt and return input_s */
	    STRING *ret = (STRING *) input_s;
	    *cnt_p = 0;
	    ret->ref_cnt++;
	    return ret;
	}

	/* have first match */
	gs_block->buffer[0].ptr = match_point;
	gs_block->buffer[0].len = m_len;
	output_len += (replacement->len - m_len);
	cnt = 1;
	idx = 1;
	if (m_len > 0) {
	    input = match_point + m_len;
	    /* after a non-null match, next match cannot be zero length
	       at front */
	    zero_match_ok = 0;
	} else {
	    input = match_point + 1;
	}

	/* get the rest */
	while (input <= input_end) {
	    match_point =
		REmatch(input, input_end - input, re, &m_len,
			1 /* turn off ^ */ );
	    if (match_point == 0) {
		break;
	    }

	    if (match_point == input && m_len == 0 && !zero_match_ok) {
		/* match of zero length to be skipped */
		input++;
		zero_match_ok = 1;
		continue;
	    }

	    gs_block->buffer[idx].ptr = match_point;
	    gs_block->buffer[idx].len = m_len;
	    output_len += (replacement->len - m_len);
	    cnt++;
	    if (m_len > 0) {
		input = match_point + m_len;
		zero_match_ok = 0;
	    } else {
		input = match_point + 1;
		zero_match_ok = 1;
	    }
	    idx++;
	    if (idx == GS_BUFFER_SZ) {
		gs_block = grow_gs_list(gs_block);
		idx = 0;
	    }
	}
    }

    *cnt_p = cnt;
    {				/* build the output and return it */
	STRING *output = new_STRING0(output_len);
	char *outp = output->str;
	const char *rep = replacement->str;
	size_t rep_len = replacement->len;

	size_t out_done = 0;	/* debug counter */

	idx = 0;
	gs_block = gs_list_base;
	input = input_s->str;

	while (1) {
	    size_t sz;
	    Match_Point mp = gs_block->buffer[idx];
	    sz = mp.ptr - input;
	    memcpy(outp, input, sz);
	    outp += sz;
	    out_done += sz;
	    /* append replacement */
	    memcpy(outp, rep, rep_len);
	    outp += rep_len;
	    out_done += rep_len;

	    if (mp.len == 0 && mp.ptr < input_end) {
		/* match is in front of mp.ptr */
		*outp++ = *mp.ptr;
		input = mp.ptr + 1;
		out_done += 1;
	    } else {
		input = mp.ptr + mp.len;
	    }

	    cnt--;
	    if (cnt == 0) {
		/* cpy what's left if any */
		if (input < input_end) {
		    memcpy(outp, input, input_end - input);
		    out_done += input_end - input;
		}
		break;
	    }
	    idx++;
	    if (idx == GS_BUFFER_SZ) {
		gs_block = gs_block->link;
		idx = 0;
	    }
	}
	/* done, cleanup and return */
	if (out_done != output->len) {
	    bozo("invalid output length in gsub");
	}
	destroy_gs_list(gs_list_base->link);
	return output;
    }
}

/*
   The caller owns the data pointed to by the input parms.
   The return is a new STRING allocated here but owned by the
   caller on return with
   replacev replacing each match of re in input_s.

   Mostly a copy of gsub() above, difference is
   replacev is a vector replacement, one with &
*/

static STRING *
gsubv(const PTR re,
      const Replv_Data * replacev,
      const STRING * input_s,
      unsigned *cnt_p)
{
    Gsub_Block first_block;
    unsigned idx;
    Gsub_Block *const gs_list_base = &first_block;
    Gsub_Block *gs_block = gs_list_base;
    unsigned cnt;		/* number of matches */

    const char *input = input_s->str;
    const char *input_end = input + input_s->len;
    size_t output_len = input_s->len;

    size_t replace_len = replacev->piece_len;
    unsigned amp_cnt = replacev->amp_cnt;

    gs_block->link = 0;

    /* get the matches */
    {
	int zero_match_ok = 1;
	/* get the first match */
	size_t m_len;
	const char *match_point =
	REmatch(input, input_s->len, re, &m_len, 0);

	if (match_point == 0) {
	    /* no matches, so no subs
	       add a ref cnt and return input_s */
	    STRING *ret = (STRING *) input_s;
	    *cnt_p = 0;
	    ret->ref_cnt++;
	    return ret;
	}

	/* have first match */
	gs_block->buffer[0].ptr = match_point;
	gs_block->buffer[0].len = m_len;
	output_len += (replace_len + (amp_cnt - 1) * m_len);
	cnt = 1;
	idx = 1;
	if (m_len > 0) {
	    input = match_point + m_len;
	    /* after a non-null match, next match cannot be zero length
	       at front */
	    zero_match_ok = 0;
	} else {
	    input = match_point + 1;
	}

	/* get the rest */
	while (input <= input_end) {
	    match_point =
		REmatch(input, input_end - input, re, &m_len,
			1 /* turn off ^ */ );
	    if (match_point == 0) {
		break;
	    }

	    if (match_point == input && m_len == 0 && !zero_match_ok) {
		/* match of zero length to be skipped */
		input++;
		zero_match_ok = 1;
		continue;
	    }

	    gs_block->buffer[idx].ptr = match_point;
	    gs_block->buffer[idx].len = m_len;
	    output_len += (replace_len + (amp_cnt - 1) * m_len);
	    cnt++;
	    if (m_len > 0) {
		input = match_point + m_len;
		zero_match_ok = 0;
	    } else {
		input = match_point + 1;
		zero_match_ok = 1;
	    }
	    idx++;
	    if (idx == GS_BUFFER_SZ) {
		gs_block = grow_gs_list(gs_block);
		idx = 0;
	    }
	}
    }

    *cnt_p = cnt;
    {				/* build the output and return it */
	STRING *output = new_STRING0(output_len);
	char *outp = output->str;

	size_t out_done = 0;	/* debug counter */

	idx = 0;
	gs_block = gs_list_base;
	input = input_s->str;

	while (1) {
	    size_t sz;
	    Match_Point mp = gs_block->buffer[idx];
	    sz = mp.ptr - input;
	    memcpy(outp, input, sz);
	    outp += sz;
	    out_done += sz;
	    /* append replacement */
	    sz = replace_v_copy(outp, replacev, mp.ptr, mp.len);
	    outp += sz;
	    out_done += sz;

	    if (mp.len == 0 && mp.ptr < input_end) {
		/* match is in front of mp.ptr */
		*outp++ = *mp.ptr;
		input = mp.ptr + 1;
		out_done += 1;
	    } else {
		input = mp.ptr + mp.len;
	    }

	    cnt--;
	    if (cnt == 0) {
		/* cpy what's left if any */
		if (input < input_end) {
		    memcpy(outp, input, input_end - input);
		    out_done += input_end - input;
		}
		break;
	    }
	    idx++;
	    if (idx == GS_BUFFER_SZ) {
		gs_block = gs_block->link;
		idx = 0;
	    }
	}
	/* done, cleanup and return */
	if (out_done != output->len) {
	    bozo("invalid output length in gsub");
	}
	destroy_gs_list(gs_list_base->link);
	return output;
    }
}

/* set up for call to gsub() */
CELL *
bi_gsub(CELL *sp)
{
    CELL *cp;			/* pts at the replacement target */
    CELL sc;			/* copy of replacement target */
    CELL tc;			/* build the result here */
    unsigned repl_cnt;

    sp -= 2;
    if (sp->type != C_RE)
	cast_to_RE(sp);
    if ((sp + 1)->type != C_REPL && (sp + 1)->type != C_REPLV)
	cast_to_REPL(sp + 1);

    cp = (CELL *) (sp + 2)->ptr;
    cellcpy(&sc, cp);
    if (sc.type < C_STRING) {
	cast1_to_s(&sc);
    }

    repl_cnt = 0;
    if ((sp + 1)->type == C_REPL) {
	tc.ptr = gsub(sp->ptr, string(sp + 1), string(&sc), &repl_cnt);
    } else {
	tc.ptr = (PTR) gsubv(sp->ptr, (Replv_Data *) (sp + 1)->ptr,
			     string(&sc), &repl_cnt);
    }

    if (repl_cnt) {
	tc.type = C_STRING;
	slow_cell_assign(cp, &tc);
    }

    /* cleanup */
    free_STRING(string(&sc));
    free_STRING(string(&tc));
    repl_destroy(sp + 1);

    sp->type = C_DOUBLE;
    sp->dval = (double) repl_cnt;

    return sp ;
}
