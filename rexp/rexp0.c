
/********************************************
rexp0.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/*  lexical scanner  */

#include  "rexp.h"

static int do_str(int, const char **, MACHINE *) ;
static int do_class(const char **, MACHINE *) ;
static int escape(const char **) ;
/*static BV *store_bvp(BV *) ;*/
static int ctohex(int) ;

static const BV* do_named_cc(const char**) ;


/* auto-generated with mktoken.c  */
/* if token defs in rexp.h change then this needs regeneration */
static unsigned char REchar2token[256] = {
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13,  9, 13, 13, 13,  6,  7,  3,  4, 13, 13, 10, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,  5,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 11, 12, 13,  8, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,  1, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13
};


#define NOT_STARTED    (-1)

static int prev ;       /* previous token */
static const char *lp ;		 /*  ptr into reg exp string  */
static const char* re_str ;   /* parsing this string */
static const char* re_end ;   /* end of re_str */



void
RE_lex_init(const char* re, size_t len)
{
    lp = re_str = re ;
    re_end = lp + len ;
    prev = NOT_STARTED ;
    RE_run_stack_init() ;
}


int
RE_lex(MACHINE* mp)
{
    int c ;

    if (lp >= re_end) return 0 ;

    switch (c = REchar2token[*(const unsigned char*)lp]) {
        case T_PLUS:
        case T_STAR:
        case T_OR:
        case T_Q:
        case T_RP:
	    lp++ ;
	    return  prev = c ;

        case T_LP:
	    switch (prev) {
		case T_CHAR:
		case T_STR:
		case T_ANY:
		case T_CLASS:
		case T_START:
		case T_RP:
		case T_PLUS:
		case T_STAR:
		case T_Q:
		case T_U:
		   return prev = T_CAT ;

	        default:
	           lp++ ;
	           return prev = T_LP ;
	 }
   }

   /*  *lp  is	an operand, but implicit cat op is possible   */
    switch (prev) {
        case NOT_STARTED:
        case T_OR:
        case T_LP:
        case T_CAT:
            switch (c) {
	        case T_ANY:
		    lp++ ;
	            *mp = RE_any() ;
		    return prev = T_ANY ;

	        case T_SLASH:
	            lp++ ;
	            c = escape(&lp) ;
	            return prev = do_str(c, &lp, mp) ;

	        case T_CHAR:
	            c = *lp++ ;
	            return prev = do_str(c, &lp, mp) ;

	        case T_CLASS:
	            return prev = do_class(&lp, mp) ;

	       case T_START:
	            *mp = RE_start() ;
	            lp++ ;
	            return prev = T_START ;
	            break ;

	       case T_END:
	           lp++ ;
	           *mp = RE_end() ;
	           return prev = T_END ;

	       default:
	           RE_panic("bad switch in RE_lex") ;
	     }
	     break ;

        default:
	    /* don't advance the pointer */
	    return prev = T_CAT ;
   }

   /* not reached */
   return 0 ;
}

/*
  Collect a run of characters into a string machine.
  If the run ends at *,+, or ?, then don't take the last
  character unless the string has length one.
*/

static int
do_str(
   int c ,			 /* the first character */
   const char **pp ,			 /* where to put the re_char pointer on exit */
   MACHINE *mp )		 /* where to put the string machine */
{
   register const char *p ;		 /* runs thru the input */
   const char *pt = 0 ;			 /* trails p by one */
   char *str ;			 /* collect it here */
   register char *s ;		 /* runs thru the output */
   size_t len ;		 /* length collected */


   p = *pp ;
   s = str = (char *)RE_malloc(re_end - re_str + 1) ;
   *s++ = c ;  len = 1 ;

   while (p < re_end)
   {
      const char *save ;

      switch (REchar2token[*(const unsigned char*)p])
      {
	 case T_CHAR:
	    pt = p ;
	    *s++ = *p++ ;
	    break ;

	 case T_SLASH:
	    pt = p ;
	    save = p+1 ;   /* keep p in a register */
	    *s++ = escape(&save) ;
	    p = save ;
	    break ;

	 default:
	    goto out ;
      }
      len++ ;
   }

out:
   /* if len > 1 and we stopped on a ? + or * , need to back up */
   if (len > 1 && (*p == '*' || *p == '+' || *p == '?'))
   {
      len-- ;
      p = pt ;
      s-- ;
   }

   *s = 0 ;
   *pp = p ;
   *mp = RE_str((char *) RE_realloc(str, len + 1), len) ;
   return T_STR ;
}


