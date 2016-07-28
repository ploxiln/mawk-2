/* printf.c */
/*
copyright 2016 Michael D. Brennan

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 3, 2007.

printf.c and printf.h were generated with the commands

   notangle -R'"printf.c"' printf.w > printf.c 
   notangle -R'"printf.h"' printf.w > printf.h 

Notangle is part of Norman Ramsey's noweb literate programming package.
Noweb home page: http://www.cs.tufts.edu/~nr/noweb/

It's easiest to read or modify this file by working with printf.w.
*/


#include <stdarg.h>
#include "mawk.h"
#include "scan.h"
#include "printf.h"
#include "int.h"

static Form* the_forms ;
static STRING the_nl = {1,1,"\n"} ;
static STRING* const the_nl_str = &the_nl ;
struct spec {
    Spec* link ;
    unsigned type ;
    const STRING* prefix ;
    int width ;
    int prec ;
    int minus_flag ;
    unsigned ast_cnt ;
    const char* form ;
} ;
enum { WP_AST = -1, WP_NONE = -2 } ;
    
enum { PF_C = 0, PF_I, PF_U, PF_F, PF_S } ;
static Form* find_form(const STRING*) ;

static Form* parse_form0(const STRING*) ;

static const char* parse_spec(const char*, const char*, Spec*) ;

static void xprint_string(const char*,size_t,FILE*) ;
static void xprint_spaces(size_t,FILE*) ;

static void xprint_string_spec(FILE*,const Spec*,CELL*) ;

static void xprint_num_spec(FILE*, const Spec*, CELL*) ;

static void xprint_pf_f(FILE*,const char*,unsigned,int,int,double) ;

static void xprint_pf_i(FILE*,const char*,unsigned,int,int, int64_t) ;

static void xprint_pf_u(FILE*,const char*,unsigned,int,int, uint64_t) ;

static void xprint_pf_c(FILE*,const char*,unsigned,int,int, unsigned) ;

static void snprintf_bozo(void) ;

static void c_or_rt_error_or_silence(const char*,...) ;


