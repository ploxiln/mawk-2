
/********************************************
rexp2.c
copyright 1991,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/




/*  test a string against a machine   */

#include "rexp.h"
#include "wait.h"

#define	 STACKGROWTH	16

#ifdef	DEBUG
static RT_STATE* slow_push(RT_STATE *, STATE *, const char *, int) ;
#endif


RT_STATE *RE_run_stack_base ;
RT_STATE *RE_run_stack_limit ;

/* Large model DOS segment arithemetic breaks the current stack.
   This hack fixes it without rewriting the whole thing, 5/31/91 */
RT_STATE *RE_run_stack_empty ;

void
RE_run_stack_init()
{
   if (!RE_run_stack_base)
   {
      RE_run_stack_base = (RT_STATE *)
      RE_malloc(sizeof(RT_STATE) * STACKGROWTH) ;
      RE_run_stack_limit = RE_run_stack_base + STACKGROWTH ;
      RE_run_stack_empty = RE_run_stack_base - 1 ;
   }
}

RT_STATE *
RE_new_run_stack()
{
   int oldsize = RE_run_stack_limit - RE_run_stack_base ;
   int newsize = oldsize + STACKGROWTH ;


   RE_run_stack_base = (RT_STATE *) realloc(RE_run_stack_base,
					newsize * sizeof(RT_STATE)) ;

   if (!RE_run_stack_base)
   {
      fprintf(stderr, "out of memory for RE run time stack\n") ;
      /* this is pretty unusual, I've only seen it happen on
       weird input to REmatch() under 16bit DOS , the same
       situation worked easily on 32bit machine.  */
      exit(100) ;
   }

   RE_run_stack_limit = RE_run_stack_base + newsize ;
   RE_run_stack_empty = RE_run_stack_base - 1 ;

   /* return the new stackp */
   return RE_run_stack_base + oldsize ;
}

#ifdef	DEBUG
static RT_STATE *
slow_push(RT_STATE* sp, STATE* m, const char* s, int u)
{
   if (sp == RE_run_stack_limit)  sp = RE_new_run_stack() ;
   sp->m = m ; sp->s = s ; sp->u = u ;
   return sp ;
}
#endif

#ifdef	 DEBUG
#define	 push(mx,sx,ux)	  stackp = slow_push(++stackp, mx, sx, ux)
#else
#define	 push(mx,sx,ux)	  if (++stackp == RE_run_stack_limit)\
				stackp = RE_new_run_stack() ;\
stackp->m=(mx);stackp->s=(sx);stackp->u=(ux)
#endif


#define	  CASE_UANY(x)	case  x + U_OFF :  case	 x + U_ON

/* test if str ~ /machine/
*/

