
/********************************************
files.c
copyright 1991-94,2014-2016  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/* files.c */

#include <unistd.h>

#include "mawk.h"
#include "files.h"
#include "memory.h"
#include "fin.h"

static FILE * tfopen(const char *, const char *) ;
static void  efflush(FILE*) ;


/* We store dynamically created files on a linked linear
   list with move to the front (big surprise)  */

typedef struct file
{
    struct file *link ;
    STRING *name ;
    int type ;
    PTR ptr ;			 /* FIN*   or  FILE*   */
}
FILE_NODE ;

static FILE_NODE *file_list ;

static  FILE_NODE* std_err ;
static  FILE_NODE* std_out ;


/* find a file on file_list 
   if not in the list open it
   return is a FIN* if input file
               FILE* if output file  

   return is 0 on failure to open input
   error exit 2 on failure to open output
*/
PTR
file_find(STRING* sval, int type)
{
   FILE_NODE *p = file_list ;
   FILE_NODE *q = (FILE_NODE *) 0 ;
   const char *name = sval->str ;
   const char *ostr ;

   while (1)
   {
      if (!p)
      {
	 /* open a new one */
	 p = ZMALLOC(FILE_NODE) ;

	 switch (p->type = type)
	 {
	    case F_TRUNC:
#if MSDOS
	       ostr = (binmode() & 2) ? "wb" : "w" ;
#else
	       ostr = "w" ;
#endif
	       if (!(p->ptr = (PTR) tfopen(name, ostr)))
		  goto out_failure ;
	       break ;

	    case F_APPEND:
#if MSDOS
	       ostr = (binmode() & 2) ? "ab" : "a" ;
#else
	       ostr = "a" ;
#endif
	       if (!(p->ptr = (PTR) tfopen(name, ostr)))
		  goto out_failure ;
	       break ;

	    case F_IN:
	       if (!(p->ptr = (PTR) FINopen(name, 0)))
	       {
		  zfree(p, sizeof(FILE_NODE)) ;
		  return (PTR) 0 ;
	       }
	       break ;

	    case PIPE_OUT:
	        {
		    FILE* outp = popen(name,"w") ;
		    if (outp == 0) goto out_failure ;
		    p->ptr = outp ;
		    flush_all_output() ;
	        }
		break ;

	    case PIPE_IN:
	        {
		    FIN* fin = FINpopen(name) ;
		    if (!fin) {
		        zfree(p, sizeof(FILE_NODE)) ;
			return (PTR) 0 ;
		    }
		    p->ptr = fin ;
		}
		break ;

#ifdef	DEBUG
	    default:
	       bozo("bad file type") ;
#endif
	 }
	 /* successful open */
	 p->name = sval ;
	 sval->ref_cnt++ ;
	 break ;		 /* while loop */
      }

      /* search is by name and type */
      if (strcmp(name, p->name->str) == 0 &&
	  (p->type == type ||
      /* no distinction between F_APPEND and F_TRUNC here */
	   (p->type >= F_APPEND && type >= F_APPEND)))

      {
	 /* found */
	 if (!q)		/*at front of list */
	    return p->ptr ;
	 /* delete from list for move to front */
	 q->link = p->link ;
	 break ;		 /* while loop */
      }

      q = p ; p = p->link ;
   }				/* end while loop */

   /* put p at the front of the list */
   p->link = file_list ;
   file_list = p ;
   return p->ptr ;

out_failure:
   errmsg(errno, "cannot open \"%s\" for output", name) ;
   mawk_exit(2) ;
   /* not reached, shutup -Wall */
   return 0 ;
}


/* Close a file and delete it's node from the file_list.
   Walk the whole list, in case a name has two nodes,
   e.g. < "/dev/tty" and > "/dev/tty"

   This only gets called from bi_close, i.e. the
   user has explicitly called close

   We will ignore user's request to close /dev/stderr so
   that stderr is valid for our error messages
*/

int
file_close(STRING* sval)
{
   FILE_NODE dummy ;
   register FILE_NODE *p ;
   FILE_NODE *q = &dummy ;	 /* trails p */
   FILE_NODE *hold ;
   char *name = sval->str ;
   int retval = -1 ;
   int error_exit = 0 ;
   int not_found = 1 ;

   if (strcmp(name, "/dev/stderr") == 0) {
       fflush(stderr) ;
       return 0 ;
   }

   dummy.link = p = file_list ;
   while (p)
   {
      if (strcmp(name, p->name->str) == 0)
      {
	 not_found = 0 ;
	 switch (p->type)
	 {
	    case F_TRUNC:
	    case F_APPEND:
	       {
	          FILE* fp = (FILE*) p->ptr ;
	          if (fclose(fp) == -1) {
	              errmsg(errno, "close error on file %s", name) ;
		      error_exit = 1 ;
		  }
		  else {
		      retval = 0 ;
		  }
	       }
	       break ;

	    case PIPE_OUT:
	        retval = pclose((FILE*) p->ptr) ;
		if (retval == -1) {
	            errmsg(errno, "close error on pipe | %s", name) ;
		    error_exit = 1 ;
	        }
	        break ;

	    case F_IN:
	    case PIPE_IN:
	       retval = FINclose((FIN *) p->ptr) ;
	       break ;

	 }

	 free_STRING(p->name) ;
	 hold = p ;
	 q->link = p = p->link ;
	 ZFREE(hold) ;
      }
      else
      {
	 q = p ;
	 p = p->link ;
      }
   }

   file_list = dummy.link ;
   if (error_exit) mawk_exit(2) ;
   if (not_found) {
       errmsg(0, "close on \"%s\" failed (not an open file)", name) ;
   }
   return retval ;
}

