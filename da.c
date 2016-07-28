
/********************************************
da.c
copyright 1991, 2014-16 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/*  disassemble code */


#include  "mawk.h"


#include  "code.h"
#include  "bi_funct.h"
#include  "repl.h"
#include  "field.h"
#include  "printf.h"

static const char* find_bi_name(PF_CP) ;

static const struct sc
{
   char op ;
   const char *name ;
} simple_code[] =

{
    { _STOP, "stop" } ,
    { FE_PUSHA, "fe_pusha" } ,
    { FE_PUSHI, "fe_pushi" } ,
    { A_TEST, "a_test" } ,
    { A_DEL, "a_del" } ,
    { DEL_A, "del_a" } ,
    { POP_AL, "pop_al" } ,
    { _POP, "pop" } ,
    { _ADD, "add" } ,
    { _SUB, "sub" } ,
    { _MUL, "mul" } ,
    { _DIV, "div" } ,
    { _MOD, "mod" } ,
    { _POW, "pow" } ,
    { _NOT, "not" } ,
    { _UMINUS, "uminus" } ,
    { _UPLUS, "uplus" } ,
    { _TEST, "test" } ,
    { _CAT, "cat" } ,
    { _ASSIGN, "assign" } ,
    { _ADD_ASG, "add_asg" } ,
    { _SUB_ASG, "sub_asg" } ,
    { _MUL_ASG, "mul_asg" } ,
    { _DIV_ASG, "div_asg" } ,
    { _MOD_ASG, "mod_asg" } ,
    { _POW_ASG, "pow_asg" } ,
    { NF_PUSHI, "nf_pushi" } ,
    { F_ASSIGN, "f_assign" } ,
    { F_ADD_ASG, "f_add_asg" } ,
    { F_SUB_ASG, "f_sub_asg" } ,
    { F_MUL_ASG, "f_mul_asg" } ,
    { F_DIV_ASG, "f_div_asg" } ,
    { F_MOD_ASG, "f_mod_asg" } ,
    { F_POW_ASG, "f_pow_asg" } ,
    { _POST_INC, "post_inc" } ,
    { _POST_DEC, "post_dec" } ,
    { _PRE_INC, "pre_inc" } ,
    { _PRE_DEC, "pre_dec" } ,
    { F_POST_INC, "f_post_inc" } ,
    { F_POST_DEC, "f_post_dec" } ,
    { F_PRE_INC, "f_pre_inc" } ,
    { F_PRE_DEC, "f_pre_dec" } ,
    { _EQ, "eq" } ,
    { _NEQ, "neq" } ,
    { _LT, "lt" } ,
    { _LTE, "lte" } ,
    { _GT, "gt" } ,
    { _GTE, "gte" } ,
    { _MATCH2, "match2" } ,
    { _EXIT, "exit" } ,
    { _EXIT0, "exit0" } ,
    { _NEXT, "next" } ,
    { _NEXTFILE, "nextfile" } ,
    { _RET, "ret" } ,
    { _RET0, "ret0" } ,
    { _OMAIN, "omain" } ,
    { _JMAIN, "jmain" } ,
    { OL_GL, "ol_gl" } ,
    { OL_GL_NR, "ol_gl_nr" } ,
    { _HALT, 0 }
} ;


/* write a visible string to fp, enclose in ec (will be / /  or " "  */

void visible_string(FILE* fp, const STRING* sval, int ec)
{
    const char* s = sval->str ;
    const char* const s_end = s + sval->len ;

    fputc(ec,fp) ;
    while(s < s_end) {
	int c = *s++ ;

	if (c == ec) {
	    fprintf(fp, "\\%c", ec) ;
	}
	else if (c == '\\') {
	    fprintf(fp, "\\\\") ;
	}
	else if (c == '\n') {
	    fputs("\\n", fp) ;
	}
	else if(c >= 32 && c < 127) {
	    fputc(c,fp) ;
	}
	else {
	    fprintf(fp, "\\x%02x", c) ;
	}
    }
    fputc(ec,fp) ;
}


static const char *jfmt = "%s%s%03d\n" ;
   /* format to print jumps */
static const char *tab2 = "\t\t" ;