/*--------------------------------------------
  BUILD A CHARACTER CLASS
 *---------------------------*/

/* turn on bit x in BV b */
#define	 ON(b,x)  ((b)[(x)>>5] |= ( 1 << ((x)&31) ))

/* turns on bits x..y inclusive */
static void block_on(BV b, unsigned x, unsigned y)
   /* caller's resposibilty to  make sure x<=y < 256 */
{
    unsigned lo = x >> 5 ;
    unsigned hi = (y+1) >> 5 ;
    unsigned r_lo = x&31 ;
    unsigned r_hi = (y+1)&31 ;

#define U32MAX   0xffffffff
    if (lo == hi) {
	b[lo] |= ~(U32MAX<<r_hi) & (U32MAX<<r_lo) ;
    }
    else {
	unsigned i ;

	if (hi < 8) {
	    b[hi] |= ~(U32MAX<<r_hi) ;
	}
	b[lo] |=  (U32MAX << r_lo) ;

        for(i = lo+1; i < hi; i++) {
	    b[i] = U32MAX ;
        }
    }
}

static void BV_or(BV bv1, const BV bv2)
{
    unsigned i ;
    for(i=0; i < 8; i++) {
        bv1[i] |= bv2[i] ;
    }
}

/* build a BV for a character class.
   *start points at the '['
   on exit:   *start points at the character after ']'
	      mp points at a machine that recognizes the class
*/

static int
do_class(const char** start, MACHINE* mp)
{
    const char *p ;
    BV* bvp = 0 ;
    const BV* cc_bvp = 0 ;
    int comp_flag = 0 ;
    const char* t ; /* temp */

    p = *start + 1 ;
    if (p == re_end) {
        RE_error_trap(-E3) ;
    }
    if (*p == '^') {
        comp_flag = 1 ;
	p++ ;
	if (p == re_end) {
	    RE_error_trap(-E3) ;
	}
    }
    /* [].. and [^].. is ok */
    if (*p == ']') {
        if (p+1 == re_end) {
	    /* [] and [^] alone is not OK */
	    RE_error_trap(-E3) ;
	}
	bvp = (BV *)RE_malloc(sizeof(BV)) ;
	memset(bvp,0,sizeof(BV)) ;
	ON(*bvp, ']') ;
	p++ ;
    }
    if (!comp_flag && bvp == 0 && p[0] == '[' && p[1] == ':') {
	t = p ;
        cc_bvp = do_named_cc(&t) ;
	if (*t == ']' && t+1 == re_end) {
	    /* [[:name:]] */
            *mp = RE_class(cc_bvp) ;
	    *start = re_end ;
	    return T_CLASS ;
	}
    }

    if (bvp == 0) {
        bvp = (BV *)RE_malloc(sizeof(BV)) ;
	memset(bvp, 0, sizeof(BV)) ;
	if (cc_bvp) {
	    BV_or(*bvp, *cc_bvp) ;
	}
    }

    {
	int done = 0 ;
	int have_one = -1 ; /* valid if >= 0 */
	int have_dash = 0 ; /* boolean 0 or 1, never 1 if have_one == -1
	                       set if last char was '-'  and have_one >= 0 */

	while(!done && p < re_end) {
	    int c = -1 ;
	    switch(*p) {
	        case ']':
		    if (have_dash) {
		        ON(*bvp,'-') ;
			have_dash = 0 ;
		    }
		    *start = p+1 ;
		    done = 1 ;
		    break ;

		case '[':
		    if (p[1] == ':') {
		        if (have_dash) {
		            ON(*bvp,'-') ;
			    have_dash = 0 ;
			}
			t = p ;
			cc_bvp = do_named_cc(&t) ;
			p = t ;
			BV_or(*bvp, *cc_bvp) ;
			have_one = -1 ;
		    }
		    else {
		        if (have_dash) {
			    if (have_one <= '[') {
			        block_on(*bvp, have_one, '[') ;
			    }
			    else {
			        ON(*bvp,'-') ;
				ON(*bvp, '[') ;
			    }
			    have_one = -1 ;
			    have_dash = 0 ;
			}
			else {
			    have_one = '[' ;
			    ON(*bvp, '[') ;
			}
			p++ ;
		    }
		    break ;

		case '-':
		    if (have_dash) {
		        if (have_one <= '-') {
			    block_on(*bvp,have_one,'-') ;
			    have_dash = 0 ;
			    have_one = -1 ;
			}
			else {
			    ON(*bvp, '-') ;
			}
			have_dash = 0 ;
			have_one = -1 ;
		    }
		    else {
		        if (have_one >= 0) {
			    have_dash = 1 ;
			}
			else {
			    ON(*bvp, '-') ;
			    have_one = '-' ;
			}
		    }
		    p++ ;
		    break ;

		case '\\':
		    t = p+1 ;
		    c = escape(&t) ;
		    p = t ;
		    /* fall thru */

		default:
		    if (c == -1) c = *p++ ;
		    if (have_dash) {
		        if (have_one <= c) {
			    block_on(*bvp, have_one, c) ;
			}
			else {
			    ON(*bvp, '-') ;
			    ON(*bvp, c) ;
			}
			have_one = -1 ;
			have_dash = 0 ;
		    }
		    else {
		        have_one = c ;
			ON(*bvp, c) ;
		    }
		    /* p has already moved */
		    break ;
	    }/*switch*/
	} /*while*/

	if (! done) {
           RE_error_trap(-E3) ;
	}
    }

    if (comp_flag) {
        unsigned i ;
        for(i=0; i < 8; i++) {
            (*bvp)[i] = ~(*bvp)[i] ;
	}
    }
    *mp = RE_class((const BV*) bvp) ;
    return T_CLASS ;
}


