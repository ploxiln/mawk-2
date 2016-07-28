
/********************************************
scan.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/




#include  "mawk.h"
#include  "scan.h"
#include  "memory.h"
#include  "field.h"
#include  "init.h"
#include  "int.h"
#include  "fin.h"
#include  "repl.h"
#include  "code.h"

#include  <fcntl.h>

#include  "files.h"


/* static functions */
static void  scan_fillbuff(void) ;
static void  scan_open(void) ;
static int  slow_next(void) ;
static void  eat_comment(void) ;
static void  eat_semi_colon(void) ;
static double  collect_decimal(int, int *) ;
static int  collect_string(void) ;
static int  collect_RE(void) ;


/*-----------------------------
  program file management
 *----------------------------*/

const char *pfile_name ;
STRING *program_string ;
PFILE *pfile_list ;
static unsigned char *buffer ;
static unsigned char *buffp ;
 /* unsigned so it works with 8 bit chars */
static int program_fd ;
static int eof_flag ;

/* tmp buffer */
char* string_buff ;
char* string_buff_end ;

/* on entry p points into string_buff, grow string_buff and
   return a new p */
char* enlarge_string_buff(char* p)
{
    const unsigned s32 = 32 * 1024 ;
    size_t len = string_buff_end - string_buff ;
    size_t nlen = (len < s32) ? 2*len : len+s32 ;
    size_t pdiff = p - string_buff ;
    string_buff = (char *)erealloc(string_buff,nlen) ;
    string_buff_end = string_buff + nlen ;
    return string_buff + pdiff ;
}


void
scan_init(const char* cmdline_program)
{
   if (cmdline_program)
   {
      program_fd = -1 ;		 /* command line program */
      program_string = new_STRING0(strlen(cmdline_program) + 1) ;
      strcpy(program_string->str, cmdline_program) ;
      /* simulate file termination */
      program_string->str[program_string->len - 1] = '\n' ;
      buffp = (unsigned char *) program_string->str ;
      eof_flag = 1 ;
   }
   else	 /* program from file[s] */
   {
      scan_open() ;
      buffp = buffer = (unsigned char *) zmalloc(BUFFSZ + 1) ;
      buffer[BUFFSZ] = 0 ;
      scan_fillbuff() ;
   }

#ifdef OS2  /* OS/2 "extproc" is similar to #! */
   if (strnicmp(buffp, "extproc ", 8) == 0)
     eat_comment();
#endif
   eat_nl() ;			 /* scan to first token */
   if (next() == 0)
   {
      /* no program */
      mawk_exit(0) ;
   }

   un_next() ;

}

static void
scan_open()			/* open pfile_name */
{
   if (pfile_name[0] == '-' && pfile_name[1] == 0)
   {
      program_fd = 0 ;
   }
   else if ((program_fd = open(pfile_name, O_RDONLY, 0)) == -1)
   {
      errmsg(errno, "cannot open %s", pfile_name) ;
      mawk_exit(2) ;
   }
}

void
scan_cleanup(void)
{
   if (program_fd >= 0)	 zfree(buffer, BUFFSZ + 1) ;
   else	 free_STRING(program_string) ;

   if (program_fd > 0)	close(program_fd) ;

   /* redefine SPACE as [ \t\n] */

   scan_code['\n'] = posix_space_flag && rs_shadow.type != SEP_MLR
      ? SC_UNEXPECTED : SC_SPACE ;
   scan_code['\f'] = SC_UNEXPECTED ;	 /*value doesn't matter */
   scan_code['\013'] = SC_UNEXPECTED ;	 /* \v not space */
   scan_code['\r'] = SC_UNEXPECTED ;
}

/*--------------------------------
  global variables shared by yyparse() and yylex()
  and used for error messages too
 *-------------------------------*/

int current_token = -1 ;
unsigned token_lineno ;
unsigned compile_error_count ;
int NR_flag ;			 /* are we tracking NR */
int paren_cnt ;
int brace_cnt ;
int print_flag ;		 /* changes meaning of '>' */
int getline_flag ;		 /* changes meaning of '<' */


/*----------------------------------------
 file reading functions
 next() and un_next(c) are macros in scan.h

 *---------------------*/

