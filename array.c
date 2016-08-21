/* array.c */
/*
copyright 1991-1996,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

array.c and array.h were generated with the commands

   notangle -R'"array.c"' array.w > array.c 
   notangle -R'"array.h"' array.w > array.h 

Notangle is part of Norman Ramsey's noweb literate programming package.
Noweb home page: http://www.cs.tufts.edu/~nr/noweb/

It's easiest to read or modify this file by working with array.w.
*/

#include "mawk.h"
#include "symtype.h"
#include "memory.h"
#include "split.h"
#include "field.h"
#include "bi_vars.h"
typedef struct itable ITable ;
static CELL* itable_find(ITable* it, STRING* sval, int64_t ival, 
                         int flag, size_t* sizep) ;
static void array_int_to_str(ARRAY) ;
static void array_split_to_int(ARRAY) ;
static void array_split_to_str(ARRAY) ;

#ifdef QSORT
static int i_compare(const void*, const void*) ;
static int s_compare(const void*, const void*) ;
#endif

typedef struct hnode {
    struct hnode* link ;
    int64_t ival ;
    STRING* key ;
    CELL cell ;
} HNODE ;
#define INIT_HMASK   255
#define MAX_AVE_BUCKET_SIZE   4
struct itable {
    unsigned hmask ;
    unsigned size ;
    unsigned limit ;
    HNODE** buckets ;
} ;
static HNODE** make_buckets(unsigned) ;
static ITable* make_empty_itable(void) ;
    
static void hnode_free(HNODE*) ;
static void itable_free(ITable*) ;

static void itable_convert_i_to_s(ITable*) ;

static void double_num_buckets(ITable*) ;
        
