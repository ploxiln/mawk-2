/********************************************
re_cmpl.c
copyright 1991-1994,2014-2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

If you import elements of this code into another product,
you agree to not name that product mawk.
********************************************/


/*  re_cmpl.c  */

#include "mawk.h"
#include "memory.h"
#include "scan.h"
#include "regexp.h"
#include "repl.h"
#include "split.h"

typedef struct re_node {
    const STRING *sval;
    PTR re;
    struct re_node *link;
} RE_NODE;

/* a list of compiled regular expressions */
static RE_NODE *re_list;

static char efmt[] = "regular expression compile failed (%s)\n%s";

/* compile a STRING to a regular expression machine.
   Search a list of pre-compiled strings first

   List uses move to the front.  Most reg expressions are compiled
   at compile time, but some are compiled at run time.
   This code:
        BEGIN { var = ... }
	$1 ~ var { .... }

   would cause repeated compilation to an RE machine of the string in var.
   So having that string near or at front of list is beneficial.
*/
PTR
re_compile(const STRING * sval)
{
    RE_NODE *p = re_list;
    RE_NODE *q = 0;

    while (p) {
	if (STRING_eq(sval, p->sval)) {
	    /* found */
	    if (!q)		/* already at front */
		goto _return;
	    else {		/* delete from list for move to front */
		q->link = p->link;
		goto found;
	    }
	} else {
	    q = p;
	    p = p->link;
	}
    }

    /* not found */
    {
	PTR re = REcompile(sval->str, sval->len);
	if (re == 0) {
	    if (mawk_state == EXECUTION) {
		rt_error(efmt,  REerrlist[REerrno], sval->str);
	    } else {		/* compiling */
		compile_error(efmt,  REerrlist[REerrno], sval->str);
		return (PTR) 0;
	    }
	}
	p = ZMALLOC(RE_NODE);
	p->sval = STRING_dup(sval);
	p->re = re;
    }

  found:
    /* insert p at the front of the list */
    p->link = re_list;
    re_list = p;

  _return:

#ifdef DEBUG
    if (dump_RE)
	REmprint(p->re, stderr);
#endif
    return p->re;
}

/* this is only used by da() */

const STRING*
re_uncompile(PTR m)
{
    register RE_NODE *p;

    for (p = re_list; p; p = p->link) {
	if (p->re == m) {
	    return p->sval ;
	}
    }
#ifdef DEBUG
    bozo("non compiled machine");
#else
    return 0 ;
#endif
}

/*=================================================*/
/*  replacement	 operations   */

typedef struct repl_node {
    struct repl_node *link;
    const STRING *sval;		/* the input */
    unsigned type;		/* C_REPL or C_REPLV */
    void *ptr;			/* STRING* or Replv_Data* */
} Repl_Node;

static Repl_Node *repl_list;

/*
 *  For sub(/re/, "ab...",[opt var]),
 *  the string "ab..." gets scanned for & and \
 *  The results are stored in repl_list.
 *
 *  A string constant, e.g. "my&string", get scanned during
 *  compilation.  As opposed to,
 *
 *       { gsub(/re/, myvar)
 *         ....
*        }
*
*   The contents of myvar are scanned at each execution, so if myvar
*   doesn't change the scan can be looked up instead of redone.
*
*   Move to the front, pushes compile time scans to the back and
*   run time scans are more to the front.
*/

/* does the scan when lookup fails */
static void do_replacement_scan(const STRING *, CELL *);