static unsigned lineno = 1 ;


static void
scan_fillbuff(void)
{
   unsigned r ;

   /* size of buffer is BUFFSZ+1 and buffer[BUFFSZ] == 0 */
   r = fillbuff(program_fd, (char *) buffer, BUFFSZ) ;
   if (r < BUFFSZ)
   {
      eof_flag = 1 ;
      /* make sure eof is terminated */
      buffer[r] = '\n' ;
      buffer[r + 1] = 0 ;
   }
}

/* read one character -- slowly */
static int
slow_next(void)
{

   while (*buffp == 0)
   {
      if (!eof_flag)
      {
	 buffp = buffer ;
	 scan_fillbuff() ;
      }
      else if (pfile_list /* open another program file */ )
      {
	 PFILE *q ;

	 if (program_fd > 0)  close(program_fd) ;
	 eof_flag = 0 ;
	 pfile_name = pfile_list->fname ;
	 q = pfile_list ;
	 pfile_list = pfile_list->link ;
	 ZFREE(q) ;
	 scan_open() ;
	 token_lineno = lineno = 1 ;
      }
      else  break /* real eof */ ;
   }

   return *buffp++ ;		 /* note can un_next() , eof which is zero */
}

static void
eat_comment(void)
{
   register int c ;

   while ((c = next()) != '\n' && scan_code[c]) ;
   un_next() ;
}

/* this is how we handle extra semi-colons that are
   now allowed to separate pattern-action blocks

   A proof that they are useless clutter to the language:
   we throw them away
*/

static void
eat_semi_colon(void)
/* eat one semi-colon on the current line */
{
   register int c ;

   while (scan_code[c = next()] == SC_SPACE) ;
   if (c != ';')  un_next() ;
}

void
eat_nl()			/* eat all space including newlines */
{
   while (1)
      switch (scan_code[next()])
      {
	 case SC_COMMENT:
	    eat_comment() ;
	    break ;

	 case SC_NL:
	    lineno++ ;
	    /* fall thru  */

	 case SC_SPACE:
	    break ;

	 case SC_ESCAPE:
	    /* bug fix - surprised anyone did this,
	       a csh user with backslash dyslexia.(Not a joke)
	    */
	    {
	       unsigned c ;

	       while (scan_code[c = next()] == SC_SPACE) ;
	       if (c == '\n')
		  token_lineno = ++lineno ;
	       else if (c == 0)
	       {
		  un_next() ;
		  return ;
	       }
	       else /* error */
	       {
		  un_next() ;
		  /* can't un_next() twice so deal with it */
		  yylval.ival = '\\' ;
		  unexpected_char() ;
		  if( ++compile_error_count == MAX_COMPILE_ERRORS )
		     mawk_exit(2) ;
		  return ;
	       }
	    }
	    break ;

	 default:
	    un_next() ;
	    return ;
      }
}

