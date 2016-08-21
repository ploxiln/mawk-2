
/********************************************
fin.c
copyright 1991,1992,2014-2016  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/* fin.c */

#include "mawk.h"
#include "fin.h"
#include "memory.h"
#include "bi_vars.h"
#include "field.h"
#include "symtype.h"
#include "scan.h"
#include <fcntl.h>

/* This file handles input files.  Opening, closing,
   buffering and (most important) splitting files into
   records, FINgets().
*/

static char * enlarge_fin_buffer(FIN *) ;
static void  set_main_to_stdin(void) ;
int  is_cmdline_assign(char *) ; /* also used by init */ 

/* convert file-descriptor to FIN*.
   It's the main stream if main_flag is set
*/
FIN *
FINdopen(int fd, int main_flag)
{
   register FIN *fin = ZMALLOC(FIN) ;

   fin->fd = fd ;
   fin->flags = main_flag ? (MAIN_FLAG | START_FLAG) : START_FLAG ;
   fin->end = fin->start = fin->buff = (char *) emalloc(FINBUFFSZ) ;
   fin->buffsz = FINBUFFSZ ;

   if ((isatty(fd) && rs_shadow.type == SEP_CHAR && rs_shadow.c == '\n')
       || (interactive_flag && fd == 0) )
   {
      /* interactive, i.e., line buffer this file */
      if (fd == 0)  fin->fp = stdin ;
      else if (!(fin->fp = fdopen(fd, "r")))
      {
	 errmsg(errno, "fdopen failed") ; mawk_exit(2) ;
      }
      fin->fd = -1 ;  /* marks it interactive */
   }
   else	 fin->fp = (FILE *) 0 ;

   return fin ;
}

/* open a FIN* by filename.
   It's the main stream if main_flag is set.
   Recognizes "-" as stdin.
   Recognizes "/dev/stdin" as stdin.
*/

FIN *
FINopen(const char* filename, int main_flag)
{
   int fd ;
   int oflag = O_RDONLY ;

#if  MSDOS
   int bm = binmode() & 1 ;
   if (bm)  oflag |= O_BINARY ;
#endif

   if ((filename[0] == '-' && filename[1] == 0) ||
       strcmp(filename,"/dev/stdin") == 0 )
   {
#if  MSDOS
      if (bm)  setmode(0, O_BINARY) ;
#endif
      return FINdopen(0, main_flag) ;
   }

   if ((fd = open(filename, oflag, 0)) == -1)
      return (FIN *) 0 ;
   else
      return FINdopen(fd, main_flag) ;
}

FIN*
FINpopen(const char* command)
{
    FIN* ret = 0 ;
    FILE* fp = popen(command, "r") ;
    if (fp) {
        ret = FINdopen(fileno(fp), 0) ;
	ret->fp = fp ;
    }
    return ret ;
}


/* frees the buffer and fd, but leaves FIN structure until
   the user calls close() 

   This only gets called from getline and FINclose

   we don't really close stdin/0 so association to "-" always works
*/

void
FINsemi_close(FIN* fin)
{
    static char dead = 0 ;

    if (fin->buff != &dead) {
        free(fin->buff) ;
	fin->buff = &dead ;
	/* belt and suspenders */
	fin->start = fin->end = fin->buff ;
	fin->flags = EOF_FLAG ;

	fin->close_val = 0 ;

	if (fin->fd == -1) {
	    if (fin->fp != stdin) {
	        fin->close_val = fclose(fin->fp) ;
	    }
	}
	else if (fin->fp) {
	    fin->close_val = pclose(fin->fp) ;
	}
	else if (fin->fd != 0) {
	    fin->close_val = close(fin->fd) ;
	}
    }
    /* else was already semi_closed */
}

/* user called close() on input file */
int
FINclose(FIN* fin)
{
   int ret ;
   FINsemi_close(fin) ;
   ret = fin->close_val ;
   ZFREE(fin) ;
   return ret ;
}

