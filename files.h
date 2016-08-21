
/********************************************
files.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


#ifndef   FILES_H
#define   FILES_H

/* IO redirection types */
#define  F_IN           (-5)
#define  PIPE_IN        (-4)
#define  PIPE_OUT       (-3)
#define  F_APPEND       (-2)
#define  F_TRUNC        (-1)
#define  IS_OUTPUT(type)  ((type)>=PIPE_OUT)

extern const char* const shell ; /* for pipes and system() */

PTR   file_find(STRING *, int) ;
int   file_close(STRING *) ;
int   file_flush(STRING *) ;
void  flush_all_output(void) ;
int   close_all_output() ;

void  write_error(FILE*) ;

#if MSDOS
int   DOSexec(char *) ;
int   binmode(void) ;
void  set_binmode(int) ;
void  enlarge_output_buffer(FILE*) ;
#endif


#endif