int
yylex(void)
{
   register int c ;

   token_lineno = lineno ;

reswitch:

   switch (scan_code[c = next()])
   {
      case 0:
	 ct_ret(EOF) ;

      case SC_SPACE:
	 goto reswitch ;

      case SC_COMMENT:
	 eat_comment() ;
	 goto reswitch ;

      case SC_NL:
	 lineno++ ;
	 eat_nl() ;
	 ct_ret(NL) ;

      case SC_ESCAPE:
	 while (scan_code[c = next()] == SC_SPACE) ;
	 if (c == '\n')
	 {
	    token_lineno = ++lineno ;
	    goto reswitch ;
	 }

	 if (c == 0)  ct_ret(EOF) ;
	 un_next() ;
	 yylval.ival = '\\' ;
	 ct_ret(UNEXPECTED) ;


      case SC_SEMI_COLON:
	 eat_nl() ;
	 ct_ret(SEMI_COLON) ;

      case SC_LBRACE:
	 eat_nl() ;
	 brace_cnt++ ;
	 ct_ret(LBRACE) ;

      case SC_PLUS:
	 switch (next())
	 {
	    case '+':
	       yylval.ival = '+' ;
	       string_buff[0] =
		  string_buff[1] = '+' ;
	       string_buff[2] = 0 ;
	       ct_ret(INC_or_DEC) ;

	    case '=':
	       ct_ret(ADD_ASG) ;

	    default:
	       un_next() ;
	       ct_ret(PLUS) ;
	 }

      case SC_MINUS:
	 switch (next())
	 {
	    case '-':
	       yylval.ival = '-' ;
	       string_buff[0] =
		  string_buff[1] = '-' ;
	       string_buff[2] = 0 ;
	       ct_ret(INC_or_DEC) ;

	    case '=':
	       ct_ret(SUB_ASG) ;

	    default:
	       un_next() ;
	       ct_ret(MINUS) ;
	 }

      case SC_COMMA:
	 eat_nl() ;
	 ct_ret(COMMA) ;

      case SC_MUL:
	 test1_ret('=', MUL_ASG, MUL) ;

      case SC_DIV:
	 {
	    static int can_precede_div[] =
	    {DOUBLE, STRING_, RPAREN, ID, D_ID, RE, RBOX, FIELD,
	     GETLINE, INC_or_DEC, -1} ;

	    int *p = can_precede_div ;

	    do
	    {
	       if (*p == current_token)
	       {
		  if (*p != INC_or_DEC)	 {
		      test1_ret('=', DIV_ASG, DIV) ;
		  }

		  if (next() == '=') {
		     un_next() ;
		     ct_ret(collect_RE()) ;
		  }
	       }
	    }
	    while (*++p != -1) ;

	    ct_ret(collect_RE()) ;
	 }

      case SC_MOD:
	 test1_ret('=', MOD_ASG, MOD) ;

      case SC_POW:
	 test1_ret('=', POW_ASG, POW) ;

      case SC_LPAREN:
	 paren_cnt++ ;
	 ct_ret(LPAREN) ;

      case SC_RPAREN:
	 if (--paren_cnt < 0)
	 {
	    compile_error("extra ')'") ;
	    paren_cnt = 0 ;
	    goto reswitch ;
	 }

	 ct_ret(RPAREN) ;

      case SC_LBOX:
	 ct_ret(LBOX) ;

      case SC_RBOX:
	 ct_ret(RBOX) ;

      case SC_MATCH:
	 string_buff[0] = '~' ;
	 string_buff[0] = 0 ;
	 yylval.ival = 1 ;
	 ct_ret(MATCH) ;

      case SC_EQUAL:
	 test1_ret('=', EQ, ASSIGN) ;

      case SC_NOT:		/* !  */
	 if ((c = next()) == '~')
	 {
	    string_buff[0] = '!' ;
	    string_buff[1] = '~' ;
	    string_buff[2] = 0 ;
	    yylval.ival = 0 ;
	    ct_ret(MATCH) ;
	 }
	 else if (c == '=')  ct_ret(NEQ) ;

	 un_next() ;
	 ct_ret(NOT) ;


      case SC_LT:		/* '<' */
	 if (next() == '=')  ct_ret(LTE) ;
	 else  un_next() ;

	 if (getline_flag)
	 {
	    getline_flag = 0 ;
	    ct_ret(IO_IN) ;
	 }
	 else  ct_ret(LT) ;

      case SC_GT:		/* '>' */
	 if (print_flag && paren_cnt == 0)
	 {
	    print_flag = 0 ;
	    /* there are 3 types of IO_OUT
	       -- build the error string in string_buff */
	    string_buff[0] = '>' ;
	    if (next() == '>')
	    {
	       yylval.ival = F_APPEND ;
	       string_buff[1] = '>' ;
	       string_buff[2] = 0 ;
	    }
	    else
	    {
	       un_next() ;
	       yylval.ival = F_TRUNC ;
	       string_buff[1] = 0 ;
	    }
	    return current_token = IO_OUT ;
	 }

	 test1_ret('=', GTE, GT) ;

      case SC_OR:
	 if (next() == '|')
	 {
	    eat_nl() ;
	    ct_ret(OR) ;
	 }
	 else
	 {
	    un_next() ;

	    if (print_flag && paren_cnt == 0)
	    {
	       print_flag = 0 ;
	       yylval.ival = PIPE_OUT ;
	       string_buff[0] = '|' ;
	       string_buff[1] = 0 ;
	       ct_ret(IO_OUT) ;
	    }
	    else  ct_ret(PIPE) ;
	 }

      case SC_AND:
	 if (next() == '&')
	 {
	    eat_nl() ;
	    ct_ret(AND) ;
	 }
	 else
	 {
	    un_next() ;
	    yylval.ival = '&' ;
	    ct_ret(UNEXPECTED) ;
	 }

      case SC_QMARK:
	 ct_ret(QMARK) ;

      case SC_COLON:
	 ct_ret(COLON) ;

      case SC_RBRACE:
	 if (--brace_cnt < 0)
	 {
	    compile_error("extra '}'") ;
	    eat_semi_colon() ;
	    brace_cnt = 0 ;
	    goto reswitch ;
	 }

	 if ((c = current_token) == NL || c == SEMI_COLON
	     || c == SC_FAKE_SEMI_COLON || c == RBRACE)
	 {
	    /* if the brace_cnt is zero , we've completed
	       a pattern action block. If the user insists
	       on adding a semi-colon on the same line
	       we will eat it.	Note what we do below:
	       physical law -- conservation of semi-colons */

	    if (brace_cnt == 0)	 eat_semi_colon() ;
	    eat_nl() ;
	    ct_ret(RBRACE) ;
	 }

	 /* supply missing semi-colon to statement that
	     precedes a '}' */
	 brace_cnt++ ;
	 un_next() ;
	 current_token = SC_FAKE_SEMI_COLON ;
	 return SEMI_COLON ;

      case SC_DIGIT:
      case SC_DOT:
	 {
	    double d;
	    int flag ;
	    static double double_zero = 0.0 ;
	    static double double_one = 1.0 ;

	    if ((d = collect_decimal(c, &flag)) == 0.0)
	    {
	       if (flag)  ct_ret(flag) ;
	       else  yylval.ptr = (PTR) & double_zero ;
	    }
	    else if (d == 1.0)
	    {
	       yylval.ptr = (PTR) & double_one ;
	    }
	    else
	    {
	       yylval.ptr = (PTR) ZMALLOC(double) ;
	       *(double *) yylval.ptr = d ;
	    }
	    ct_ret(DOUBLE) ;
	 }

      case SC_DOLLAR:		/* '$' */
	 {
	    double d;
	    int flag ;

	    while (scan_code[c = next()] == SC_SPACE) ;
	    if (scan_code[c] != SC_DIGIT &&
		scan_code[c] != SC_DOT)
	    {
	       un_next() ;
	       ct_ret(DOLLAR) ;
	    }

	    /* compute field address at compile time */
	    if ((d = collect_decimal(c, &flag)) == 0.0)
	    {
	       if (flag)  ct_ret(flag) ; /* an error */
	       else  yylval.cp = &field[0] ;
	    }
	    else
	    {
	       int ival = d_to_int(d) ;
	       double dval = (double) ival ;
	       if (dval != d) {
		  compile_error(
		     "$%g is an invalid field index", d) ;
		  /* set it to something valid so error does not propagate,
		     it will not be executed or da'ed  */
		  ival = 0 ;
               }
	       yylval.cp = field_ptr(ival) ;
	    }

	    ct_ret(FIELD) ;
	 }

      case SC_DQUOTE:
	 return current_token = collect_string() ;

      case SC_IDCHAR:		/* collect an identifier */
	 {
	    unsigned char *p =
	    (unsigned char *) string_buff + 1 ;
	    SYMTAB *stp ;

	    string_buff[0] = c ;

	    while (
		     (c = scan_code[*p++ = next()]) == SC_IDCHAR ||
		     c == SC_DIGIT) ;

	    un_next() ;
	    *--p = 0 ;

	    switch ((stp = find(string_buff))->type)
	    {
	       case ST_NONE:
		  /* check for function call before defined */
		  if (next() == '(')
		  {
		     stp->type = ST_FUNCT ;
		     stp->stval.fbp = (FBLOCK *)
			zmalloc(sizeof(FBLOCK)) ;
		     stp->stval.fbp->name = stp->name ;
		     stp->stval.fbp->code = (INST *) 0 ;
		     yylval.fbp = stp->stval.fbp ;
		     current_token = FUNCT_ID ;
		  }
		  else
		  {
		     yylval.stp = stp ;
		     current_token =
			current_token == DOLLAR ? D_ID : ID ;
		  }
		  un_next() ;
		  break ;

	       case ST_NR:
		  NR_flag = 1 ;
		  stp->type = ST_VAR ;
		  /* fall thru */

	       case ST_VAR:
	       case ST_ARRAY:
	       case ST_LOCAL_NONE:
	       case ST_LOCAL_VAR:
	       case ST_LOCAL_ARRAY:

		  yylval.stp = stp ;
		  current_token =
		     current_token == DOLLAR ? D_ID : ID ;
		  break ;

	       case ST_ENV:
		  stp->type = ST_ARRAY ;
		  stp->stval.array = new_ARRAY() ;
		  load_environ(stp->stval.array) ;
		  yylval.stp = stp ;
		  current_token =
		     current_token == DOLLAR ? D_ID : ID ;
		  break ;

	       case ST_FUNCT:
		  yylval.fbp = stp->stval.fbp ;
		  current_token = FUNCT_ID ;
		  break ;

	       case ST_KEYWORD:
		  current_token = stp->stval.kw ;
		  break ;

	       case ST_BUILTIN:
		  yylval.bip = stp->stval.bip ;
		  current_token = BUILTIN ;
		  break ;

	       case ST_FIELD:
		  yylval.cp = stp->stval.cp ;
		  current_token = FIELD ;
		  break ;

	       default:
		  bozo("find returned bad st type") ;
	    }
	    return current_token ;
	 }


      case SC_UNEXPECTED:
	 yylval.ival = c & 0xff ;
	 ct_ret(UNEXPECTED) ;
   }
   return 0 ;			 /* never get here make lint happy */
}

