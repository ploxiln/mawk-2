
/********************************************
rexp3.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/*  match a string against a machine   */

#include "rexp.h"
#include "wait.h"



extern RT_STATE *RE_run_stack_base ;
extern RT_STATE *RE_run_stack_limit ;
extern RT_STATE *RE_run_stack_empty ;


#define	 push(mx,sx,ssx,ux)   if (++stackp == RE_run_stack_limit)\
				stackp = RE_new_run_stack() ;\
stackp->m=(mx);stackp->s=(sx);stackp->ss=(ssx);\
stackp->u = (ux)


#define	  CASE_UANY(x)	case  x + U_OFF :  case	 x + U_ON

/* returns start of first longest match and the length by
   reference.  If no match returns NULL and length zero */

char *REmatch(const char* str, size_t str_len ,
              PTR machine, size_t* lenp, Bool no_front_match)
{
   STATE *m = (STATE *) machine ;
   const char *s ;
   const char *ss ;
   register RT_STATE *stackp ;
   int u_flag ;
   const char * const str_end = str + str_len ;

   /* state of current best match stored here */
   const char *cb_ss ;		 /* the start */
   const char *cb_e ;		 /* the end , pts at first char not matched */

   *lenp = 0 ;

   /* check for the easy case */
   if (m->type == M_STR && (m + 1)->type == M_ACCEPT)
   {
      char* ts = str_str(str,str_len,m->data.str,m->len) ;
      if (ts) {
          *lenp = m->len ;
      }
      return ts ;
   }

   s = no_front_match ? str : str-1 ; /* only s == str-1 matches ^ */
   u_flag = U_ON ; cb_e = cb_ss = ss = (char *) 0 ;
   stackp = RE_run_stack_empty ;
   if (wait_queue) empty_wait_queue() ;
   goto reswitch ;

refill :
    if (stackp == RE_run_stack_empty) {
        RTS_Node* wp = wait_queue ;
        if (wp == 0) {
            /* done */
	    if (cb_ss) {
	        *lenp = cb_e - cb_ss ;
	    }
	    return (char*) cb_ss ;
	}
	wait_queue = wp->link ;

	m = wp->state.m ;
	s = wp->state.s ;
	u_flag = wp->state.u ;
	ss = wp->state.ss ;

	wp->link = wait_free_list ;
	wait_free_list = wp ;
	m++ ;
    }
    else {
        m = stackp->m ;
	s = stackp->s ;
	u_flag = stackp->u ;
	ss = stackp->ss ;
	stackp-- ;
    }
    /* does new state start too late ? */
    if (cb_ss) {
        if (ss) {
            if (cb_ss < ss || (cb_ss == ss && cb_e == str_end)) {
	        goto refill ;
	    }
	}
        else if (cb_ss < s || (cb_ss == s && cb_e == str_end)) {
	    goto refill ;
	}
    }

reswitch  :

   switch (m->type + u_flag)
   {
      case M_STR + U_OFF + END_OFF:
	 if (s > str_end || str_end - s < m->len || memcmp(s, m->data.str, m->len) !=0)  {
	     goto refill ;
	 }
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s += m->len ;	m++ ;
	 goto reswitch ;

      case M_STR + U_OFF + END_ON:
	 if (str_end - s != m->len || memcmp(s, m->data.str,m->len) != 0)  {
	     goto refill ;
	 }
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s = str_end+1 ;
	 m++ ;
	 goto reswitch ;

      case M_STR + U_ON + END_OFF:
	 if (s > str_end) goto refill ;
	 if (s < str) s = str ;
	 if (!(s = str_str(s,str_end - s, m->data.str, m->len)))  goto refill ;
	 push(m, s + 1, ss, U_ON) ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s += m->len ; m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_STR + U_ON + END_ON:
         if (s > str_end) goto refill ;
	 if (s < str) s = str ;
	 {
	     int t = (str_end - s) - m->len ;
	     if (t < 0 || memcmp(s = s + t, m->data.str, m->len) != 0) {
	         goto refill ;
	     }
	 }
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s = str_end+1 ; m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_CLASS + U_OFF + END_OFF:
	 if (s >= str_end || !ISON(*m->data.bvp, s[0]))	 goto refill ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s++ ; m++ ;
	 goto reswitch ;

      case M_CLASS + U_OFF + END_ON:
	 if (s+1 != str_end || !ISON(*m->data.bvp, s[0]))	 goto refill ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s = str_end+1 ; m++ ;
	 goto reswitch ;

      case M_CLASS + U_ON + END_OFF:
	 if (s < str) s = str ;
         while(1) {
	     if (s >= str_end) goto refill ;
	     if (ISON(*m->data.bvp, s[0])) break ;
	     s++ ;
	 }
	 push(m, s+1, ss, U_ON) ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s++ ;
	 m++ ;
	 u_flag = U_OFF ;
	 goto reswitch ;

      case M_CLASS + U_ON + END_ON:
	 if (s < str) s = str ;
	 if (s >= str_end || !ISON(*m->data.bvp, str_end[-1])) {
	     goto refill ;
	 }
	 if (!ss)
	 {
	    const char* xs = str_end - 1 ;
	    if (cb_ss && (xs > cb_ss || (xs == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = xs ;
	 }
	 s = str_end+1 ; m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_ANY + U_OFF + END_OFF:
	 if (s >= str_end)  goto refill ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s++ ; m++ ;
	 goto reswitch ;

      case M_ANY + U_OFF + END_ON:
	 if (s+1 != str_end)  goto refill ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s = str_end+1 ; m++ ;
	 goto reswitch ;

      case M_ANY + U_ON + END_OFF:
	 if (s < str) s = str ;
	 if (s >= str_end)  goto refill ;
	 push(m, s+1, ss, U_ON) ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s++ ;
	 m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_ANY + U_ON + END_ON:
	 if (s < str) s = str ;
	 if (s >= str_end) goto refill ;
	 s = str_end - 1 ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 s = str_end+1 ; m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_START + U_OFF + END_OFF:
      case M_START + U_ON + END_OFF:
	 if (s >= str) goto refill ;
	 ss = s = str ;
	 m++ ;	u_flag = U_OFF ;
	 goto reswitch ;

      case M_START + U_OFF + END_ON:
      case M_START + U_ON + END_ON:
         if (s >= str || str != str_end) goto refill ;
	 ss = str ;
	 s = str+1 ;
	 m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_END + U_OFF:
	 if (s != str_end)	 goto refill ;
	 if (!ss)
	 {
	    if (cb_ss) goto refill ;
	    else  ss = str_end ;
	 }
	 s = str_end+1 ;
	 m++ ; goto reswitch ;

      case M_END + U_ON:
	 if (s > str_end) goto refill ;
	 if (!ss)
	 {
	    if (cb_ss) goto refill ;
	    else  ss = str_end ;
	 }
	 s = str_end+1 ;
	 m++ ; u_flag = U_OFF ;
	 goto reswitch ;

       CASE_UANY(M_U):
	 if (s < str) s = str ;
	 if (!ss)
	 {
	    if (cb_ss && (s > cb_ss || (s == cb_ss && cb_e == str_end))) {
	        goto refill ;
	    }
	    else  ss = s ;
	 }
	 u_flag = U_ON ; m++ ;
	 goto reswitch ;

       CASE_UANY(M_1J):
	 m += m->data.jump ;
	 goto reswitch ;

       CASE_UANY(M_2JA):	/* take the non jump branch */
	 push(m + m->data.jump, s, ss, u_flag) ;
	 m++ ;
	 goto reswitch ;

       CASE_UANY(M_2JB):	/* take the non-jump branch */
	 push(m + m->data.jump, s, ss, u_flag) ;
	 m++ ;
	 goto reswitch ;

      CASE_UANY(M_WAIT):
         if (stackp != RE_run_stack_empty || wait_queue != 0) {
	     wait_queue_insert(m,s,u_flag,ss) ;
	     goto refill ;
	 }
	 m++ ;
	 goto reswitch ;

      case M_ACCEPT + U_OFF:
          if (s > str_end) {
	      s = str_end ;
	  }
	  if (!ss) ss = s ;
	  if (!cb_ss || ss < cb_ss || (ss == cb_ss && s > cb_e)) {
	      /* new current best */
	      cb_ss = ss ;
	      cb_e = s ;
	  }
	  goto refill ;

      case M_ACCEPT + U_ON:
	  if (s > str_end) {
	      s = str_end ;
	  }
	  else if (s < str) {
	      s = str ;
	  }
	  if (!ss)  {
	      ss = s ;
	  }
	  else {
	      s = str_end ;
	  }
	  if (!cb_ss || ss < cb_ss || (ss == cb_ss && s > cb_e)) {
	      /* new current best */
	      cb_ss = ss ;
	      cb_e = s ;
	  }
	  goto refill ;

      default:
	 RE_panic("unexpected case in REmatch") ;
   }
   /* cannot get here, shutup compiler warning */
   return 0 ;
}