char* sprintf_ptr ;
static int checking_convfmt = 0 ;
static Form* find_form(const STRING* form)
{
    Form* p = the_forms ;
    Form* q = 0 ;
    while(p) {
        if (STRING_eq(form,p->form)) {
            if (q) { /* move to the front */
                q->link = p->link ;
                p->link = the_forms ;
                the_forms = p ;
            }
            return p ;
        }
        else {
            q = p ;
            p = p->link ;
        }
    }
    /* not found */
    return 0 ;
}
const Form* parse_form(const STRING* form)
{
    Form* ret = find_form(form) ;
    if (ret) return ret ;
    ret = parse_form0(form) ;
    /* zero if an error */
    if (ret) {
        ret->link = the_forms ;
        the_forms = ret ;
    }
    return ret ;
}
static Form* parse_form0(const STRING* form)
{
    STRING* prefix ;
    Spec tmp_head ;
    Spec* tail = &tmp_head ;
    char xbuffer[1024] ;
    char* buffer = form->len <= 1024 ? xbuffer : (char*)emalloc(form->len) ;
    const char* str = form->str ; /* walks form */
    const char* end = str + form->len ;

    tmp_head.link = 0 ;
    Form* ret = (Form*)zmalloc(sizeof(Form)) ;

    while(1) {
        {
            char* bp = buffer ;
            prefix = 0 ;
            while(str < end) {
                if (*str == '%') {
                    if (str[1] == '%') {
                        *bp++ = '%' ;
                        str += 2 ;
                        continue ;
                    }
                    else { /* prefix is done */
                        if (bp > buffer) {
                            prefix = new_STRING2(buffer, bp - buffer) ;
                            bp = buffer ;
                        }
                        break ;
                    }
                }
                else *bp++ = *str++ ;
            }
            if (bp > buffer) {
                /* prefix is really the ending */
                if (bp == buffer+1 && buffer[0] == '\n') {
                    prefix = STRING_dup(the_nl_str) ;
                }
                else {
                    prefix = new_STRING2(buffer, bp - buffer) ;
                }
            }
        }
        if (str == end) 
            {
                ret->ending = prefix ;
                ret->form = STRING_dup(form) ;
                ret->link = 0 ;
                ret->specs = tmp_head.link ;
                ret->num_args = 0 ;
                {
                    const Spec* sp = ret->specs ;
                    while(sp) {
                        ret->num_args += 1 + sp->ast_cnt ;
                        sp = sp->link ;
                    }
                }
                if (buffer != xbuffer) free(buffer) ;
                return ret ;
            }


        else {
            str = parse_spec(str+1,end,tail) ;
            if (str == 0) {
                /* error, msg already done */
                return 0 ;
            }
            tail = tail->link ;
            tail->prefix = prefix ;
        }
    }
}
const char* parse_spec(const char* str, const char* end, Spec* tail)
{
    const char* const start = str-1 ;
    Spec* spec = (Spec*)zmalloc(sizeof(Spec)) ;
    spec->link = 0 ;
    spec->width = WP_NONE ;
    spec->prec  = WP_NONE ;
    spec->minus_flag = 0 ;
    spec->ast_cnt = 0 ;
    spec->form = 0 ;
    unsigned l_cnt = 0 ;

    {
        while(str < end) {
            switch(*str) {
                case '-' : 
                    spec->minus_flag = 1 ;
                    /* fall thru */
                case '+' : case ' ' : case '#' : case '0' :
                    str++ ;
                    continue ;
                default:
                    break ;
            }
            break ; /* while */
        }
        if (str == end) {
            c_or_rt_error_or_silence(
                "incomplete format specification \"%s\"", start) ;
            {
                return 0 ;
            }

        }
    }

    {
        if (*str == '*') {
            spec->width = WP_AST ;
            spec->ast_cnt = 1 ;
            str++ ;
        }
        else {
            unsigned char c = *(unsigned char*) str ;

            if (scan_code[c] == SC_DIGIT) {
                spec->width = c - '0' ;
                str++ ;
                while(1) {
                    c = *(unsigned char*) str ;
                    if (scan_code[c] != SC_DIGIT) break ;
                    spec->width = spec->width * 10 + c - '0' ;
                    str++ ;
                }
            }
        }
        /* width is done */
        if (*str == '.') {
            /* have precision , no numbers will be prec 0 */
            str++ ;
            if (*str == '*') {
                spec->prec = WP_AST ;
                spec->ast_cnt++ ;
                str++ ;
            }
            else {
                unsigned char c ;
                spec->prec = 0 ;
                while(1) {
                    c = * (unsigned char*) str ;
                    if (scan_code[c] != SC_DIGIT) break ;
                    spec->prec = spec->prec * 10 + c - '0' ;
                    str++ ;
                }
            }
        }
    }

    {
        while(*str == 'l') {
            str++ ;
            l_cnt++ ;
        }
    }

    {
        switch(*str) {
            case 's':
                spec->type = PF_S ;
                break ;
            case 'c':
            case 'e': case 'g': case 'f': case 'E': case 'G':
                {
                    size_t len = str+1 - start - l_cnt ;
                    char* form = (char*) zmalloc(len+1) ;
                    spec->type = *str == 'c' ? PF_C : PF_F ;
                    memcpy(form, start, len-1) ;
                    form[len] = 0 ;
                    form[len-1] = *str ;
                    spec->form = form ;
                }

                break ;
            case 'd': case 'i':
            case 'o': case 'x': case 'X': case 'u':
                {
                    size_t len = str+1 - start ;
                    int delta = (have_long64? 1 : 2) - l_cnt ;
                        /* delta is number of 'l' to add or remove */
                    char* form ;

                    len += delta ;
                    form = (char*) zmalloc(len+1) ;
                    if (*str == 'd' || *str == 'i') {
                        spec->type = PF_I ;
                    }
                    else {
                        spec->type = PF_U ;
                    }
                    form[len] = 0 ;
                    form[len-1] = *str ;
                    form[len-2] = 'l' ;
                    if (!have_long64) {
                        form[len-3] = 'l' ;
                    }
                    memcpy(form, start, len - (have_long64?2:3)) ;
                    spec->form = form ;
                }

                   
                break ;
            default:
               c_or_rt_error_or_silence(
      "invalid control character '%c' in [s]printf format (\"%s\")",
               *(unsigned char*) str, start) ;
               {
                   return 0 ;
               }

               break ;
        }
    }

    tail->link = spec ;
    return str+1 ;
}
STRING*  do_xprintf(
    FILE* fp,
    const Form* form,
    CELL* cp)
{
    const Spec* spec = form->specs ;

    sprintf_ptr = string_buff ;
    while(spec) {
        if (spec->prefix) {
            xprint_string(spec->prefix->str,spec->prefix->len,fp) ;
        }
        if (spec->type == PF_S) {
            xprint_string_spec(fp,spec,cp) ;
        }
        else {
            xprint_num_spec(fp,spec,cp) ;
        }
        cp +=  1 + spec->ast_cnt ;
        spec = spec->link ;
    }
    if (form->ending) {
        xprint_string(form->ending->str, form->ending->len, fp) ;
    }
    if (fp) {
        if (ferror(fp)) {
            write_error(fp) ;
            mawk_exit(2) ;
        }
        return 0 ;
    }
    return new_STRING2(string_buff, sprintf_ptr - string_buff) ;
}
static void xprint_string(const char* str, size_t len,  FILE* fp) 
{
    if (fp) {
        fwrite(str, 1, len, fp) ;
    }
    else {
        size_t room = string_buff_end - sprintf_ptr ;
        while(room < len) {
            sprintf_ptr = enlarge_string_buff(sprintf_ptr) ;
            room = string_buff_end - sprintf_ptr ;
        }
        memcpy(sprintf_ptr, str, len) ;
        sprintf_ptr += len ;
    }
}

