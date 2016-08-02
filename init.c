
/********************************************
init.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/




/* init.c */
#include <signal.h>
#include "mawk.h"
#include "code.h"
#include "memory.h"
#include "symtype.h"
#include "init.h"
#include "bi_vars.h"
#include "field.h"


static void  process_cmdline(int, char **) ;
static void  set_ARGV(int, char **, int) ;
static void  bad_option(const char *) ;
static void  no_program(void) ;
static void  print_help(void) ;
static void  catch_fpe(int) ;

extern void  print_version(void) ;
extern int  is_cmdline_assign(char *) ;

#if  MSDOS
void  stdout_init(void) ;
#if  HAVE_REARGV
void  reargv(int *, char ***) ;
#endif
#endif

const char *progname ;
int interactive_flag = 0 ;

#ifndef	 SET_PROGNAME
#define	 SET_PROGNAME() \
   {char *p = strrchr(argv[0],'/') ;\
    progname = p ? p+1 : argv[0] ; }
#endif


static void
buffer_init(void)
{
    string_buff = (char*) emalloc(SPRINTF_LIMIT) ;
    string_buff_end = string_buff + SPRINTF_LIMIT ;
}


void
initialize(int argc, char** argv)
{

   SET_PROGNAME() ;

   buffer_init() ;
   bi_vars_init() ;		 /* load the builtin variables */
   bi_funct_init() ;		 /* load the builtin functions */
   kw_init() ;			 /* load the keywords */
   field_init() ;

#if   MSDOS
   {
      char *p = getenv("MAWKBINMODE") ;

      if (p)  set_binmode(atoi(p)) ;
   }
#endif


   process_cmdline(argc, argv) ;

   code_init() ;
   signal(SIGFPE, catch_fpe) ;
   set_stdoutput() ;

#if  MSDOS
   stdout_init() ;
#endif
}

int dump_code_flag ;		 /* if on dump internal code */
int posix_space_flag ;

#ifdef	 DEBUG
int dump_RE = 1 ;			 /* if on dump compiled REs  */
#endif

static void
bad_option(const char* s)
{
   errmsg(0, "not an option: %s", s) ; mawk_exit(2) ;
}

static void
no_program(void)
{
   mawk_exit(0) ;
}

#define optarg optarg_			/* remove conflict with optarg in <getopt.h> */