/* storage for bit vectors so they can be reused ,
   stored in an unsorted linear array
   the array grows as needed
*/

#define		BV_GROWTH	6

/* unlikely this saves anything so stub out  */

#if 0
static BV *
store_bvp(BV* bvp)
{
   static BV **bv_base, **bv_limit ;
   static BV **bv_next ;	 /* next empty slot in the array */

   register BV **p ;
   unsigned t ;


   if (bv_next == bv_limit)
   {
      /* need to grow */
      if (!bv_base)
      {
	 /* first growth */
	 t = 0 ;
	 bv_base = (BV **) RE_malloc(BV_GROWTH * sizeof(BV *)) ;
      }
      else
      {
	 t = bv_next - bv_base ;
	 bv_base = (BV **) RE_realloc(bv_base, (t + BV_GROWTH) * sizeof(BV *)) ;
      }

      bv_next = bv_base + t ;
      bv_limit = bv_next + BV_GROWTH ;
   }

   /* put bvp in bv_next as a sentinal */
   *bv_next = bvp ;
   p = bv_base ;
   while (memcmp(*p, bvp, sizeof(BV)))	p++ ;

   if (p == bv_next)
   {
      /* it is new */
      bv_next++ ;
   }
   else
   {
      /* we already have it */
      free(bvp) ;
   }

   return *p ;
}
#endif


/* ----------	convert escape sequences  -------------*/

#define isoctal(x)  ((x)>='0'&&(x)<='7')

#define	 NOT_HEX	16
static char hex_val['f' - 'A' + 1] =
{
   10, 11, 12, 13, 14, 15, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   10, 11, 12, 13, 14, 15} ;

/* interpret 1 character as hex */
static int
ctohex(register int c)
{
   int t ;

   if (c >= '0' && c <= '9')  return c - '0' ;
   if (c >= 'A' && c <= 'f' && (t = hex_val[c - 'A']))	return t ;
   return NOT_HEX ;
}

#define	 ET_END	    7

static struct
{
   char in, out ;
}
escape_test[ET_END + 1] =
{
   { 'n', '\n' },
   { 't', '\t' },
   { 'f', '\f' },
   { 'b', '\b' },
   { 'r', '\r' },
   { 'a', '\07' },
   { 'v', '\013' },
   {0, 0}
} ;


/*-----------------
  return the char
  and move the pointer forward
  on entry *s -> at the character after the slash
 *-------------------*/