static void xprint_spaces(size_t len, FILE* fp)
{
    char buffer[1024] ;
    memset(buffer, ' ', len > 1024 ? 1024 : len) ;
    while(len > 1024) {
        xprint_string(buffer, 1024, fp) ;
        len -= 1024 ;
    }
    xprint_string(buffer, len, fp) ;
}
static 
void xprint_string_spec(FILE* fp, const Spec* spec, CELL* cp)
{
    int width = spec->width ;
    int prec = spec->prec ;
    const char* str ;
    size_t len ;
    int minus_flag = spec->minus_flag ;

    {
        if (width == WP_AST) {
            cast1_to_d(cp) ;
            width = d_to_int(cp->dval) ;
            if (width < 0) {
                minus_flag = 1 ;
                width = -width ;
            }
            cp++ ;
        }
        if (prec == WP_AST) {
            cast1_to_d(cp) ;
            prec = d_to_int(cp->dval) ;
            cp++ ;
        }
    }

    cast1_to_s(cp) ;
    str = string(cp)->str ;
    len = string(cp)->len ;
    
    if (width < 0) width = 0 ; /* effectively ignore */
    if (prec < 0)  prec = len ; /* ditto */
    if (prec < len) len = prec ;

    /* output str and spaces if needed */
    if (len >= width) {
       xprint_string(str,len,fp) ;
    }
    else if (minus_flag) {
        /* left justify */
        xprint_string(str,len,fp) ;
        xprint_spaces(width-len,fp) ;
    }
    else {
        xprint_spaces(width-len,fp) ;
        xprint_string(str,len,fp) ;
    }
}
static
void xprint_num_spec(FILE* fp, const Spec* spec, CELL* cp)
{
    unsigned ast_cnt = spec->ast_cnt ;
    int ast[2] ;

    {
        if (ast_cnt >= 1)  {
            cast1_to_d(cp) ;
            ast[0] = d_to_int(cp->dval) ;
            cp++ ;
        }
        if (ast_cnt == 2) {
            cast1_to_d(cp) ;
            ast[1] = d_to_int(cp->dval) ;
            cp++ ;
        }
    }


    switch(spec->type) {
        case PF_F:
            if (cp->type != C_DOUBLE) cast1_to_d(cp) ;
            xprint_pf_f(fp, spec->form, ast_cnt, ast[0],ast[1],cp->dval) ;
            break ;

        case PF_I:
            if (cp->type != C_DOUBLE) cast1_to_d(cp) ;
            xprint_pf_i(fp, spec->form, ast_cnt, ast[0],ast[1],
                        d_to_i64(cp->dval)) ;
            break ;

        case PF_U:
            if (cp->type != C_DOUBLE) cast1_to_d(cp) ;
            xprint_pf_u(fp, spec->form, ast_cnt, ast[0],ast[1],
                        d_to_u64(cp->dval)) ;
            break ;

        case PF_C:
            {
                unsigned c ;
                {
                    int64_t val ;

                    if (cp->type == C_MBSTRN) {
                        check_strnum(cp) ;
                    }
                    if (cp->type == C_STRING) {
                        c = string(cp)->str[0] ;
                    }
                    else {
                        cast1_to_d(cp) ;
                        val = d_to_i64(cp->dval) ;
                        c = (unsigned) (val & 0xff) ;
                    }
                }

                xprint_pf_c(fp, spec->form, ast_cnt, ast[0],ast[1],c) ;
            }
            break ;
    }
}
static void xprint_pf_f(FILE* fp, const char* form, 
    unsigned cnt, int x, int y, double d)
{
    if (fp) {
        switch(cnt) {
            case 0:
                fprintf(fp, form, d) ;
                break ;
            case 1:
                fprintf(fp, form, x, d) ;
                break ;
            case 2:
                fprintf(fp, form, x, y, d) ;
                break ;
        }
    }
    else {
        size_t room = string_buff_end - sprintf_ptr ;
        size_t used = (size_t)-1 ;

        while(1) {
            switch(cnt) {
                case 0:
                    used = snprintf(sprintf_ptr, room, form, d) ;
                    break ;
                case 1:
                    used = snprintf(sprintf_ptr, room, form, x ,d) ;
                    break ;
                case 2:
                    used = snprintf(sprintf_ptr, room, form, x,y,d) ;
                    break ;
            }
            if ((int) used < 0) snprintf_bozo() ;
            
            if (used <= room) break ; /* while */
            do {
                sprintf_ptr = enlarge_string_buff(sprintf_ptr) ;
                room = string_buff_end - sprintf_ptr ;
            }
            while(room < used) ;
        }
        sprintf_ptr += used ;
    }
}
static void xprint_pf_i(FILE* fp, const char* form, 
    unsigned cnt, int x, int y, int64_t val)
{
    if (fp) {
        switch(cnt) {
            case 0:
                fprintf(fp, form, val) ;
                break ;
            case 1:
                fprintf(fp, form, x, val) ;
                break ;
            case 2:
                fprintf(fp, form, x, y, val) ;
                break ;
        }
    }
    else {
        size_t room = string_buff_end - sprintf_ptr ;
        size_t used = (size_t)-1 ;

        while(1) {
            switch(cnt) {
                case 0:
                    used = snprintf(sprintf_ptr, room, form, val) ;
                    break ;
                case 1:
                    used = snprintf(sprintf_ptr, room, form, x ,val) ;
                    break ;
                case 2:
                    used = snprintf(sprintf_ptr, room, form, x,y,val) ;
                    break ;
            }
            if ((int) used < 0) snprintf_bozo() ;
            
            if (used <= room) break ; /* while */
            do {
                sprintf_ptr = enlarge_string_buff(sprintf_ptr) ;
                room = string_buff_end - sprintf_ptr ;
            }
            while(room < used) ;
        }
        sprintf_ptr += used ;
    }
}
static void xprint_pf_u(FILE* fp, const char* form, 
    unsigned cnt, int x, int y, uint64_t val)
{
    if (fp) {
        switch(cnt) {
            case 0:
                fprintf(fp, form, val) ;
                break ;
            case 1:
                fprintf(fp, form, x, val) ;
                break ;
            case 2:
                fprintf(fp, form, x, y, val) ;
                break ;
        }
    }
    else {
        size_t room = string_buff_end - sprintf_ptr ;
        size_t used = (size_t)-1 ;

        while(1) {
            switch(cnt) {
                case 0:
                    used = snprintf(sprintf_ptr, room, form, val) ;
                    break ;
                case 1:
                    used = snprintf(sprintf_ptr, room, form, x ,val) ;
                    break ;
                case 2:
                    used = snprintf(sprintf_ptr, room, form, x,y,val) ;
                    break ;
            }
            if ((int) used < 0) snprintf_bozo() ;
            
            if (used <= room) break ; /* while */
            do {
                sprintf_ptr = enlarge_string_buff(sprintf_ptr) ;
                room = string_buff_end - sprintf_ptr ;
            }
            while(room < used) ;
        }
        sprintf_ptr += used ;
    }
}
static void xprint_pf_c(FILE* fp, const char* form, 
    unsigned cnt, int x, int y, unsigned val)
{
    if (fp) {
        switch(cnt) {
            case 0:
                fprintf(fp, form, val) ;
                break ;
            case 1:
                fprintf(fp, form, x, val) ;
                break ;
            case 2:
                fprintf(fp, form, x, y, val) ;
                break ;
        }
    }
    else {
        size_t room = string_buff_end - sprintf_ptr ;
        size_t used = (size_t)-1 ;

        while(1) {
            switch(cnt) {
                case 0:
                    used = snprintf(sprintf_ptr, room, form, val) ;
                    break ;
                case 1:
                    used = snprintf(sprintf_ptr, room, form, x ,val) ;
                    break ;
                case 2:
                    used = snprintf(sprintf_ptr, room, form, x,y,val) ;
                    break ;
            }
            if ((int) used < 0) snprintf_bozo() ;
            
            if (used <= room) break ; /* while */
            do {
                sprintf_ptr = enlarge_string_buff(sprintf_ptr) ;
                room = string_buff_end - sprintf_ptr ;
            }
            while(room < used) ;
        }
        sprintf_ptr += used ;
    }
}
static void snprintf_bozo() {
    rt_error("unexpected error (%d) in internal call to snprintf", errno) ;
    /* no return */
}
void visible_string(FILE*, const STRING*, int) ; /* in da.c */
static void da_Spec(FILE*, const Spec*) ;
void da_Form(FILE* fp, const Form* form)
{
    const Spec* spec = form->specs ;
    while(spec) {
        da_Spec(fp,spec) ;
        spec = spec->link ;
    }
    if (form->ending) {
        visible_string(fp, form->ending,'\"') ;
    }
}
static void da_Spec(FILE* fp, const Spec* spec)
{
    if (spec->prefix) visible_string(fp, spec->prefix, '\"') ;
    if (spec->type != PF_S) {
        fprintf(fp, "%s", spec->form) ;
    }
    else {
        fputc('%', fp) ;
        if (spec->minus_flag) fputc('-',fp) ;
        if (spec->width == WP_AST) {
            fputc('*',fp) ;
        }
        else if (spec->width >= 0) {
            fprintf(fp, "%d", spec->width) ;
        }
        if (spec->prec >= 0) {
            fprintf(fp, ".%d", spec->prec) ;
        }
        else if (spec->prec == WP_AST) {
            fputs(".*", fp) ;
        }
        fputc('s', fp) ;
    }
}
STRING* check_convfmt(const STRING* conv)
{
    const Form* form ;
    checking_convfmt = 1 ;
    form = parse_form(conv) ;
    checking_convfmt = 0 ;
    if (form == 0 || form->num_args != 1 || form->specs->type != PF_F) {
        return 0 ;
    }
    /* no \0 in prefix or ending */
    if (form->ending && memchr(form->ending->str, 0, form->ending->len)) {
        return 0 ;
    }

    {
        const STRING* prefix = form->specs->prefix ;
        if (prefix && memchr(prefix->str, 0, prefix->len)) {
            return 0 ;
        }
    }
    return  new_STRING(form->specs->form) ;
}
static void c_or_rt_error_or_silence(const char* format, ...)
{
    if (! checking_convfmt) {
        /* up to caller not to exceed this buffer */
        char buffer[1024] ;
        va_list args ;

        va_start(args,format) ;
        vsprintf(buffer, format, args) ;
        if (mawk_state == EXECUTION) {
            rt_error(buffer) ;
        }
        else {
            compile_error(buffer) ;
        }
    }
}