static void
process_cmdline(int argc, char** argv)
{
   int i, nextarg ;
   char *optarg ;
   PFILE dummy ;		 /* starts linked list of filenames */
   PFILE *tail = &dummy ;

   for (i = 1; i < argc && argv[i][0] == '-'; i = nextarg)
   {
      if (argv[i][1] == 0)	/* -  alone */
      {
	 if (!pfile_name) no_program() ;
	 break ;		 /* the for loop */
      }
      /* safe to look at argv[i][2] */

      /* handle two arguments that are now widely supported in most free and open-source software  */
      if (strncmp(argv[i], "--v", 3) == 0 ||
          strncmp(argv[i], "--V", 3) == 0)
      {
          /* --version  */
	  print_version();
	  /* no return */
      }

      if (strncmp(argv[i], "--h", 3) == 0 ||
          strncmp(argv[i], "--H", 3) == 0)
      {
          /* --help */
	  print_help();
	  /* no return */
      }

      if (argv[i][2] == 0)
      {
	 if (i == argc - 1 && argv[i][1] != '-')
	 {
	    if (strchr("WFvf", argv[i][1]))
	    {
	       errmsg(0, "option %s lacks argument", argv[i]) ;
	       mawk_exit(2) ;
	    }
	    bad_option(argv[i]) ;
	 }

	 optarg = argv[i + 1] ;
	 nextarg = i + 2 ;
      }
      else  /* argument glued to option */
      {
	 optarg = &argv[i][2] ;
	 nextarg = i + 1 ;
      }

      switch (argv[i][1])
      {
	 case 'W':

	    if (optarg[0] >= 'a' && optarg[0] <= 'z')
	       optarg[0] += 'A' - 'a' ;
	    if (optarg[0] == 'V')  print_version() ;
	    if (optarg[0] == 'H')  print_help() ;
	    else if (optarg[0] == 'D')
	    {
	       dump_code_flag = 1 ;
	    }
	    else if (optarg[0] == 'S')
	    {
	       /* obsolete, silently ignore */
	    }
#if  MSDOS
	    else if (optarg[0] == 'B')
	    {
	       char *p = strchr(optarg, '=') ;
	       int x = p ? atoi(p + 1) : 0 ;

	       set_binmode(x) ;
	    }
#endif
	    else if (optarg[0] == 'P')
	    {
	       posix_space_flag = 1 ;
	       posix_repl_scan_flag = 1 ;
	    }
	    else if (optarg[0] == 'E')
	    {
	       if ( pfile_name )
	       {
		  errmsg(0, "-W exec is incompatible with -f") ;
		  mawk_exit(2) ;
	       }
	       else if ( nextarg == argc ) no_program() ;

	       pfile_name = argv[nextarg] ;
	       i = nextarg + 1 ;
	       goto no_more_opts ;
	    }
	    else if (optarg[0] == 'I')
	    {
	       interactive_flag = 1 ;
	       setbuf(stdout,(char*)0) ;
	    }
	    else  errmsg(0, "vacuous option: -W %s", optarg) ;


	    break ;

	 case 'v':
	    if (!is_cmdline_assign(optarg))
	    {
	       errmsg(0, "improper assignment: -v %s", optarg) ;
	       mawk_exit(2) ;
	    }
	    break ;

	 case 'F':

	    {
	        /* recognize escape sequences */
	        size_t len = rm_escape(optarg) ;
	        cell_destroy(FS) ;
	        FS->type = C_STRING ;
	        FS->ptr =  new_STRING2(optarg,len) ;
	        cast_for_split(cellcpy(&fs_shadow, FS)) ;
	    }
	    break ;

	 case '-':
	    if (argv[i][2] != 0)  bad_option(argv[i]) ;
	    i++ ;
	    goto no_more_opts ;

	 case 'f':
	    /* first file goes in pfile_name ; any more go
	       on a list */
	    if (!pfile_name)  pfile_name = optarg ;
	    else
	    {
	       tail = tail->link = ZMALLOC(PFILE) ;
	       tail->fname = optarg ;
	    }
	    break ;

	 default:
	    bad_option(argv[i]) ;
      }
   }

 no_more_opts:

   tail->link = (PFILE *) 0 ;
   pfile_list = dummy.link ;

   if (pfile_name)
   {
      set_ARGV(argc, argv, i) ;
      scan_init((char *) 0) ;
   }
   else	 /* program on command line */
   {
      if (i == argc)  no_program() ;
      set_ARGV(argc, argv, i + 1) ;

#if  MSDOS && ! HAVE_REARGV	/* reversed quotes */
      {
	 char *p ;

	 for (p = argv[i]; *p; p++)
	    if (*p == '\'')  *p = '\"' ;
      }
#endif
      scan_init(argv[i]) ;
/* #endif  */
   }
}


static void
set_ARGV(int argc, char** argv, int i)
   	 /* argv[i] = ARGV[i] */
{
   SYMTAB *st_p ;
   CELL argi ;
   register CELL *cp ;

   st_p = insert("ARGV") ;
   st_p->type = ST_ARRAY ;
   Argv = st_p->stval.array = new_ARRAY() ;
   argi.type = C_DOUBLE ;
   argi.dval = 0.0 ;
   cp = array_find(st_p->stval.array, &argi, CREATE) ;
   cp->type = C_STRING ;
   cp->ptr = (PTR) new_STRING(argv[0]) ;

   /* ARGV[0] is set, do the rest
     The type of ARGV[1] ... should be C_MBSTRN
     because the user might enter numbers from the command line */

   for (argi.dval = 1.0; i < argc; i++, argi.dval += 1.0)
   {
      cp = array_find(st_p->stval.array, &argi, CREATE) ;
      cp->type = C_MBSTRN ;
      cp->ptr = (PTR) new_STRING(argv[i]) ;
   }
   ARGC->type = C_DOUBLE ;
   ARGC->dval = argi.dval ;
}