/* collect a decimal constant in temp_buff.
   Return the value and error conditions by reference */

static double
collect_decimal(int c, int* flag)
{
   register unsigned char *p = (unsigned char *) string_buff + 1 ;
   unsigned char *endp ;
   double d;

   *flag = 0 ;
   string_buff[0] = c ;

   if (c == '.')
   {
      if (scan_code[*p++ = next()] != SC_DIGIT)
      {
	 *flag = UNEXPECTED ;
	 yylval.ival = '.' ;
	 return 0.0 ;
      }
   }
   else
   {
      while (scan_code[*p++ = next()] == SC_DIGIT) ;
      if (p[-1] != '.')
      {
	 un_next() ;
	 p-- ;
      }
   }
   /* get rest of digits after decimal point */
   while (scan_code[*p++ = next()] == SC_DIGIT) ;

   /* check for exponent */
   if (p[-1] != 'e' && p[-1] != 'E')
   {
      un_next() ;
      *--p = 0 ;
   }
   else	 /* get the exponent */
   {
      if (scan_code[*p = next()] != SC_DIGIT &&
	  *p != '-' && *p != '+')
      {
	 *++p = 0 ;
	 *flag = BAD_DECIMAL ;
	 return 0.0 ;
      }
      else  /* get the rest of the exponent */
      {
	 p++ ;
	 while (scan_code[*p++ = next()] == SC_DIGIT) ;
	 un_next() ;
	 *--p = 0 ;
      }
   }

   errno = 0 ;			 /* check for overflow/underflow */
   d = strtod(string_buff, (char **) &endp) ;

#ifndef	 STRTOD_UNDERFLOW_ON_ZERO_BUG
   if (errno)  compile_error("%s : decimal %sflow", string_buff,
		    d == 0.0 ? "under" : "over") ;
#else /* ! sun4 bug */
   if (errno && d != 0.0)
      compile_error("%s : decimal overflow", string_buff) ;
#endif

   if (endp < p)
   {
      *flag = BAD_DECIMAL ;
      return 0.0 ;
   }
   return d ;
}