/* return one input record as determined by RS,
   from input file (FIN)  fin
*/

char *
FINgets(FIN* fin, size_t* len_p)
{
    char* rec_start ;
    char* rec_end = 0 ;
    size_t match_len ;

restart :

    rec_start = fin->start ;

    if (fin->start >= fin->end) {
      if (fin->flags & EOF_FLAG)
      {
	 if (fin->flags & MAIN_FLAG)
	 {
	    fin = next_main(0) ;  goto restart ;
	 }
	 else
	 {
	    *len_p = 0 ; return (char *) 0 ;
	 }
      }

      if (fin->fd == -1)
      {
	 /* line buffering */
	 if (!fgets(fin->buff, BUFFSZ - 1, fin->fp))
	 {
	    fin->flags |= EOF_FLAG ;
	    fin->buff[0] = 0 ;
	    fin->end = fin->start = fin->buff ;
	    goto restart ;	 /* might be main_fin */
	 }
	 else  /* return this line */
	 {
	    /* TBD fix -- doesn't handle lines bigger than BUFFSZ-1 */
	    /* low priority */
	    /* find eol */
	    char* p = fin->buff ;

	    while (*p != '\n' && *p != 0)  p++ ;

	    *p = 0 ; *len_p = p - fin->buff ;
	    fin->end = fin->start = p ;
	    return fin->buff ;
	 }
      }
      else
      {
	 /* block buffering */
	 size_t r = fillbuff(fin->fd, fin->buff, fin->buffsz) ;
	 if (r == 0)
	 {
	    fin->flags |= EOF_FLAG ;
	    fin->end = fin->start = fin->buff ;
	    goto restart ;	 /* might be main */
	 }
	 else if (r < fin->buffsz)
	 {
	    fin->flags |= EOF_FLAG ;
	 }

	 rec_start = fin->start = fin->buff ;
	 fin->end = fin->start + r ;

	 if (fin->flags & START_FLAG)
	 {
	    fin->flags &= ~START_FLAG ;
	    if (rs_shadow.type == SEP_MLR)
	    {
	       /* trim blank lines from front of file */
	       while(rec_start < fin->end && *rec_start == '\n') rec_start++ ;
	       fin->start = rec_start ;
	       if (rec_start >= fin->end)  {
	           goto restart ;
	       }
	    }
	 }
      }
   }

retry:

   switch (rs_shadow.type)
   {
      case SEP_CHAR:
	 rec_end = (char *)memchr(rec_start, rs_shadow.c, fin->end - rec_start) ;
	 match_len = 1 ;
	 break ;

      case SEP_STR:
	 {
	     STRING* str = (STRING*) rs_shadow.ptr ;
	     rec_end = str_str(rec_start, fin->end - rec_start, str->str, str->len) ;
	     match_len = str->len ;
	     break ;
	 }

      case SEP_MLR:
      case SEP_RE:
	 rec_end = re_pos_match(rec_start,fin->end - rec_start, rs_shadow.ptr, &match_len,0) ;
	 /* if the match is at the end, there might still be
	       more to match in the file */
	 if (rec_end && rec_end + match_len == fin->end && !(fin->flags & EOF_FLAG)) {
	    rec_end = (char *) 0 ;
	 }
	 break ;

      default:
	 bozo("type of rs_shadow") ;
   }

   if (rec_end)
   {
      /* the easy and normal case */
      *len_p = rec_end - rec_start ;
      fin->start = rec_end + match_len ;
      return rec_start ;
   }

   if (fin->flags & EOF_FLAG)
   {
      /* last record without a record terminator */
      *len_p = fin->end - rec_start ;
      fin->start = fin->end ;

      if (rs_shadow.type == SEP_MLR && fin->end[-1] == '\n')
      {
          /* multi line terminator
             last record was terminated with only one '\n'
	     make the single '\n' the last record terminator
	  */
	  *len_p -= 1 ;
      }
      return rec_start ;
   }

   if (rec_start == fin->buff)
   {
      /* current record is too big for the input buffer, grow buffer */
      rec_start = enlarge_fin_buffer(fin) ;
   }
   else
   {
      /* move a partial line to front of buffer and try again
         the partial line starts at rec_start and ends at fin->end */

      size_t partial_sz = fin->end - rec_start ;
      size_t fillsz = fin->buffsz - partial_sz ;
      size_t actual_fillsz ;

      /* the man pages say memcpy might not work here, probably BS but ...*/
      memmove(fin->buff, rec_start, partial_sz) ;
      rec_start = fin->buff ;

      actual_fillsz = fillbuff(fin->fd,fin->buff+partial_sz,fillsz) ;
      fin->end = fin->buff + partial_sz + actual_fillsz ;

      if (actual_fillsz < fillsz) {
          fin->flags |= EOF_FLAG ;
      }
   }
   goto retry ;
}