/* does lookup with move to front. When lookup fails, calls
 * do_replacement_scan()
 *
 *
 * results are put in *cp which caller has destroyed if needed
*/
void
replacement_scan(const STRING * sval, CELL *cp)
{
    Repl_Node dummy;
    Repl_Node *q = &dummy;
    Repl_Node *p = repl_list;	/* q trails p for move to front */
    dummy.link = p;

    while (p) {
	if (STRING_eq(sval, p->sval)) {
	    /* found */
	    if (p != repl_list) {
		/* delete and move to front */
		q->link = p->link;
		p->link = repl_list;
		repl_list = p;
	    }
	    cp->type = p->type;
	    if (p->type == C_REPL) {
		cp->ptr = STRING_dup(p->ptr);
	    } else {
		cp->ptr = p->ptr;
	    }
	    return;
	} else {
	    q = q->link;
	    p = p->link;
	}
    }
    /* not in list, have to really scan it */
    do_replacement_scan(sval, cp);
    p = (Repl_Node *)zmalloc(sizeof(Repl_Node));
    p->sval = STRING_dup(sval);
    p->type = cp->type;
    if (cp->type == C_REPL) {
	p->ptr = STRING_dup(cp->ptr);
    } else {
	p->ptr = cp->ptr;
    }
    /* insert at front of list */
    p->link = repl_list;
    repl_list = p;
}

/* A CELL of type C_REPLV points at Replv_Data.
 * Replv_Data is created here.
*/

static Replv_Data *
create_replv_data(STRING ** pieces, unsigned cnt)
{
    unsigned i = 0;
    Replv_Data *ret = (Replv_Data *) zmalloc(sizeof(Replv_Data));
    ret->cnt = cnt;
    ret->amp_cnt = 0;
    ret->piece_len = 0;
    ret->pieces = (STRING **) zmalloc(cnt * sizeof(STRING *));
    for (; i < cnt; i++) {
	ret->pieces[i] = pieces[i];
	if (pieces[i] == 0) {
	    ret->amp_cnt++;
	} else {
	    ret->piece_len += pieces[i]->len;
	}
    }
    return ret;
}

#ifdef MEM_CHECK
static void
destroy_replv_data(Replv_Data * p)
{
    unsigned i = 0;
    for (; i < p->cnt; i++) {
	if (p->pieces[i]) {
	    free_STRING(p->pieces[i]);
	}
    }
    zfree(p->pieces, p->cnt * sizeof(STRING *));
    zfree(p, sizeof(Replv_Data));
}
#endif

static void
  do_replacement_scan_posix(const STRING *, CELL *);
static void
  do_replacement_scan_awk(const STRING *, CELL *);

int posix_repl_scan_flag = 0;

static void
do_replacement_scan(const STRING * sval, CELL *cp)
{
    if (posix_repl_scan_flag) {
	do_replacement_scan_posix(sval, cp);
    } else {
	do_replacement_scan_awk(sval, cp);
    }
}

/*     \\  --> \
       \&  --> literal &
       &    is replaced by the matched string
*/
static void
do_replacement_scan_posix(const STRING * sval, CELL *cp)
{
    const char *str = sval->str;
    const char *str_end = str + sval->len;

    /* handle usual case */
    if (memchr(str, '&', sval->len) == 0 && memchr(str, '\\', sval->len) == 0) {
	cp->type = C_REPL;
	cp->ptr = STRING_dup(sval);
    } else {
	char *buffer = (char *) zmalloc(sval->len);
	STRING **sbuff = (STRING **) zmalloc(sval->len * sizeof(STRING *));

	unsigned idx = 0;	/* index into sbuff */
	char *bp = buffer;	/* walks buffer */
	const char *s = str;	/* walks str */

	while (s < str_end) {
	    if (*s == '&') {
		if (bp > buffer) {
		    /* stuff in front of & */
		    sbuff[idx++] = new_STRING2(buffer, bp - buffer);
		}
		sbuff[idx++] = 0;	/* for the & */
		s++;
		/* got a piece, so reset bp */
		bp = buffer;
	    } else if (*s == '\\' && ((s[1] == '&' || s[1] == '\\'))) {
		*bp++ = s[1];
		s += 2;
	    } else {
		*bp++ = *s++;
	    }
	}
	/* handle last piece */
	if (bp > buffer) {
	    sbuff[idx++] = new_STRING2(buffer, bp - buffer);
	}

	if (idx == 1 && sbuff[0] != 0) {
	    cp->type = C_REPL;
	    cp->ptr = sbuff[0];
	} else {
	    cp->type = C_REPLV;
	    cp->ptr = create_replv_data(sbuff, idx);
	}
	zfree(buffer, sval->len);
	zfree(sbuff, sval->len * sizeof(STRING *));
    }
}