static int64_t* itable_i_vector(ITable*) ;
static STRING** itable_s_vector(ITable*) ;
CELL* array_find(ARRAY A, CELL *cp, int create_flag)
{
    CELL key ; /* a copy of *cp */
    if (A->size == 0 && !create_flag) {
        return 0 ;
    }
    cellcpy(&key,cp) ;
reswitch:
    switch(A->type) {
        case AY_NULL:
            {
                ITable* tb = make_empty_itable() ;
                A->ptr = tb ;
                A->size = 0 ;
                if (key.type != C_DOUBLE || !is_int_double(key.dval)) {
                    A->type = AY_STR ;
                    goto reswitch ;
                }
                A->type = AY_INT ;
                return itable_find(tb, 0, (int64_t) key.dval, CREATE, &A->size) ;
            }


        case AY_SPLIT:
            {
                if (key.type != C_DOUBLE || !is_int_double(key.dval)) {
                    array_split_to_str(A) ;
                    goto reswitch ;
                }
                if (key.dval < 1.0 || key.dval > (double) A->size) {
                    if (create_flag) {
                        array_split_to_int(A) ;
                        goto reswitch ;
                    }
                    else  return 0 ;
                }
                else {
                    /* the expected case */
                    CELL* cells = (CELL*)A->ptr ;
                    unsigned d = (unsigned) key.dval ;
                    return &cells[d-1] ;
                }
            }

        case AY_STR:
            { 
                CELL* ret ;
                cast1_to_s(&key) ;
                ret = itable_find((ITable*)(A->ptr),string(&key), 0,
                          create_flag, &A->size) ;
                free_STRING(string(&key)) ;
                return ret ;
            }
        case AY_INT:
            {
                if (key.type != C_DOUBLE || !is_int_double(key.dval)) {
                    array_int_to_str(A) ;
                    goto reswitch ;
                }
                /* the expected case */
                return itable_find((ITable*)A->ptr, 0, (int64_t)key.dval, 
                           create_flag, &A->size) ;
            }
    }
    /* not reached */
    return 0 ;
}
void array_delete(ARRAY A, CELL* cp)
{
    CELL key ; /* copy of *cp */
    if (A->type == AY_NULL) return ;

    cellcpy(&key,cp) ;
reswitch:
    switch(A->type) {
        case AY_STR:
            cast1_to_s(&key) ;
            itable_find((ITable*)A->ptr, string(&key), 0, DELETE_, &A->size) ;
            free_STRING(string(&key)) ;
            break ;
        case AY_INT:
            if (key.type != C_DOUBLE || !is_int_double(key.dval)) {
                array_int_to_str(A) ;
                goto reswitch ;
            }
            itable_find((ITable*)A->ptr, 0, (int64_t) key.dval,
                        DELETE_, &A->size) ;
            break ;
        case AY_SPLIT:
            if (key.type != C_DOUBLE || !is_int_double(key.dval)) {
                array_split_to_str(A) ;
            }
            else if (key.dval < 1.0 || key.dval > (double)A->size) {
                /* not in the array so nothing to do */
                return ;
            }
            else {
                array_split_to_int(A) ;
            }
            goto reswitch ;
    }
    if (A->size == 0) array_clear(A) ;
}
void array_load(ARRAY A, size_t cnt)
{
    array_clear(A) ;
    if (cnt > 0) {
        CELL* cells = (CELL*)zmalloc(sizeof(CELL) * cnt) ;
        memset(cells, 0, sizeof(CELL) * cnt) ;
        A->size = cnt ;
        A->type = AY_SPLIT ;
        A->ptr = cells ;
        transfer_to_array(cells, cnt) ;
    }
}
void array_clear(ARRAY A)
{
    if (A->type == AY_NULL) return ;

    if (A->type == AY_SPLIT) {
        unsigned i ;
        for(i = 0; i < A->size; i++) {
            cell_destroy((CELL*)A->ptr+i) ;
        }
        zfree(A->ptr, A->size * sizeof(CELL)) ;
    }
    else {
        itable_free((ITable*)A->ptr) ;
    }

    A->ptr = 0 ;
    A->size = 0 ;
    A->type = AY_NULL ;
}
ALoop* make_aloop(ARRAY A, CELL* cp) 
{
    ALoop* al = (ALoop*)zmalloc(sizeof(ALoop)) ;
    al->type = A->type ;
    al->size = A->size ;
    al->next = 0 ;
    al->cp = cp ;
    al->link = 0 ;
    if (al->type == AY_INT) {
        al->ptr.ival = itable_i_vector((ITable*)(A->ptr)) ;
#ifdef QSORT
        qsort(al->ptr.ival, al->size, sizeof(int64_t), i_compare) ;
#endif
    }
    else if (al->type == AY_STR) {
        al->ptr.sval = itable_s_vector((ITable*)(A->ptr)) ;
#ifdef QSORT
        qsort(al->ptr.sval, al->size, sizeof(STRING*), s_compare) ;
#endif
    }
    return al ;
}
void aloop_free(ALoop* al)
{
    if (al->type == AY_INT) {
        zfree(al->ptr.ival, sizeof(int64_t) * al->size) ;
    }
    else if (al->type == AY_STR) {
        unsigned i ;
        for(i=0; i < al->size; i++) {
            free_STRING(al->ptr.sval[i]) ;
        }
        zfree(al->ptr.sval, sizeof(STRING*) * al->size) ;
    }
    zfree(al, sizeof(ALoop)) ;
}
int aloop_next(ALoop* al)
{
    if (al->next >= al->size) return 0 ;
    cell_destroy(al->cp) ;
    switch(al->type) {
        case AY_SPLIT:
            al->cp->type = C_DOUBLE ;
            al->cp->dval = (double) (al->next+1) ;
            break ;
        case AY_INT:
            al->cp->type = C_DOUBLE ;
            al->cp->dval = (double) al->ptr.ival[al->next] ;
            break ;
        case AY_STR:
            al->cp->type = C_STRING ;
            al->cp->ptr = STRING_dup(al->ptr.sval[al->next]) ;
            break ;
    }
    al->next++ ;
    return 1 ;
}
CELL *array_cat(
   CELL *sp,
   int cnt)
{
   CELL *p ;  /* walks the eval stack */
   CELL subsep ;  /* local copy of SUBSEP */
   size_t subsep_len ; /* string length of subsep_str */
   char *subsep_str ;

   size_t total_len ;  /* length of cat'ed expression */
   CELL *top ;   /* value of sp at entry */
   char *target ;  /* build cat'ed char* here */
   STRING *sval ;  /* build cat'ed STRING here */
   {
       cellcpy(&subsep, SUBSEP) ;
       if ( subsep.type < C_STRING ) cast1_to_s(&subsep) ;
       subsep_len = string(&subsep)->len ;
       subsep_str = string(&subsep)->str ;
   }

   {
       top = sp ; 
       sp -= (cnt-1) ;
   }

   total_len = ((size_t) (cnt-1)) * subsep_len ;
   for(p = sp ; p <= top ; p++) {
      if ( p->type < C_STRING ) cast1_to_s(p) ;
      total_len += string(p)->len ;
   }

   sval = new_STRING0(total_len) ;
   target = sval->str ;
   for(p = sp ; p < top ; p++) {
      memcpy(target, string(p)->str, string(p)->len) ;
      target += string(p)->len ;
      memcpy(target, subsep_str, subsep_len) ;
      target += subsep_len ;
   }
   /* now p == top */
   memcpy(target, string(p)->str, string(p)->len) ;

   for(p = sp; p <= top ; p++) free_STRING(string(p)) ;
   free_STRING(string(&subsep)) ;
   /* set contents of sp , sp->type > C_STRING is possible so reset */
   sp->type = C_STRING ;
   sp->ptr = (PTR) sval ;
   return sp ;

}
static void array_int_to_str(ARRAY A)
{
    ITable* ht = (ITable*)A->ptr ;
    itable_convert_i_to_s(ht) ;
    A->type = AY_STR ;
}
static void array_split_to_int(ARRAY A)
{
    ITable* tb = make_empty_itable() ;
    unsigned i ;
    CELL* cells = (CELL*)A->ptr ;
    for(i=1; i <= A->size; i++) {
        size_t unused ;
        CELL* cp = itable_find(tb,0, i, CREATE, &unused) ;
        *cp = cells[i-1] ; /* no ref cnt adjustment needed */
    }
    zfree(cells, sizeof(CELL) * A->size) ;
    A->type = AY_INT ;
    A->ptr = tb ;
    /* A->size stayed the same */
}
static void array_split_to_str(ARRAY A)
{
    ITable* tb = make_empty_itable() ;
    unsigned i ;
    CELL* cells = (CELL*)A->ptr ;
    for(i=1; i <= A->size; i++) {
        size_t unused ;
        char buffer[128] ;
        STRING* sval ;
        CELL* cp ; 
        sprintf(buffer, "%u" , i) ;
        sval = new_STRING(buffer) ;
        cp = itable_find(tb, sval, 0, CREATE, &unused) ;
        *cp = cells[i-1] ; /* no ref cnt adjustment needed */
        free_STRING(sval) ;
    }
    zfree(cells, sizeof(CELL) * A->size) ;
    A->type = AY_STR ;
    A->ptr = tb ;
    /* A->size stayed the same */
}
#ifdef QSORT
static int i_compare(const void* l, const void* r)
{
    const int64_t* il = l ;
    const int64_t* ir = r ;
    return *il - *ir ;
}

