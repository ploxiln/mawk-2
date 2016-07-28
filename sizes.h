/********************************************
sizes.h
copyright 1991-1995,2014-2016.  Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/*  sizes.h  */

#ifndef  SIZES_H
#define  SIZES_H

#define  MAX__INT   0x7fffffff 


#define EVAL_STACK_SIZE  256	/* initial size , can grow */

/*
 * FBANK_SZ, the number of fields at startup, must be a power of 2.
 *
 */
#define  FBANK_SZ	1024
#define  FB_SHIFT	  10	/* lg(FBANK_SZ) */

/*
 * initial size of sprintf buffer
 */
#define  SPRINTF_LIMIT	8192

#define  BUFFSZ         4096
#define  FINBUFFSZ      8192
  /* starting buffer size for input files, grows if
     necessary */

#define  HASH_PRIME  113

#define  MAX_COMPILE_ERRORS  5	/* quit if more than 4 errors */

#endif /* SIZES_H */