#define  K256    (1024*256)
/* double buffer size up to 256K, then grow by 256K */

static char *
enlarge_fin_buffer(FIN* fin)
{
   size_t r ;
   size_t oldsize = fin->buffsz ;
   size_t newsize = oldsize < K256 ? 2*oldsize : oldsize + K256 ;
   size_t delta = newsize - oldsize ;

   fin->start =
      fin->buff = (char *) erealloc(fin->buff, newsize) ;
   fin->buffsz = newsize ;

   r = fillbuff(fin->fd, fin->buff + oldsize, delta) ;
   if (r < delta)  fin->flags |= EOF_FLAG ;
   fin->end = fin->buff + oldsize + r ;

   return fin->buff ;
}

/*--------
  target is big enough to hold size chars
  terminates if read error

  Note a return of less than size indicates EOF
 *--------------*/
size_t
fillbuff(int fd, char* target, size_t size)
{
    int r ;
    size_t entry_size = size ;

    while (size>0) {
        switch (r = read(fd, target, size)) {
	    case -1:
	        errmsg(errno, "read error") ;
	        mawk_exit(2) ;

	    case 0:
	        goto out ;

	    default:
	        target += r ; size -= r ;
	        break ;
        }
    }
out :
    return entry_size - size ;
}

/* main_fin is a handle to the main input stream
   == 0	 never been opened   */

FIN *main_fin ;
ARRAY Argv ;			 /* to the user this is ARGV  */
static double argi = 1.0 ;	 /* index of next ARGV[argi] to try to open */


static void
set_main_to_stdin(void)
{
   cell_destroy(FILENAME) ;
   FILENAME->type = C_STRING ;
   FILENAME->ptr = (PTR) new_STRING("-") ;
   cell_destroy(FNR) ;
   FNR->type = C_DOUBLE ;
   FNR->dval = 0.0 ; rt_fnr = 0 ;
   main_fin = FINdopen(0, 1) ;
}


/* this gets called once to get the input stream going.
   It is called after the execution of the BEGIN block
   unless there is a getline inside BEGIN {}
*/
void
open_main(void)
{
   CELL argc ;

#if  MSDOS
   int k = binmode() ;

   if (k & 1)  setmode(0, O_BINARY) ;
   if ( k & 2 ) { setmode(1,O_BINARY) ; setmode(2,O_BINARY) ; }
#endif

   cellcpy(&argc, ARGC) ;
   if (argc.type != C_DOUBLE)  cast1_to_d(&argc) ;

   if (argc.dval == 1.0)  set_main_to_stdin() ;
   else	 next_main(1) ;
}

