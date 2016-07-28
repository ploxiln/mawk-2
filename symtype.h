
/********************************************
symtype.h
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/* types related to symbols are defined here */

#ifndef  SYMTYPE_H
#define  SYMTYPE_H


/* struct to hold info about builtins */
typedef struct {
const char *name ;
PF_CP  fp ;  /* ptr to function that does the builtin */
unsigned char min_args, max_args ; 
/* info for parser to check correct number of arguments */
} BI_REC ;

/*---------------------------
   structures and types for arrays
 *--------------------------*/

#include "array.h"

extern  ARRAY  Argv ;


/* for parsing  (i,j) in A  */
typedef  struct {
int start ; /* offset to code_base */
int cnt ;
} ARG2_REC ;

/*------------------------
  user defined functions
  ------------------------*/

typedef  struct fblock {
const char *name ;
INST *code  ;
unsigned short nargs ;
char *typev ;  /* array of size nargs holding types */
} FBLOCK ;   /* function block */

void   add_to_fdump_list(FBLOCK *) ;
void   fdump(void) ;

/*-------------------------
  elements of the symbol table
  -----------------------*/

#define  ST_NONE 0
#define  ST_VAR   1
#define  ST_KEYWORD   2
#define  ST_BUILTIN 3 /* a pointer to a builtin record */
#define  ST_ARRAY   4 /* a void * ptr to a hash table */
#define  ST_FIELD   5  /* a cell ptr to a field */
#define  ST_FUNCT   6
#define  ST_NR      7  /*  NR is special */
#define  ST_ENV     8  /* and so is ENVIRON */
#define  ST_UNUSED  9  
#define  ST_LOCAL_NONE  10
#define  ST_LOCAL_VAR   11
#define  ST_LOCAL_ARRAY 12

#define  is_local(stp)   ((stp)->type>=ST_LOCAL_NONE)

typedef  struct {
const char *name ;
int type ;
unsigned offset ;  /* offset in stack frame for local vars */
union {
CELL *cp ;
int  kw ;
PF_CP fp ;
BI_REC *bip ;
ARRAY  array ; 
FBLOCK  *fbp ;
} stval ;
}  SYMTAB ;

/* for patching length(A) when A is local and type unknown */
typedef struct {
    FBLOCK* fbp ;
    unsigned offset ;
} Local_PI ;


/*****************************
 structures for type checking function calls
 ******************************/

typedef  struct ca_rec {
struct ca_rec  *link ;
short type ;
short arg_num ;  /* position in callee's stack */
/*---------  this data only set if we'll  need to patch -------*/
/* happens if argument is an ID or type ST_NONE or ST_LOCAL_NONE */

int call_offset ;
/* where the type is stored */
SYMTAB  *sym_p ;  /* if type is ST_NONE  */
char *type_p ;  /* if type  is ST_LOCAL_NONE */
}  CA_REC  ; /* call argument record */

/* type field of CA_REC matches with ST_ types */
#define   CA_EXPR       ST_LOCAL_VAR
#define   CA_ARRAY      ST_LOCAL_ARRAY

typedef  struct fcall {
struct fcall *link ;
FBLOCK  *callee ;
short   call_scope ;
short   move_level ;  
FBLOCK  *call ;  /* only used if call_scope == SCOPE_FUNCT  */
INST    *call_start ; /* computed later as code may be moved */
CA_REC  *arg_list ;
short   arg_cnt_checked ;
unsigned line_no ; /* for error messages */
} FCALL_REC ;

extern  FCALL_REC  *resolve_list ;

void  resolve_fcalls(void) ;
void  check_fcall(FBLOCK*,int,int,FBLOCK*,CA_REC*,unsigned) ;
void   relocate_resolve_list(int,int,FBLOCK*,int,unsigned,int) ;

/* hash.c */
unsigned   hash(const char *) ;
unsigned   hash2(const char*,size_t) ;
SYMTAB * insert(const char *) ;
SYMTAB * find(const char *) ;
const char * reverse_find(int, PTR) ;
SYMTAB * save_id(const char *) ;
void     restore_ids(void) ;

/* error.c */
void   type_error(SYMTAB *) ;

#endif  /* SYMTYPE_H */
