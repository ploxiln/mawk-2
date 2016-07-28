
/********************************************
fin.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/

/* fin.h */

#ifndef  FIN_H
#define  FIN_H
/* structure to control input files */

typedef struct {
int  fd ;
FILE *fp ;   
char *buff ; /* base */
char *start ; /* start of next record */
char* end ;   /* end of data in buff[] */
size_t buffsz ; /* size of buff[] */
int  flags ;
int  close_val ;   /* return value for close */
}  FIN ;

/* fd and fp in FIN: three possiblities.
   interactive -- reading with fgets,  fp is valid fd is -1
   block reading a file --  fd is valid >= 0, fp is 0
   block reading an input pipe -- fd is valid, used for read
                                  fp is valid, used for pclose()
*/

#define  MAIN_FLAG    1   /* part of main input stream if on */
#define  EOF_FLAG     2
#define  START_FLAG   4   /* used when RS == "" */

FIN *   FINdopen(int, int) ;
FIN *   FINopen(const char *, int) ;
FIN*    FINpopen(const char*) ;
int     FINclose(FIN *) ;
void    FINsemi_close(FIN *) ;
char*   FINgets(FIN *, size_t *) ;
size_t  fillbuff(int, char *, size_t) ;

extern  FIN  *main_fin ;  /* for the main input stream */
void    open_main(void) ;
FIN*    next_main(int) ;

#define setmode	setmode_		/* avoid conflict with declaration in <unistd.h> on BitRig BSD */
void   setmode(int,int) ;
#endif  /* FIN_H */