void
da(INST* start, FILE* fp)
{
   CELL *cp ;
   register INST *p = start ;
   const char *name ;

   while (p->op != _HALT)
   {
      /* print the relative code address (label) */
      fprintf(fp, "%03ld ", (long)(p - start)) ;

      switch (p++->op)
      {

	 case _PUSHC:
	    cp = (CELL *) p++->ptr ;
	    switch (cp->type)
	    {
	       case C_RE:
	           {
		       const STRING* sval = re_uncompile(cp->ptr) ;
		       fprintf(fp, "pushc\t0x%lx\t", (long) cp->ptr) ;
		       visible_string(fp,sval,'/') ;
		       fputc('\n',fp) ;
		    }
		  break ;

	       case C_SPACE:
		  fprintf(fp, "pushc\tspace split\n") ;
		  break ;

	       case C_SNULL:
		  fprintf(fp, "pushc\tnull split\n") ;
		  break ;
	       case C_REPL:
	          {
		      const STRING* sval = repl_unscan(cp) ;
		      fprintf(fp, "pushc\trepl\t") ;
		      visible_string(fp, sval, '"') ;
		      fputc('\n', fp) ;
		  }
		  break ;
	       case C_REPLV:
	          {
		      const STRING* sval = repl_unscan(cp) ;
		      fprintf(fp, "pushc\treplv\t") ;
		      visible_string(fp, sval, '"') ;
		      fputc('\n', fp) ;
		  }
		  break ;

	       default:
		  fprintf(fp,"pushc\tWEIRD\n") ;  ;
		  break ;
	    }
	    break ;

	 case _PUSHD:
	    fprintf(fp, "pushd\t%.6g\n", *(double *) p++->ptr) ;
	    break ;
	 case _PUSHS:
	    {
	       STRING *sval = (STRING *) p++->ptr ;
	       fprintf(fp, "pushs\t") ;
	       visible_string(fp,sval,'"') ;
	       fputc('\n',fp) ;
	       break ;
	    }
	 case PUSHFM:
	    {
	       const Form* form = (const Form*)(p++->ptr) ;
	       fprintf(fp, "pushfm\t") ;
	       da_Form(fp, form) ;
	       fputc('\n',fp) ;
	       break ;
	    }

	 case _MATCH0:
	 case _MATCH1:
	    {
	        const STRING* sval = re_uncompile(p->ptr) ;
	        fprintf(fp, "match%d\t0x%lx\t",
		    p[-1].op == _MATCH1, (long) p->ptr) ;
		visible_string(fp, sval, '/') ;
		fputc('\n', fp) ;
	    }
	    p++ ;
	    break ;

	 case _PUSHA:
	    fprintf(fp, "pusha\t%s\n",
		    reverse_find(ST_VAR, &p++->ptr)) ;
	    break ;

	 case _PUSHI:
	    cp = (CELL *) p++->ptr ;
	    if (cp == field)  fprintf(fp, "pushi\t$0\n") ;
	    else if (cp == &fs_shadow)
	       fprintf(fp, "pushi\t@fs_shadow\n") ;
	    else
	    {
	       if (cp > NF && cp <= LAST_PFIELD) {
		  name = reverse_find(ST_FIELD, &cp) ;
	       }
	       else  name = reverse_find(ST_VAR, &cp) ;

	       fprintf(fp, "pushi\t%s\n", name) ;
	    }
	    break ;

	 case L_PUSHA:
	    fprintf(fp, "l_pusha\t%d\n", p++->op) ;
	    break ;

	 case L_PUSHI:
	    fprintf(fp, "l_pushi\t%d\n", p++->op) ;
	    break ;

	 case LAE_PUSHI:
	    fprintf(fp, "lae_pushi\t%d\n", p++->op) ;
	    break ;

	 case LAE_PUSHA:
	    fprintf(fp, "lae_pusha\t%d\n", p++->op) ;
	    break ;

	 case LA_PUSHA:
	    fprintf(fp, "la_pusha\t%d\n", p++->op) ;
	    break ;

	 case F_PUSHA:
	    cp = (CELL *) p++->ptr ;
	    if (cp >= NF && cp <= LAST_PFIELD) {
	       fprintf(fp, "f_pusha\t%s\n",
		       reverse_find(ST_FIELD, &cp)) ;
	    }
	    else  fprintf(fp, "f_pusha\t$%d\n",
		       field_addr_to_index(cp)) ;
	    break ;

	 case F_PUSHI:
	    p++ ;
	    fprintf(fp, "f_pushi\t$%d\n", p++->op) ;
	    break ;

	 case AE_PUSHA:
	    fprintf(fp, "ae_pusha\t%s\n",
		    reverse_find(ST_ARRAY, &p++->ptr)) ;
	    break ;

	 case AE_PUSHI:
	    fprintf(fp, "ae_pushi\t%s\n",
		    reverse_find(ST_ARRAY, &p++->ptr)) ;
	    break ;

	 case A_PUSHA:
	    fprintf(fp, "a_pusha\t%s\n",
		    reverse_find(ST_ARRAY, &p++->ptr)) ;
	    break ;

	 case PI_LOAD:
	     {
	         SYMTAB* stp = (SYMTAB*) p++->ptr ;
		 switch(stp->type) {
		     case ST_VAR:
		         fprintf(fp, "pushi/pi_load\t%s\n", stp->name) ;
			 break ;
		     case ST_ARRAY:
		         fprintf(fp, "a_pusha/pi_load\t%s\n", stp->name) ;
			 p[1].ptr = (PTR)bi_alength ;
			 break ;
		     default:
		         fprintf(fp, "pushi/pi_load\t%s\n", "@unused") ;
			 break ;
		 }
	     }
	     break ;

	 case LPI_LOAD:
	     {
	         Local_PI* pi = (Local_PI*) p++->ptr ;
		 FBLOCK* fbp = pi->fbp ;
		 unsigned offset = pi->offset ;
		 int type = fbp->typev[offset] ;
		 switch(type) {
		     case ST_LOCAL_VAR:
		         fprintf(fp, "lpushi/lpi_load\t%s %u\n",
			             fbp->name, offset) ;
			 break ;
		     case ST_LOCAL_ARRAY:
		         fprintf(fp, "la_pusha/lpi_load\t%s %u\n",
			             fbp->name, offset) ;
			 p[1].ptr = (PTR)bi_alength ;
			 break ;
		     case ST_LOCAL_NONE:
		         fprintf(fp, "pushi/lpi_load\t%s\n", "@unused") ;
			 break ;
		     default:
		         bozo("da LPI_LOAD") ;
			 break ;
		 }
	     }
	     break ;
	 case _PUSHINT:
	    fprintf(fp, "pushint\t%d\n", p++->op) ;
	    break ;

	 case _BUILTIN:
	    fprintf(fp, "%s\n",
		    find_bi_name((PF_CP) p++->ptr)) ;
	    break ;

	 case _PRINT:
	     {
	         const char* name_ = "print" ;
		 PF_CP printer = (PF_CP) p++->ptr ;
		 if (printer == bi_printf) {
		     name_ = "printf" ;
		 }
		 else if (printer == bi_printf1) {
		     name_ = "printf1" ;
		 }
	         fprintf(fp, "%s\n", name_) ;
	     }
	     break ;

	 case _JMP:
	    fprintf(fp, jfmt, "jmp", tab2, (p - start) + p->op) ;
	    p++ ;
	    break ;

	 case _JNZ:
	    fprintf(fp, jfmt, "jnz", tab2, (p - start) + p->op) ;
	    p++ ;
	    break ;

	 case _JZ:
	    fprintf(fp, jfmt, "jz", tab2, (p - start) + p->op) ;
	    p++ ;
	    break ;

	 case _LJZ:
	    fprintf(fp, jfmt, "ljz", tab2, (p - start) + p->op) ;
	    p++ ;
	    break ;

	 case _LJNZ:
	    fprintf(fp, jfmt, "ljnz", tab2+1 , (p - start) + p->op) ;
	    p++ ;
	    break ;

	 case SET_ALOOP:
	    fprintf(fp, "set_al\t%03ld\n", (long)(p + p->op - start)) ;
	    p++ ;
	    break ;

	 case ALOOP:
	    fprintf(fp, "aloop\t%03ld\n", (long)(p - start + p->op)) ;
	    p++ ;
	    break ;

	 case  A_CAT :
	    fprintf(fp,"a_cat\t%d\n", p++->op) ;
	    break ;

	 case _CALL:
	    fprintf(fp, "call\t%s\t%d\n",
		    ((FBLOCK *) p->ptr)->name, p[1].op) ;
	    p += 2 ;
	    break ;

	 case _RANGE:
	    fprintf(fp, "range\t%03ld %03ld %03ld\n",
	    /* label for pat2, action, follow */
		    (long)(p - start + p[1].op),
		    (long)(p - start + p[2].op),
		    (long)(p - start + p[3].op)) ;
	    p += 4 ;
	    break ;

	 default:
	    {
	       const struct sc *q = simple_code ;
	       int k = (p - 1)->op ;

	       while (q->op != _HALT && q->op != k)  q++ ;

	       fprintf(fp, "%s\n",
		       q->op != _HALT ? q->name : "bad instruction") ;
	    }
	    break ;
      }
   }
   fflush(fp) ;
}