/*
      If the string has no &'s, then unchanged.
      \\  --> \ only if the run ends in &
      2*n+1 \'s followed by & becomes n \'s and literal &
      2*n   \'s followd by & becomes n \'s and & that's replaced by match
*/
static void
do_replacement_scan_awk(const STRING * sval, CELL *cp)
{
    const char *str = sval->str;
    const char *str_end = str + sval->len;

    /* handle usual case */
    if (memchr(str, '&', sval->len) == 0) {
	cp->type = C_REPL;
	cp->ptr = STRING_dup(sval);
    } else {
	char *buffer = (char *) zmalloc(sval->len);
	STRING **sbuff = (STRING **) zmalloc(sval->len * sizeof(STRING *));

	unsigned idx = 0;	/* index into sbuff */
	char *bp = buffer;	/* walks buffer */
	const char *s = str;	/* walks str */

	while (s < str_end) {
	    if (*s == '&') {
		if (bp > buffer) {
		    /* stuff in front of & */
		    sbuff[idx++] = new_STRING2(buffer, bp - buffer);
		}
		sbuff[idx++] = 0;	/* for the & */
		s++;
		/* got a piece, so reset bp */
		bp = buffer;
	    } else if (*s == '\\') {
		/* find end of run of \ */
		const char *q = s + 1;
		while (q < str_end && *q == '\\') {
		    q++;
		}
		if (q == str_end || *q != '&') {
		    /* \'s do nothing */
		    while (s < q) {
			*bp++ = '\\';
			s++;
		    }
		} else {
		    /* \\ --> \ and \& --> literal & */
		    while (s < q) {
			*bp++ = s[1];
			s += 2;
		    }
		}
	    } else {
		*bp++ = *s++;
	    }
	}
	/* handle last piece */
	if (bp > buffer) {
	    sbuff[idx++] = new_STRING2(buffer, bp - buffer);
	}

	if (idx == 1 && sbuff[0] != 0) {
	    cp->type = C_REPL;
	    cp->ptr = sbuff[0];
	} else {
	    cp->type = C_REPLV;
	    cp->ptr = create_replv_data(sbuff, idx);
	}
	zfree(buffer, sval->len);
	zfree(sbuff, sval->len * sizeof(STRING *));
    }
}

/* return the STRING* for a CELL of type REPL or REPLV,
   This is only used by da()
*/

const STRING *
repl_unscan(CELL *cp)
{
    Repl_Node *p = repl_list;
    while (p) {
	if (p->ptr == cp->ptr) {
	    return p->sval;
	} else {
	    p = p->link;
	}
    }
#ifdef DEBUG
    bozo("unable to uncompile an repl");
#else
    return 0;
#endif
}

#ifdef MEM_CHECK

static void
free_RE_NODE(RE_NODE * p)
{
    free_STRING((STRING*)p->sval);
    /*REdestroy(p->re);*/
    zfree(p, sizeof(RE_NODE));
}

static void
free_Repl_Node(Repl_Node * p)
{
    free_STRING((STRING *) p->sval);
    if (p->type == C_REPL) {
	free_STRING((STRING *) p->ptr);
    } else {
	destroy_replv_data((Replv_Data *) p->ptr);
    }
    zfree(p, sizeof(Repl_Node));
}

void
free_re_repl_list(void)
{
    while (re_list != 0) {
	RE_NODE *next = re_list->link;
	free_RE_NODE(re_list);
	re_list = next;
    }

    while (repl_list != 0) {
	Repl_Node *next = repl_list->link;
	free_Repl_Node(repl_list);
	repl_list = next;
    }
}
#endif