static int
escape(const char** start_p)
{
   register const char *p = *start_p ;
   register unsigned x ;
   unsigned xx ;
   int i ;


   escape_test[ET_END].in = *p ;
   i = 0 ;
   while (escape_test[i].in != *p)  i++ ;
   if (i != ET_END)
   {
      /* in escape_test table */
      *start_p = p + 1 ;
      return escape_test[i].out ;
   }

   if (isoctal(*p))
   {
      x = *p++ - '0' ;
      if (isoctal(*p))
      {
	 x = (x << 3) + *p++ - '0' ;
	 if (isoctal(*p))  x = (x << 3) + *p++ - '0' ;
      }
      *start_p = p ;
      return x & 0xff ;
   }

   if (*p == 0)	 return '\\' ;

   if (*p++ == 'x')
   {
      if ((x = ctohex(*p)) == NOT_HEX)
      {
	 *start_p  = p ;  return 'x' ;
      }

      /* look for another hex digit */
      if ((xx = ctohex(*++p)) != NOT_HEX)
      {
	 x = (x<<4) + xx ; p++ ;
      }

      *start_p = p ; return x ;
   }

   /* anything else \c -> c */
   *start_p = p ;
   return (int)(*(const unsigned char *) (p - 1)) ;
}

/* named character class  */

#include <ctype.h>

#if !defined(isblank)			/* check for nonstandard macro */

#define isblank isblank_		/* hide from C++ declaration conflict */

int
isblank(int c)
{
    return ((c == ' ') || (c == '\t'));	/* Sun Solaris 10 results */
}

#endif

/* if this table ever changes, make sure it is SORTED on name */

static struct cc_table {
    const char* name ;
    int (* const tester)(int) ;
    const BV* bvp ;
} the_cc_table[] = {
    { "alnum", isalnum,  0},
    { "alpha", isalpha,  0},
    { "blank", isblank,  0},
    { "cntrl", iscntrl,  0},
    { "digit", isdigit,  0},
    { "graph", isgraph,  0},
    { "lower", islower,  0},
    { "print", isprint,  0},
    { "punct", ispunct,  0},
    { "space", isspace,  0},
    { "upper", isupper,  0},
    { "xdigit", isxdigit,  0}
} ;

#define NAME_LEN    6  /* length of longest name in table */

#if defined(__cplusplus)
#define try try_
#endif

/* binary search of the_cc_table */
static const BV* lookup_cclass(const char* name)
{
    typedef struct cc_table CC ;
    CC* left = the_cc_table ;
    CC* right = the_cc_table + sizeof(the_cc_table)/sizeof(CC) ;
    while(left < right) {
        CC* try = left + (right-left)/2 ;
	int k = strcmp(name, try->name) ;
	if (k < 0) right = try ;
	else if (k > 0) left = try+1 ;
	/* found */
	else if (try->bvp) {
	    return try->bvp ;
	}
	else {
	    unsigned c ;
	    int (*tester)(int) = try->tester ;
	    BV* bvp = (BV *)RE_malloc(sizeof(BV)) ;
	    try->bvp = (const BV*) bvp ;
	    memset(bvp, 0, sizeof(BV)) ;
	    for(c = 0; c < 256; c++) {
	        if (tester(c)) {
		    ON(*bvp,c) ;
		}
	    }
	    return (const BV*) bvp ;
	}
    }
    /* not found */
    return 0 ;
}

/* extracts name in [:name:] and then calls lookup_cclass() */
static const BV* do_named_cc(const char** start)
{
    char name[NAME_LEN+1] ;
    const char* p = *start ;
    unsigned cnt = 0 ;
    char* n = name ;
    p += 2 ;   /* move past [: */

    while(p < re_end && cnt <= NAME_LEN) {
        if (*p == ':') {
	    if (p[1] != ']') {
	       /* badly formed class */
	       *start = p ;
               RE_error_trap(-E6) ;
	    }
	    *n = 0 ;
	    *start = p+2 ;
	    {
	        const BV* ret = lookup_cclass(name) ;
		if (ret == 0) {
		    RE_error_trap(-E6) ;
		}
		return ret ;
	    }
	}
	else {
	    *n++ = *p++ ;
	    cnt++ ;
	}
    }
    /* badly formed class */
    *start = p ;
    RE_error_trap(-E6) ;
    /* not reached */
    return 0 ;
}