static const struct
{
   PF_CP action ;
   const char *name ;
}
special_cases[] =
{
    { bi_split, "split" } ,
    { bi_length, "length"} ,
    { bi_alength, "alength"} ,
    { bi_match, "match" } ,
    { bi_getline, "getline" } ,
    { bi_sub, "sub" } ,
    { bi_gsub, "gsub" } ,
    { bi_sprintf, "sprintf" } ,
    { bi_sprintf1, "sprintf1" } ,
    {0,0}
} ;

static const char *
find_bi_name(PF_CP p)
{
   BI_REC *q ;
   int i ;

   for (q = bi_funct; q->name; q++)
   {
      if (q->fp == p)
      {
	 /* found */
	 return q->name ;
      }
   }
   /* next check some special cases */
    for (i = 0; special_cases[i].action != 0; i++) {
        if (special_cases[i].action == p) {
            return special_cases[i].name ;
	}
    }

    return "unknown builtin" ;
}

static struct fdump
{
   struct fdump *link ;
   FBLOCK *fbp ;
} *fdump_list ;			 /* linked list of all user functions */

void
add_to_fdump_list(FBLOCK *fbp)
{
   struct fdump *p = ZMALLOC(struct fdump) ;
   p->fbp = fbp ;
   p->link = fdump_list ;  fdump_list = p ;
}

void
fdump(void)
{
   register struct fdump *p, *q = fdump_list ;

   while (q)
   {
      p = q ;
      q = p->link ;
      fprintf(stdout, "function %s\n", p->fbp->name) ;
      da(p->fbp->code, stdout) ;
      ZFREE(p) ;
   }
}