/* report an output error and close the file */
void write_error(FILE* fp)
{
    FILE_NODE* p = file_list ;
    FILE_NODE* q = 0 ;
    while(p) {
        if (p->ptr == (void*) fp) {
	    if (q == 0) {
	        file_list = file_list->link ;
	    }
	    else {
	        q->link = p->link ;
	    }
	    break ; /* while */
	}
	else {
	    q = p ;
	    p = p->link ;
	}
    }
    fclose(fp) ;
    if (p)  {  /* this test should never fail */
        const char* type = p->type == PIPE_OUT ? "pipe | " : "file" ;
	errmsg(errno, "write error to %s %s", type, p->name->str) ;
	zfree(p, sizeof(FILE_NODE)) ;
    }
}


/*
find an output file with name == sval and fflush it
*/

int
file_flush(STRING* sval)
{
   int ret = -1 ;
   register FILE_NODE *p = file_list ;
   unsigned len = sval->len ;
   char *name = sval->str ;

   if (len==0)
   {
      /* for consistency with gawk */
      flush_all_output() ;
      return 0 ;
   }

   while( p )
   {
      if ( IS_OUTPUT(p->type) &&
	   len == p->name->len &&
	   strcmp(name,p->name->str) == 0 )
      {
	 ret = 0 ;
	 efflush((FILE*)p->ptr) ;
         /* it's possible for a command and a file to have the same
	    name -- so keep looking */
      }
      p = p->link ;
   }
   if (ret == -1) {
       errmsg(0, "flush error on %s (not an open file)", name) ;
   }
   return ret ;
}

void
flush_all_output(void)
{
   FILE_NODE *p ;

   for(p=file_list; p ; p = p->link)
      if (IS_OUTPUT(p->type)) efflush((FILE*)p->ptr) ;
}

static void
efflush(FILE* fp)
{
   if (fflush(fp) < 0)
   {
      write_error(fp) ;
      mawk_exit(2) ;
   }
}

/* we close output to check for write errors that might not have shown up
   yet because of buffering */

int
close_all_output(void)
{
   FILE_NODE *p = file_list ;
   int ret = 0 ;

   while (p)
   {
      if (IS_OUTPUT(p->type) && p != std_err)
      {
          int r ;
	  FILE* f = (FILE*) p->ptr ;
	  if (p->type == PIPE_OUT) {
	      r = pclose(f) ;
	  }
	  else {
	      r = fclose(f) ;
	  }
	  if (r == -1) {
	      ret = -1 ;
	      errmsg(errno, "close error on %s %s",
	          p->type == PIPE_OUT ? "pipe |" : "file",
		  p->name->str) ;
	  }
      }
      p = p->link ;
   }
   return ret ;
}


/* put stdout and stderr in file_list */
void
set_stdoutput(void)
{
   FILE_NODE *p, *q ;

   std_out = p = ZMALLOC(FILE_NODE) ;
   p->link = (FILE_NODE*) 0 ;
   p->type = F_TRUNC ;
   p->name = new_STRING("/dev/stdout") ;
   p->ptr = (PTR) stdout ;
   std_err = q = ZMALLOC(FILE_NODE);
   q->link = p ;
   q->type = F_TRUNC ;
   q->name = new_STRING("/dev/stderr") ;
   q->ptr = (PTR) stderr ;
   file_list = q ;
}

/* fopen() but no buffering to ttys */
static FILE *
tfopen(const char* name, const char* mode)
{
   FILE *retval = fopen(name, mode) ;

   if (retval)
   {
      if (isatty(fileno(retval)))  setbuf(retval, (char *) 0) ;
      else
      {
#ifdef MSDOS
	 enlarge_output_buffer(retval) ;
#endif
      }
   }
   return retval ;
}

#ifdef  MSDOS
void
enlarge_output_buffer(fp)
   FILE *fp ;
{
   if (setvbuf(fp, (char *) 0, _IOFBF, BUFFSZ) < 0)
   {
      errmsg(errno, "setvbuf failed on fileno %d", fileno(fp)) ;
      mawk_exit(2) ;
   }
}

void
stdout_init(void)
{
   if (!isatty(1))  enlarge_output_buffer(stdout) ;
   if (binmode() & 2)
   {
      setmode(1,O_BINARY) ; setmode(2,O_BINARY) ;
   }
}
#endif /* MSDOS */
