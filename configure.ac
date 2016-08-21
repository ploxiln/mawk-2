AC_INIT(mawk, 1.9.9.6, mawkeddy@gmail.com)

dnl ==================================================================== 
dnl Checks for compilers:

AC_PROG_CC
AC_PROG_CPP

dnl ==================================================================== 
dnl Force creation of config.h 

AC_CONFIG_HEADERS([config.h:config.hin])

dnl ====================================================================
dnl Checks for selected function declarations that are sometimes missing
dnl from particular compilation environments:

AC_CHECK_DECLS([random, srandom],,,[#include <stdlib.h>])

dnl ====================================================================
dnl Checks for selected type definitions that are sometimes missing

AC_CHECK_TYPES([int64_t, __int64_t, uint64_t, __uint64_t])

dnl ====================================================================
dnl Checks for separate math library (unless the user already set LIBS):

if test -z "$LIBS"
then
    needlm=0

    AC_SEARCH_LIBS(exp, [m], needlm=1)
    AC_SEARCH_LIBS(log, [m], needlm=1)
    AC_SEARCH_LIBS(sin, [m], needlm=1)

    dnl Common elementary functions are implemented in hardware on the
    dnl Intel x86 and Motorola 680x0 families, and thus, may not
    dnl require library calls for evaluation, so pick a library
    dnl function that we know is not in hardware.  The best choice
    dnl seems to be one of the Bessel functions that all Unix systems
    dnl have supplied since the 1980s Berkeley days.

    AC_SEARCH_LIBS(j0,  [m], needlm=1)

    test $needlm -eq 1 && LIBS="-lm"
fi

dnl ====================================================================
dnl Checks for yacc alternatives:

dnl AC_CHECK_PROGS(YACC,            [bison byacc yacc ])
dnl 
dnl AC_MSG_CHECKING(for bison flags)
dnl YACC="$ac_cv_prog_YACC"
dnl if test "xx$YACC" = "xxbison"
dnl then
dnl     YFLAGS="$YFLAGS -y"
dnl     AC_MSG_RESULT(-y)
dnl else
dnl     AC_MSG_RESULT()
dnl fi

AC_PROG_YACC

dnl ====================================================================
dnl Final output:

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