int
REtest(const char* str, size_t len, PTR machine)
{
   register STATE *m = (STATE *) machine ;
   register const char *s = str ;
   register RT_STATE *stackp ;
   int u_flag ;
   const char *const str_end = str + len ;

   /* handle the easy case quickly */
   if (m->type == M_STR && (m+1)->type == M_ACCEPT) {
      return str_str(s, len, m->data.str, m->len) != (char *) 0 ;
   }
   else
   {
      s = str - 1 ;      /* ^ can be matched  */
      u_flag = U_ON ;
      stackp = RE_run_stack_empty ;
      if (wait_queue) empty_wait_queue() ;
      goto reswitch ;
   }

refill :
   if (stackp == RE_run_stack_empty) {
       RTS_Node* wp ;
       if (wait_queue == 0) return 0 ;
       wp = wait_queue ;
       wait_queue = wp->link ;

       m = wp->state.m ;
       s = wp->state.s ;
       u_flag = wp->state.u ;
       /* put wp on free list */
       wp->link = wait_free_list ;
       wait_free_list = wp ;
       m++ ;
   }
   else {
       m = stackp->m ;
       s = stackp->s ;
       u_flag = stackp->u ;
       stackp-- ;
   }

reswitch  :

   switch (m->type + u_flag)
   {
      case M_STR + U_OFF + END_OFF:
	if (s > str_end || (size_t)(str_end - s) < m->len || memcmp(s, m->data.str, m->len))  {
	     goto refill ;
	 }
	 s += m->len ;	m++ ;
	 goto reswitch ;

      case M_STR + U_OFF + END_ON:
	 if ((size_t)(str_end - s) != m->len || memcmp(s,m->data.str,m->len) != 0) {
	     goto refill ;
	 }
	 s = str_end+1 ;
	 m++ ;
	 goto reswitch ;

      case M_STR + U_ON + END_OFF:
	 if (s > str_end) goto refill ;
         if (s < str) s = str ;
	 if (!(s = str_str(s, (size_t)(str_end - s) , m->data.str, m->len)))  goto refill ;
	 push(m, s + 1, U_ON) ;
	 s += m->len ; m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_STR + U_ON + END_ON:
	 if (s > str_end) goto refill ;
         if (s < str) s = str ;
	 {
	     int t = ((size_t)(str_end - s)) - m->len ;
	     if (t < 0 || memcmp(s + t, m->data.str, m->len) != 0) {
	         goto refill ;
	     }
	 }
	 s = str_end+1 ;
	 m++ ;
	 u_flag = U_OFF ;
	 goto reswitch ;

      case M_CLASS + U_OFF + END_OFF:
	 if (s >= str_end || !ISON(*m->data.bvp, s[0]))	 goto refill ;
	 s++ ; m++ ;
	 goto reswitch ;

      case M_CLASS + U_OFF + END_ON:
	 if (s+1 != str_end || !ISON(*m->data.bvp, s[0]))  goto refill ;
	 s = str_end+1 ;
	 m++ ;
	 goto reswitch ;

      case M_CLASS + U_ON + END_OFF:
         if (s < str) s = str ;
	 while (1) {
	    if (s >= str_end) goto refill ;
	    if (ISON(*m->data.bvp,s[0])) break ;
	    s++ ;
	 }
	 s++ ;
	 push(m, s, U_ON) ;
	 m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_CLASS + U_ON + END_ON:
	 if (s < str) s = str ;
	 if (s >= str_end || !ISON(*m->data.bvp, str_end[-1]))
	    goto refill ;
	 s = str_end+1 ; m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_ANY + U_OFF + END_OFF:
	 if (s >= str_end)  goto refill ;
	 s++ ; m++ ;
	 goto reswitch ;

      case M_ANY + U_OFF + END_ON:
	 if (s + 1 != str_end)  goto refill ;
	 s = str_end+1 ;
	 m++ ;
	 goto reswitch ;

      case M_ANY + U_ON + END_OFF:
	 if (s < str) s = str ;
	 if (s >= str_end)  goto refill ;
	 s++ ;
	 push(m, s, U_ON) ;
	 m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_ANY + U_ON + END_ON:
         if (s < str) s = str ;
	 if (s >= str_end)  goto refill ;
	 s = str_end+1 ; m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_START + U_OFF + END_OFF:
      case M_START + U_ON + END_OFF:
	 if (!(s < str))  goto refill ;
	 s = str ;
	 m++ ;	u_flag = U_OFF ;
	 goto reswitch ;

      case M_START + U_OFF + END_ON:
      case M_START + U_ON + END_ON:
	 if (!(s < str) || str != str_end)  goto refill ;
	 s = str_end+1 ;
	 m++ ; u_flag = U_OFF ;
	 goto reswitch ;

      case M_END + U_OFF:
	 if (s != str_end)  goto refill ;
	 s++ ;
	 m++ ;
	 goto reswitch ;

      case M_END + U_ON:
         if (s > str_end) goto refill ;
	 s = str_end+1 ;
	 m++ ; u_flag = U_OFF ;
	 goto reswitch ;

       CASE_UANY(M_U):
          if (s < str) s = str ;
	  u_flag = U_ON ; m++ ;
	  goto reswitch ;

       CASE_UANY(M_1J):
	 m += m->data.jump ;
	 goto reswitch ;

       CASE_UANY(M_2JA):	/* take the non jump branch */
	 push(m + m->data.jump, s, u_flag) ;
	 m++ ;
	 goto reswitch ;

       CASE_UANY(M_2JB):
	 push(m + m->data.jump, s, u_flag) ;
	 m++ ;
	 goto reswitch ;

       CASE_UANY(M_WAIT):
         if (stackp != RE_run_stack_empty || wait_queue != 0) {
	     wait_queue_insert(m,s,u_flag,0) ;
	     goto refill ;
	 }
	 m++ ;
	 goto reswitch ;

       CASE_UANY(M_ACCEPT):
	 return 1 ;

      default:
	 RE_panic("unexpected case in REtest") ;
   }
   /* not reached, shutup -Wall */
   return 0 ;
}



#ifdef	MAWK

char *
is_string_split(PTR q, size_t* lenp)
{
   STATE *p;

   p = (STATE *)q;
   if (p && p[0].type == M_STR && p[1].type == M_ACCEPT)
   {
      *lenp = p->len ;
      return p->data.str ;
   }
   else	 return (char *) 0 ;
}
#else /* mawk provides its own str_str */

char *
str_str(target, key, klen)
   register char *target ;
   register char *key ;
   unsigned klen ;
{
   int c = key[0] ;

   switch (klen)
   {
      case 0:
	 return (char *) 0 ;

      case 1:
	 return strchr(target, c) ;

      case 2:
	 {
	    int c1 = key[1] ;

	    while (target = strchr(target, c))
	    {
	       if (target[1] == c1)  return target ;
	       else  target++ ;
	    }
	    break ;
	 }

      default:
	 klen-- ; key++ ;
	 while (target = strchr(target, c))
	 {
	    if (memcmp(target + 1, key, klen) == 0)  return target ;
	    else  target++ ;
	 }
	 break ;
   }
   return (char *) 0 ;
}


#endif /* MAWK */
