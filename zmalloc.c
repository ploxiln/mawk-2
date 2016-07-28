
/********************************************
zmalloc.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/

#include  "mawk.h"
#include  "zmalloc.h"

static void
out_of_mem(void)
{
   const char* out = "out of memory" ;

   if (mawk_state == EXECUTION)	 rt_error(out) ;
   else
   {
      /* I don't think this will ever happen */
      compile_error(out) ; mawk_exit(2) ;
   }
}

void* emalloc(size_t sz)
{
    void* ret = malloc(sz) ;
    if (ret == 0) out_of_mem() ;
    return ret ;
}

void* erealloc(void* p, size_t sz)
{
    void* ret = realloc(p, sz) ;
    if (ret == 0) out_of_mem() ;
    return ret ;
}

/* if we are valgrinding or purifying */

#ifdef  MEM_CHECK

void* zmalloc(size_t sz)
{
    return emalloc(sz) ;
}

void* zrealloc(void* p, size_t old, size_t new)
{
    return erealloc(p,new) ;
}

void zfree(void* p, size_t sz)
{
    free(p) ;
}

#else    /* usual  case */
/*
  zmalloc() gets mem from emalloc() in chunks of ZSIZE * AVAIL_SZ
  and cuts these blocks into smaller pieces that are multiples ZSIZE.
  When a piece is returned via zfree(), it goes
  on a linked linear list indexed by its size.	The lists are
  an array, pool[].
*/

/* block sizes are set by this #define */

#define  ZSZ    (4*sizeof(long))

typedef union zblock {
    union zblock* link ;
    double align ;
    char filler[ZSZ] ;
} ZBlock ;

#define  ZSIZE  sizeof(ZBlock)

#define  bytes_to_blocks(b) (((b)+ZSIZE-1)/ZSIZE)

/* memory from emalloc goes here to be partitioned into
   smaller pieces  that end up in pool[]
*/

static ZBlock* avail ;
static size_t amt_avail ;

#define AVAIL_SZ    1024    /* number of ZBlocks to get from emalloc */

static void
fill_avail(void)
{
    avail = (ZBlock *)emalloc(ZSIZE * AVAIL_SZ) ;
    amt_avail = AVAIL_SZ ;
}

#define  POOL_SZ    16
static ZBlock* pool[POOL_SZ] ;
/* size of biggest block in pool[] */
#define zmalloc_limit  (16*ZSIZE)

void* zmalloc(size_t sz)
{
    if (sz > zmalloc_limit) {
        return emalloc(sz) ;
    }
    {
        size_t  blks = bytes_to_blocks(sz) ;
	ZBlock* p = pool[blks-1] ;
	if (p) {
	    /* get mem from pool */
	    pool[blks-1] = p->link ;
	    return p ;
	}

	if (blks > amt_avail) {
	    if (amt_avail > 0) {
		avail->link = pool[amt_avail-1] ;
	        pool[amt_avail-1] = avail ;
	    }
	    fill_avail() ;
	}
        /* cut a piece off the avail block */
	p = avail ;
	avail += blks ;
	amt_avail -= blks ;
	return p ;
    }
}

void zfree(void* p, size_t sz)
{
    if (sz > zmalloc_limit) {
        free(p) ;
    }
    else {
	/* put p in pool[] */
        size_t blks = bytes_to_blocks(sz) ;
	ZBlock* zp = (ZBlock*) p ;
	zp->link = pool[blks-1] ;
	pool[blks-1] = zp ;
    }
}

void* zrealloc(void* p, size_t old_size, size_t new_size)
{
    if (new_size > zmalloc_limit && old_size > zmalloc_limit) {
        return erealloc(p,new_size) ;
    }
    else {
        void* ret = zmalloc(new_size) ;
        memcpy(ret, p, old_size < new_size ? old_size : new_size) ;
        zfree(p, old_size) ;
	return ret ;
    }
}

#endif