/*----------  process escape characters ---------------*/

static char hex_val['f' - 'A' + 1] =
{
   10, 11, 12, 13, 14, 15, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0,
   10, 11, 12, 13, 14, 15} ;

#define isoctal(x)  ((x)>='0'&&(x)<='7')

#define	 hex_value(x)	hex_val[(x)-'A']

#define ishex(x) (scan_code[x] == SC_DIGIT ||\
		  ('A' <= (x) && (x) <= 'f' && hex_value(x)))


/* process one , two or three octal digits
   moving a pointer forward by reference */
static int
octal(char** start_p)
{
   register char *p = *start_p ;
   register unsigned x ;

   x = *p++ - '0' ;
   if (isoctal(*p))
   {
      x = (x << 3) + *p++ - '0' ;
      if (isoctal(*p))	x = (x << 3) + *p++ - '0' ;
   }
   *start_p = p ;
   return x & 0xff ;
}

/* process one or two hex digits
   moving a pointer forward by reference */

static int
hex(char** start_p)
{
   register unsigned char *p = (unsigned char *) *start_p ;
   register unsigned x ;
   unsigned t ;

   if (scan_code[*p] == SC_DIGIT)  x = *p++ - '0' ;
   else	 x = hex_value(*p++) ;

   if (scan_code[*p] == SC_DIGIT)  x = (x << 4) + *p++ - '0' ;
   else if ('A' <= *p && *p <= 'f' && (t = hex_value(*p)))
   {
      x = (x << 4) + t ;
      p++ ;
   }

   *start_p = (char *) p ;
   return x ;
}