static int s_compare(const void* l, const void* r)
{
    STRING* const* sl = l ;
    STRING* const* sr = r ;
    return STRING_cmp(*sl,*sr) ;
}
#endif
static ITable* make_empty_itable()
{
    ITable* ret = ZMALLOC(ITable) ;
    ret->hmask = INIT_HMASK ;
    ret->size = 0 ;
    ret->limit = MAX_AVE_BUCKET_SIZE * (INIT_HMASK+1) ;
    ret->buckets = make_buckets(INIT_HMASK+1) ;
    return ret ;
}
static HNODE** make_buckets(unsigned cnt)
{
    HNODE** bks = (HNODE**)emalloc(cnt * sizeof(HNODE*)) ;
    memset(bks, 0, cnt * sizeof(HNODE*)) ;
    return bks ;
}
CELL* itable_find(ITable* htable, STRING* sval, int64_t ival, 
                  int flag, size_t* szp) 
{
    int64_t hash = sval ? hash2(sval->str,sval->len) : ival ;
    unsigned idx = (unsigned) hash & htable->hmask ;
    HNODE* q = 0 ;
    HNODE* p = htable->buckets[idx] ;
    while(p) {
        if (hash == p->ival && (!sval || STRING_eq(sval,p->key))) {
            /* found */
            if (flag == DELETE_) {
                {
                    htable->size-- ;
                    *szp = htable->size ;
                    if (q) {
                        q->link = p->link ;
                    }
                    else {
                        htable->buckets[idx] = p->link ;
                    }
                    hnode_free(p) ;
                    return 0 ;
                }

            }
            if (q) {
                /* move to front */
                q->link = p->link ;
                p->link = htable->buckets[idx] ;
                htable->buckets[idx] = p ;
            }  /* else at front */
            return &p->cell ;
        }
        q = p ;
        p = p->link ;
    }
    /* not found */
    if (flag == CREATE) {
        {
            p = ZMALLOC(HNODE) ;
            p->key = sval ? STRING_dup(sval) : 0 ;
            p->ival = hash ;
            p->cell.type = C_NOINIT ;
            p->link = htable->buckets[idx] ;
            htable->buckets[idx] = p ;
            htable->size++ ;
            *szp = htable->size ;
            if (htable->size > htable->limit) {
                double_num_buckets(htable) ;
            }
        }

        return &p->cell ;
    }
    return 0 ;
}
static void hnode_free(HNODE* p)
{
    if (p->key) free_STRING(p->key) ;
    cell_destroy(&p->cell) ;
    zfree(p, sizeof(HNODE)) ;
}
static void itable_free(ITable* ht)
{
    unsigned i ;
    unsigned size = ht->size ;
    for(i=0; size > 0; i++) {
        HNODE* p = ht->buckets[i] ;
        while(p) {
            HNODE* q = p ;
            p = p->link ;
            hnode_free(q) ;
            size-- ;
        }
    }
    free(ht->buckets) ;
    zfree(ht, sizeof(ITable)) ;
}
static void itable_convert_i_to_s(ITable* ht)
{
    char buffer[256] ;
    unsigned hmask = ht->hmask ;
    HNODE** new_buckets = make_buckets(hmask+1) ;
    unsigned i ;  /* walks old buckets */
    unsigned j ;  /* index into new_buckets */
    unsigned cnt = ht->size ;  /* number of nodes to convert */
    for(i=0; cnt > 0; i++) {
        HNODE* p = ht->buckets[i] ;
        while(p) {
            HNODE* q = p ;
            p = p->link ;
            sprintf(buffer, LDFMT, q->ival) ;
            q->key = new_STRING(buffer) ;
            q->ival = hash(buffer) ;
            j = q->ival & hmask ;
            q->link = new_buckets[j] ;
            new_buckets[j] = q ;
            cnt-- ;
        }
    }
    free(ht->buckets) ;
    ht->buckets = new_buckets ;
}
static void double_num_buckets(ITable* htable)
{
   unsigned old_hmask = htable->hmask ;
   unsigned new_hmask = (old_hmask<<1)+1 ;
   HNODE** buckets = htable->buckets ;
   buckets = (HNODE**)erealloc(buckets, sizeof(HNODE*) * (new_hmask+1)) ;
   {
       HNODE* p ;  /* walks bucket[i] */
       HNODE* q ;  /* trails p */
       HNODE* tail ; /* builds bucket[j] from the back */
       HNODE p0 ;  /* sentinel */
       HNODE t0 ;  /* sentinel */
       unsigned i, j ;

       for(i=0, j=old_hmask+1; i < old_hmask+1; i++, j++) {
           p = buckets[i] ;
           q = &p0 ;
           q->link = p ;
           tail = &t0 ;
           tail->link = 0 ;

           while(p) {
               if (p->ival & (old_hmask+1)) {
                   /* it moves */
                   q->link = p->link ;
                   tail = tail->link = p ;
                   tail->link = 0 ;
                   p = q->link ;
               }
               else {
                   q = p ;
                   p = p->link ;
               }
           }
           buckets[i] = p0.link ;
           buckets[j] = t0.link ;
       }
   }

   htable->hmask = new_hmask ;
   htable->limit = (new_hmask+1) * MAX_AVE_BUCKET_SIZE ;
   htable->buckets = buckets ;
}
static int64_t*  itable_i_vector(ITable* it)
{
    int64_t* ret = (int64_t*)zmalloc(sizeof(int64_t)*it->size) ;
    unsigned r = 0 ;
    unsigned i ;
    for(i=0; r < it->size; i++) {
        HNODE* p = it->buckets[i] ;
        while(p) {
            ret[r++] = p->ival ;
            p = p->link ;
        }
    }
    return ret ;
}
static STRING**  itable_s_vector(ITable* it)
{
    STRING** ret = (STRING**)zmalloc(sizeof(STRING*)*it->size) ;
    unsigned r = 0 ;
    unsigned i ;
    for(i=0; r < it->size; i++) {
        HNODE* p = it->buckets[i] ;
        while(p) {
            ret[r++] = STRING_dup(p->key) ;
            p = p->link ;
        }
    }
    return ret ;
}