/*----- ENVIRON ----------*/

void
load_environ(ARRAY ENV)
{
   CELL c ;
   extern char **environ ;
   register char **p = environ ; /* walks environ */
   char *s ;			 /* looks for the '=' */
   CELL *cp ;			 /* pts at ENV[&c] */

   c.type = C_STRING ;

   while (*p)
   {
      if ((s = strchr(*p, '=')))	/* shouldn't fail */
      {
	 int len = s - *p ;
	 c.ptr = (PTR) new_STRING0(len) ;
	 memcpy(string(&c)->str, *p, len) ;
	 s++ ;

	 cp = array_find(ENV, &c, CREATE) ;
	 cp->type = C_MBSTRN ;
	 cp->ptr = (PTR) new_STRING(s) ;

	 free_STRING(string(&c)) ;
      }
      p++ ;
   }
}


/*  FPE exceptions

mawk 1.x.x  in 199x tried hard to catch fpe when necessary,
      have optimal settings and
      maximize information passed to user

      What to do in 201x?

      Odds of fpe in an awk program is verrrrrrrrrry small
      probablity it generates an INF or NAN is at least .9
          probably higher

    Since an fpe is unlikely, mawk 2.x.x will deal with 
    it very simply.

*/

static void catch_fpe(int x)
{
    if (x == SIGFPE) {
        rt_error("floating point arithmetic exception") ;
    }
    else bozo("catch_fpe") ;
    mawk_exit(2) ;
}


/*   HELP  */

/* to change this
   edit help.txt
   run $ mawk -f help.awk help.txt > out
   with an editor put out into help[]
*/

static const char* const help[] = {
"Usage:",
"\tmawk [--help]",
"\tmawk [--version]",
"\tmawk [-W option] [-F value] [-v var=value] [--] 'program text' file(s)",
"\tmawk [-W option] [-F value] [-v var=value] [-f program-file] [--] file(s)",
"",
"Generic options:",
"",
"\t--help         displays this help message and exits 0.",
"",
"\t--version      displays mawk version and exits 0.",
"",
"Normal awk-implementation options:",
"",
"\t-F value       sets the field separator, FS, to value.",
"",
"\t-f file        program text is read from file instead of from the",
"\t               command line.  Multiple -f options are allowed.",
"",
"\t-v var=value   assigns value to program variable var.",
"",
"\t--             indicates the unambiguous end of options.",
"",
"The above options are available with any IEEE 1003 POSIX-compatible",
"implementation of AWK.  mawk-specific options are prefaced with -W.",
"",
"\t-W dump        writes an assembler-like listing of the internal",
"\t               representation of the program to stdout and exits 0 (on",
"\t               successful compilation).",
"",
"\t-W exec file   program text is read from file and this is the last",
"\t               option.  It is useful on systems that support the",
"\t               #! \"magic number\" convention for executable scripts.",
"",
"\t-W help        displays this help message and exits 0.",
"",
"\t-W interactive sets unbuffered writes to stdout and line buffered reads",
"\t               from stdin.  Records from stdin are lines regardless of",
"\t               the value of RS.",
"",
"\t-W posix       forces mawk not to consider '\\n' to be space and \\\\",
"\t               is always \\ on the second scan of a replacement string.",
"",
"\t-W version     displays mawk version and exits 0.",
"",
"Just the first letter for each option is enough.  For example, -Wv, -W v ",
"and --v are equivalent to -W version or --version.",
0 } ;

static void
print_help()
{
    int i = 0 ;
    while(help[i]) {
        printf("%s\n", help[i]) ;
	i++ ;
    }
    mawk_exit(0) ;
}