#define	 ET_END	    9

static struct
{
   char in, out ;
}
escape_test[ET_END + 1] = {
   { 'n', '\n'},
   { 't', '\t'},
   { 'f', '\f'},
   { 'b', '\b'},
   { 'r', '\r'},
   { 'a', '\07'},
   { 'v', '\013'},
   { '\\', '\\'},
   { '\"', '\"'},
   {0, 0}
} ;


/* process the escape characters in a string, in place
   return the new length */

size_t rm_escape(char* s)
{
   char *p, *q ;
   char *t ;
   int i ;

   q = p = s ;

   while (*p)
   {
      if (*p == '\\') {
         p++ ;
	 escape_test[ET_END].in = *p ; /* sentinal */
	 i = 0 ;
	 while (escape_test[i].in != *p)  i++ ;

	 if (i != ET_END)	/* in table */
	 {
	    p++ ;
	    *q++ = escape_test[i].out ;
	 }
	 else if (isoctal(*p))
	 {
	    t = p ;
	    *q++ = octal(&t) ;
	    p = t ;
	 }
	 else if (*p == 'x' && ishex(*(unsigned char *) (p + 1)))
	 {
	    t = p + 1 ;
	    *q++ = hex(&t) ;
	    p = t ;
	 }
	 else if (*p == 0)	/* can only happen with command line assign */
	    *q++ = '\\' ;
	 else  /* not an escape sequence */
	 {
	    *q++ = '\\' ;
	    *q++ = *p++ ;
	 }
      }
      else  *q++ = *p++ ;
   }

   *q = 0 ;
   return q-s ;
}

static int
collect_string(void)
{
   register unsigned char *p = (unsigned char *) string_buff ;
   int c ;
   int e_flag = 0 ;		 /* on if have an escape char */

   while (1) {
      switch (scan_code[*p++ = next()])
      {
	 case SC_DQUOTE:	/* done */
	    *--p = 0 ;
	    goto out ;

	 case SC_NL:
	    p[-1] = 0 ;
	    /* fall thru */

	 case 0:		/* unterminated string */
	    compile_error(
			    "runaway string constant \"%.10s ...",
			    string_buff, token_lineno) ;
	    mawk_exit(2) ;

	 case SC_ESCAPE:
	    if ((c = next()) == '\n')
	    {
	       p-- ;
	       lineno++ ;
	    }
	    else if (c == 0)  un_next() ;
	    else
	    {
	       *p++ = c ;
	       e_flag = 1 ;
	    }

	    break ;

	 default:
	    break ;
      }
  }

out:
    {
	size_t len = (char*) p - string_buff ;
        if (e_flag) {
            len = rm_escape(string_buff) ;
	}
        yylval.ptr = new_STRING2(string_buff, len) ;
    }
    return STRING_ ;
}