/* get the next command line file open */
FIN *
next_main(int open_flag)
    /* called by open_main() if open_flag is on  */
{
   register CELL *cp ;
   CELL argc ;			 /* copy of ARGC */
   CELL c_argi ;		 /* cell copy of argi */
   CELL argval ;		 /* copy of ARGV[c_argi] */


   argval.type = C_NOINIT ;
   c_argi.type = C_DOUBLE ;

   if (main_fin)  FINclose(main_fin) ;
   /* FILENAME and FNR don't change unless we really open
     a new file */

   /* make a copy of ARGC to avoid side effect */
   if (cellcpy(&argc, ARGC)->type != C_DOUBLE)	cast1_to_d(&argc) ;

   while (argi < argc.dval)
   {
      c_argi.dval = argi ;
      argi += 1.0 ;

      if (!(cp = array_find(Argv, &c_argi, NO_CREATE)))
	 continue ;		 /* its deleted */

      /* make a copy so we can cast w/o side effect */
      cell_destroy(&argval) ;
      cp = cellcpy(&argval, cp) ;
      if (cp->type < C_STRING)	cast1_to_s(cp) ;
      if (string(cp)->len == 0)	 continue ;
      /* file argument is "" */

      /* it might be a command line assignment */
      if (is_cmdline_assign(string(cp)->str))  continue ;

      /* try to open it -- we used to continue on failure,
       but posix says we should quit */
      if (!(main_fin = FINopen(string(cp)->str, 1)))
      {
         errmsg(errno, "cannot open %s", string(cp)->str) ;
	 mawk_exit(2) ;
      }

      /* success -- set FILENAME and FNR */
      cell_destroy(FILENAME) ;
      cellcpy(FILENAME, cp) ;
      free_STRING(string(cp)) ;
      cell_destroy(FNR) ;
      FNR->type = C_DOUBLE ;
      FNR->dval = 0.0 ; rt_fnr = 0 ;

      return main_fin ;
   }
   /* failure */
   cell_destroy(&argval) ;

   if (open_flag)
   {
      /* all arguments were null or assignment */
      set_main_to_stdin() ;
      return main_fin ;
   }

   /* real failure */
   {
      /* this is how we mark EOF on main_fin  */
      static char dead_buff = 0 ;
      static FIN dead_main =
      {0, (FILE *) 0, &dead_buff, &dead_buff, &dead_buff, 0, EOF_FLAG} ;

      return main_fin = &dead_main ;
      /* since MAIN_FLAG is not set, FINgets won't call next_main() */
   }
}


int
is_cmdline_assign(char* s)
{
   char *p ;
   int c ;
   SYMTAB *stp ;
   CELL *cp = 0 ;
   unsigned len ;
   CELL cell ;			 /* used if command line assign to pseudo field */
   CELL *fp = (CELL *) 0 ;	 /* ditto */

   if (scan_code[*(unsigned char *) s] != SC_IDCHAR)  return 0 ;

   p = s + 1 ;
   while ((c = scan_code[*(unsigned char *) p]) == SC_IDCHAR
	  || c == SC_DIGIT)
      p++ ;

   if (*p != '=')  return 0 ;

   *p = 0 ;
   stp = find(s) ;

   switch (stp->type)
   {
      case ST_NONE:
	 stp->type = ST_VAR ;
	 stp->stval.cp = cp = ZMALLOC(CELL) ;
	 break ;

      case ST_VAR:
      case ST_NR:		/* !! no one will do this */
	 cp = stp->stval.cp ;
	 cell_destroy(cp) ;
	 break ;

      case ST_FIELD:
	 /* must be pseudo field */
	 fp = stp->stval.cp ;
	 cp = &cell ;
	 break ;

      default:
	 rt_error(
	 "cannot command line assign to %s\n\ttype clash or keyword"
		    ,s) ;
   }

   /* we need to keep ARGV[i] intact */
   *p++ = '=' ;
   len = strlen(p) + 1 ;
   p = strcpy((char *) zmalloc(len), p) ;
   {
       /* posix says escape sequences are on from command line */
       size_t rlen = rm_escape(p) ;
       cp->ptr = new_STRING2(p,rlen) ;
   }
   zfree(p, len) ;
   check_strnum(cp) ;		 /* sets cp->type */
   if (fp)			/* move it from cell to pfield[] */
   {
      field_assign(fp, cp) ;
      free_STRING(string(cp)) ;
   }
   return 1 ;
}