/* bad character class in an RE */
static void box_error(unsigned char* p)
{
    *p = 0 ;
    compile_error(
	"invalid character class in regular expr /%.10s ..",
	string_buff, token_lineno) ;
    mawk_exit(2) ;
}

/* seen [:   collect to :] */
static unsigned char* collect_named_box(unsigned char* p)
{
    unsigned c ;
    while(1) {
        if (p >(unsigned char*) string_buff_end - 2) {
	    p = (unsigned char*) enlarge_string_buff((char*)p) ;
	}
	switch(c = next()) {
	    case 0:
	    case '\n':
	        box_error(p) ;
	    case '\\':
	        c = next() ;
		if (c != '\n') box_error(p) ;
		break ;

	    case ':':
	        c = next() ;
		if (c == ']') {
		    p[0] = ':' ;
		    p[1] = ']' ;
		    return p+2 ;
		}
		else box_error(p) ;

	    default:
	        *p++ = c ;
		break ;
	}
    }
}

/* seen [ collect to ] */
unsigned char* collect_RE_box(unsigned char* p)
{
    unsigned c ;
    unsigned char* end = (unsigned char*) string_buff_end ;

    /* [].. and [^].. is ok */
    if (p+2 >= end) {
        p = (unsigned char*) enlarge_string_buff((char*) p) ;
	end = (unsigned char*) string_buff_end ;
    }
    c = next() ;
    if (c == ']') {
        *p++ = ']' ;
	c = next() ;
    }
    else if (c == '^') {
        *p++ = '^' ;
	c = next() ;
	if (c == ']') {
	    *p++ = ']' ;
	    c = next() ;
	}
    }

    /* c is ready to be tested */
    while(1) {
        if (p+2 >= end) {
            p = (unsigned char*) enlarge_string_buff((char*) p) ;
	    end = (unsigned char*) string_buff_end ;
        }
        switch(c) {
	    case 0:
	    case '\n':
	        box_error(p) ;
	    case ']':
	        /* done */
		*p++ = ']' ;
		return p ;

	    case '\\':
	        c = next() ;
		if (c == 0) {
		    box_error(p) ;
		}
		if (c == '\n') {
		    /* "\\\n" is removed */
		}
		else {
		    p[0] = '\\' ;
		    p[1] = c ;
		    p += 2 ;
		}
		break ;

	    case '[':
	        c = next() ;
		if (c == ':') {
		    p[0] = '[' ;
		    p[1] = ':' ;
		    p = collect_named_box(p+2) ;
		}
		else {
		    un_next() ;
		    *p++ = '[' ;
		}
		break ;

	    default:
	        *p++ = c ;
		break ;
	}
	c = next() ;
    }
}

static void runaway_RE(unsigned char* p)
{
    *p = 0 ;
    compile_error("runaway regular expression /%.10s ...",
			    string_buff, token_lineno) ;
     mawk_exit(2) ;
}

/* seen /  collect to terminating /

    no attempt to interpret as it will be passed to REcompile()

    however this needs to understand [..] as / is ok inside
    a character class.  This was a bug in mawk 1.3.3

    also removes  "\\\n" (escaped nl) so it doesn't go to REcompile()
*/
static int
collect_RE(void)
{
    unsigned char *p = (unsigned char *) string_buff ;
    unsigned  c ;
    STRING *sval ;

    while (1) {
        if (p >= (unsigned char*) string_buff_end - 2) {
	    p = (unsigned char*) enlarge_string_buff((char*)p) ;
	}
	c = next() ;
	switch(c) {
	    case '/':  /* done */
	        *p = 0 ;
                sval = new_STRING(string_buff) ;
                yylval.ptr = re_compile(sval) ;
                free_STRING(sval) ;
                return RE ;

	    case 0:
	    case '\n':
	        runaway_RE(p) ;

	    case '\\':
	        c = next() ;
		if (c == 0) {
		    runaway_RE(p) ;
		}
		if (c == '\n') {
		     /* "\\\n" is removed */
		}
		else {
		    p[0] = '\\' ;
		    p[1] = c ;
		    p += 2 ;
		}
		break ;

	    case '[':
		*p++ = c ;
	        p = collect_RE_box(p) ;
		break ;

	    default:
	        *p++ = c ;
		break ;
	}
    }
}


