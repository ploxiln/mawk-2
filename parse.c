
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 17 "parse.y"

#include <stdio.h>
#include "mawk.h"
#include "symtype.h"
#include "code.h"
#include "memory.h"
#include "bi_funct.h"
#include "bi_vars.h"
#include "jmp.h"
#include "field.h"
#include "files.h"
#include "printf.h"


#define  YYMAXDEPTH	200


extern void   eat_nl(void) ;
static void   resize_fblock(FBLOCK *) ;
static void   switch_code_to_main(void) ;
static void   code_array(SYMTAB *) ;
static void   code_call_id(CA_REC *, SYMTAB *) ;
static void   field_A2I(void) ;
static void   check_var(SYMTAB *) ;
static void   check_array(SYMTAB *) ;
static void   RE_as_arg(void) ;
int REempty(PTR) ;

static int scope ;
static FBLOCK *active_funct ;
      /* when scope is SCOPE_FUNCT  */

#define  code_address(x)  if( is_local(x) ) \
			     code2op(L_PUSHA, (x)->offset) ;\
                          else  code2(_PUSHA, (x)->stval.cp)

#define  CDP(x)  (code_base+(x))
/* WARNING: These CDP() calculations become invalid after calls
   that might change code_base.  Which are:  code2(), code2op(),
   code_jmp() and code_pop().
*/

/* this nonsense caters to MSDOS large model */
#define  CODE_FE_PUSHA()  code_ptr->ptr = (PTR) 0 ; code1(FE_PUSHA)



/* Line 189 of yacc.c  */
#line 121 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     UNEXPECTED = 258,
     BAD_DECIMAL = 259,
     NL = 260,
     SEMI_COLON = 261,
     LBRACE = 262,
     RBRACE = 263,
     LBOX = 264,
     RBOX = 265,
     COMMA = 266,
     IO_OUT = 267,
     POW_ASG = 268,
     MOD_ASG = 269,
     DIV_ASG = 270,
     MUL_ASG = 271,
     SUB_ASG = 272,
     ADD_ASG = 273,
     ASSIGN = 274,
     COLON = 275,
     QMARK = 276,
     OR = 277,
     AND = 278,
     IN = 279,
     MATCH = 280,
     GTE = 281,
     GT = 282,
     LTE = 283,
     LT = 284,
     NEQ = 285,
     EQ = 286,
     CAT = 287,
     GETLINE = 288,
     MINUS = 289,
     PLUS = 290,
     MOD = 291,
     DIV = 292,
     MUL = 293,
     UMINUS = 294,
     NOT = 295,
     PIPE = 296,
     IO_IN = 297,
     POW = 298,
     INC_or_DEC = 299,
     FIELD = 300,
     DOLLAR = 301,
     RPAREN = 302,
     LPAREN = 303,
     DOUBLE = 304,
     STRING_ = 305,
     RE = 306,
     ID = 307,
     D_ID = 308,
     FUNCT_ID = 309,
     BUILTIN = 310,
     LENGTH = 311,
     PRINT = 312,
     PRINTF = 313,
     SPLIT = 314,
     MATCH_FUNC = 315,
     SUB = 316,
     GSUB = 317,
     SPRINTF = 318,
     DO = 319,
     WHILE = 320,
     FOR = 321,
     BREAK = 322,
     CONTINUE = 323,
     IF = 324,
     ELSE = 325,
     DELETE = 326,
     BEGIN = 327,
     END = 328,
     EXIT = 329,
     NEXT = 330,
     NEXTFILE = 331,
     RETURN = 332,
     FUNCTION = 333
   };
#endif
/* Tokens.  */
#define UNEXPECTED 258
#define BAD_DECIMAL 259
#define NL 260
#define SEMI_COLON 261
#define LBRACE 262
#define RBRACE 263
#define LBOX 264
#define RBOX 265
#define COMMA 266
#define IO_OUT 267
#define POW_ASG 268
#define MOD_ASG 269
#define DIV_ASG 270
#define MUL_ASG 271
#define SUB_ASG 272
#define ADD_ASG 273
#define ASSIGN 274
#define COLON 275
#define QMARK 276
#define OR 277
#define AND 278
#define IN 279
#define MATCH 280
#define GTE 281
#define GT 282
#define LTE 283
#define LT 284
#define NEQ 285
#define EQ 286
#define CAT 287
#define GETLINE 288
#define MINUS 289
#define PLUS 290
#define MOD 291
#define DIV 292
#define MUL 293
#define UMINUS 294
#define NOT 295
#define PIPE 296
#define IO_IN 297
#define POW 298
#define INC_or_DEC 299
#define FIELD 300
#define DOLLAR 301
#define RPAREN 302
#define LPAREN 303
#define DOUBLE 304
#define STRING_ 305
#define RE 306
#define ID 307
#define D_ID 308
#define FUNCT_ID 309
#define BUILTIN 310
#define LENGTH 311
#define PRINT 312
#define PRINTF 313
#define SPLIT 314
#define MATCH_FUNC 315
#define SUB 316
#define GSUB 317
#define SPRINTF 318
#define DO 319
#define WHILE 320
#define FOR 321
#define BREAK 322
#define CONTINUE 323
#define IF 324
#define ELSE 325
#define DELETE 326
#define BEGIN 327
#define END 328
#define EXIT 329
#define NEXT 330
#define NEXTFILE 331
#define RETURN 332
#define FUNCTION 333




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 64 "parse.y"

CELL *cp ;
SYMTAB *stp ;
int  start ; /* code starting address as offset from code_base */
PF_CP  fp ;  /* ptr to a (print/printf) or (sub/gsub) function */
BI_REC *bip ; /* ptr to info about a builtin */
FBLOCK  *fbp  ; /* ptr to a function block */
ARG2_REC *arg2p ;
CA_REC   *ca_p  ;
int   ival ;
PTR   ptr ;



/* Line 214 of yacc.c  */
#line 328 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 340 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  98
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1351

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  79
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  59
/* YYNRULES -- Number of rules.  */
#define YYNRULES  183
/* YYNRULES -- Number of states.  */
#define YYNSTATES  364

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   333

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    15,    17,    18,
      22,    23,    27,    28,    32,    33,    34,    41,    45,    49,
      51,    53,    55,    58,    60,    63,    65,    68,    71,    74,
      76,    79,    82,    84,    86,    88,    92,    96,   100,   104,
     108,   112,   116,   120,   124,   128,   132,   136,   140,   144,
     145,   150,   151,   156,   157,   158,   166,   168,   171,   173,
     175,   177,   181,   183,   187,   191,   195,   199,   203,   207,
     210,   213,   216,   218,   221,   224,   227,   230,   232,   233,
     235,   237,   241,   247,   252,   259,   265,   268,   269,   275,
     281,   288,   297,   298,   300,   302,   306,   309,   313,   317,
     318,   321,   326,   329,   331,   336,   338,   346,   351,   354,
     359,   363,   368,   370,   373,   375,   378,   382,   388,   394,
     400,   407,   415,   419,   426,   429,   431,   434,   441,   444,
     448,   450,   454,   458,   462,   466,   470,   474,   478,   481,
     487,   489,   493,   495,   499,   504,   509,   516,   518,   521,
     525,   528,   532,   534,   537,   540,   544,   549,   551,   553,
     555,   558,   562,   569,   571,   573,   575,   579,   582,   587,
     590,   593,   594,   596,   598,   602,   604,   608,   611,   614,
     616,   620,   624,   627
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      80,     0,    -1,    81,    -1,    80,    81,    -1,    82,    -1,
     129,    -1,   134,    88,    -1,    88,    -1,    -1,    72,    83,
      88,    -1,    -1,    73,    84,    88,    -1,    -1,    93,    85,
      89,    -1,    -1,    -1,    93,    11,    86,    93,    87,    89,
      -1,     7,    90,     8,    -1,     7,     1,     8,    -1,    88,
      -1,    92,    -1,    91,    -1,    90,    91,    -1,    88,    -1,
      93,    92,    -1,    92,    -1,     1,    92,    -1,    67,    92,
      -1,    68,    92,    -1,   123,    -1,    75,    92,    -1,    76,
      92,    -1,     5,    -1,     6,    -1,    98,    -1,   100,    19,
      93,    -1,   100,    18,    93,    -1,   100,    17,    93,    -1,
     100,    16,    93,    -1,   100,    15,    93,    -1,   100,    14,
      93,    -1,   100,    13,    93,    -1,    93,    31,    93,    -1,
      93,    30,    93,    -1,    93,    29,    93,    -1,    93,    28,
      93,    -1,    93,    27,    93,    -1,    93,    26,    93,    -1,
      93,    25,    93,    -1,    -1,    93,    22,    94,    93,    -1,
      -1,    93,    23,    95,    93,    -1,    -1,    -1,    93,    21,
      96,    93,    20,    97,    93,    -1,    99,    -1,    98,    99,
      -1,    49,    -1,    50,    -1,    52,    -1,    48,    93,    47,
      -1,    51,    -1,    99,    35,    99,    -1,    99,    34,    99,
      -1,    99,    38,    99,    -1,    99,    37,    99,    -1,    99,
      36,    99,    -1,    99,    43,    99,    -1,    40,    99,    -1,
      35,    99,    -1,    34,    99,    -1,   103,    -1,    52,    44,
      -1,    44,   100,    -1,   119,    44,    -1,    44,   119,    -1,
      52,    -1,    -1,   102,    -1,    93,    -1,   102,    11,    93,
      -1,    55,   105,    48,   101,    47,    -1,    63,   105,    48,
      47,    -1,    63,   105,    48,   104,   102,    47,    -1,    63,
     105,    48,   102,    47,    -1,    50,    11,    -1,    -1,    57,
     106,   108,   110,    92,    -1,    58,   106,   108,   110,    92,
      -1,    58,   106,   104,   107,   110,    92,    -1,    58,   106,
      48,   104,   107,    47,   110,    92,    -1,    -1,   102,    -1,
     101,    -1,    48,   109,    47,    -1,    48,    47,    -1,    93,
      11,    93,    -1,   109,    11,    93,    -1,    -1,    12,    93,
      -1,    69,    48,    93,    47,    -1,   111,    91,    -1,    70,
      -1,   111,    91,   112,    91,    -1,    64,    -1,   113,    91,
      65,    48,    93,    47,    92,    -1,    65,    48,    93,    47,
      -1,   114,    91,    -1,   115,   116,   117,    91,    -1,    66,
      48,     6,    -1,    66,    48,    93,     6,    -1,     6,    -1,
      93,     6,    -1,    47,    -1,    93,    47,    -1,    93,    24,
      52,    -1,    48,   109,    47,    24,    52,    -1,    52,   105,
       9,   102,    10,    -1,    52,   105,     9,   102,    10,    -1,
      52,   105,     9,   102,    10,    44,    -1,    71,    52,   105,
       9,   102,    10,    92,    -1,    71,    52,    92,    -1,    66,
      48,    52,    24,    52,    47,    -1,   118,    91,    -1,    45,
      -1,    46,    53,    -1,    46,    53,   105,     9,   102,    10,
      -1,    46,    99,    -1,    48,   119,    47,    -1,   119,    -1,
     119,    19,    93,    -1,   119,    18,    93,    -1,   119,    17,
      93,    -1,   119,    16,    93,    -1,   119,    15,    93,    -1,
     119,    14,    93,    -1,   119,    13,    93,    -1,   120,   121,
      -1,    59,    48,    93,    11,    52,    -1,    47,    -1,    11,
      93,    47,    -1,    56,    -1,    56,    48,    47,    -1,    56,
      48,    93,    47,    -1,    56,    48,    52,    47,    -1,    60,
      48,    93,    11,   122,    47,    -1,    93,    -1,    74,    92,
      -1,    74,    93,    92,    -1,    77,    92,    -1,    77,    93,
      92,    -1,   124,    -1,   124,   125,    -1,   126,    99,    -1,
      99,    41,    33,    -1,    99,    41,    33,   125,    -1,    33,
      -1,   100,    -1,   119,    -1,   124,    42,    -1,   124,   125,
      42,    -1,   127,    48,   122,    11,    93,   128,    -1,    61,
      -1,    62,    -1,    47,    -1,    11,   125,    47,    -1,   130,
      88,    -1,   131,    48,   132,    47,    -1,    78,    52,    -1,
      78,    54,    -1,    -1,   133,    -1,    52,    -1,   133,    11,
      52,    -1,     1,    -1,    54,   105,   135,    -1,    48,    47,
      -1,   136,   137,    -1,    48,    -1,   136,    93,    11,    -1,
     136,    52,    11,    -1,    93,    47,    -1,    52,    47,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   141,   141,   142,   145,   146,   147,   150,   156,   155,
     162,   161,   168,   167,   175,   191,   174,   204,   206,   212,
     213,   219,   220,   224,   225,   227,   229,   235,   238,   241,
     245,   251,   259,   259,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   277,   305,
     304,   312,   311,   318,   319,   318,   324,   325,   329,   331,
     333,   341,   345,   349,   350,   351,   352,   353,   354,   355,
     357,   359,   361,   364,   372,   379,   383,   390,   399,   400,
     403,   405,   410,   429,   433,   444,   452,   466,   469,   483,
     491,   501,   514,   519,   524,   525,   529,   533,   538,   542,
     543,   550,   555,   559,   563,   571,   576,   582,   602,   628,
     652,   653,   657,   658,   675,   679,   692,   697,   708,   721,
     733,   750,   758,   769,   783,   800,   802,   811,   825,   827,
     831,   835,   836,   837,   838,   839,   840,   841,   847,   851,
     858,   860,   895,   900,   905,   909,   957,   964,   987,   990,
     993,   996,  1001,  1008,  1014,  1019,  1024,  1031,  1033,  1033,
    1035,  1039,  1047,  1066,  1067,  1071,  1076,  1084,  1093,  1112,
    1135,  1142,  1143,  1146,  1152,  1165,  1178,  1190,  1192,  1207,
    1209,  1216,  1225,  1231
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "UNEXPECTED", "BAD_DECIMAL", "NL",
  "SEMI_COLON", "LBRACE", "RBRACE", "LBOX", "RBOX", "COMMA", "IO_OUT",
  "POW_ASG", "MOD_ASG", "DIV_ASG", "MUL_ASG", "SUB_ASG", "ADD_ASG",
  "ASSIGN", "COLON", "QMARK", "OR", "AND", "IN", "MATCH", "GTE", "GT",
  "LTE", "LT", "NEQ", "EQ", "CAT", "GETLINE", "MINUS", "PLUS", "MOD",
  "DIV", "MUL", "UMINUS", "NOT", "PIPE", "IO_IN", "POW", "INC_or_DEC",
  "FIELD", "DOLLAR", "RPAREN", "LPAREN", "DOUBLE", "STRING_", "RE", "ID",
  "D_ID", "FUNCT_ID", "BUILTIN", "LENGTH", "PRINT", "PRINTF", "SPLIT",
  "MATCH_FUNC", "SUB", "GSUB", "SPRINTF", "DO", "WHILE", "FOR", "BREAK",
  "CONTINUE", "IF", "ELSE", "DELETE", "BEGIN", "END", "EXIT", "NEXT",
  "NEXTFILE", "RETURN", "FUNCTION", "$accept", "program", "program_block",
  "PA_block", "$@1", "$@2", "$@3", "$@4", "$@5", "block",
  "block_or_separator", "statement_list", "statement", "separator", "expr",
  "$@6", "$@7", "$@8", "$@9", "cat_expr", "p_expr", "lvalue", "arglist",
  "args", "builtin", "string_comma", "mark", "pmark", "printf_args",
  "pr_args", "arg2", "pr_direction", "if_front", "else", "do",
  "while_front", "for1", "for2", "for3", "array_loop_front", "field",
  "split_front", "split_back", "re_arg", "return_statement", "getline",
  "fvalue", "getline_file", "sub_or_gsub", "sub_back", "function_def",
  "funct_start", "funct_head", "f_arglist", "f_args", "outside_error",
  "call_args", "ca_front", "ca_back", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    79,    80,    80,    81,    81,    81,    82,    83,    82,
      84,    82,    85,    82,    86,    87,    82,    88,    88,    89,
      89,    90,    90,    91,    91,    91,    91,    91,    91,    91,
      91,    91,    92,    92,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    94,
      93,    95,    93,    96,    97,    93,    98,    98,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,   100,   101,   101,
     102,   102,   103,   103,   103,   103,   104,   105,    91,    91,
      91,    91,   106,   107,   108,   108,   108,   109,   109,   110,
     110,   111,    91,   112,    91,   113,    91,   114,    91,    91,
     115,   115,   116,   116,   117,   117,    93,    93,   100,    99,
      99,    91,    91,   118,    91,   119,   119,   119,   119,   119,
      99,    93,    93,    93,    93,    93,    93,    93,    99,   120,
     121,   121,    99,    99,    99,    99,    99,   122,    91,    91,
     123,   123,    99,    99,    99,    99,    99,   124,   125,   125,
     126,   126,    99,   127,   127,   128,   128,   129,   130,   131,
     131,   132,   132,   133,   133,   134,    99,   135,   135,   136,
     136,   136,   137,   137
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     1,     0,     3,
       0,     3,     0,     3,     0,     0,     6,     3,     3,     1,
       1,     1,     2,     1,     2,     1,     2,     2,     2,     1,
       2,     2,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     0,
       4,     0,     4,     0,     0,     7,     1,     2,     1,     1,
       1,     3,     1,     3,     3,     3,     3,     3,     3,     2,
       2,     2,     1,     2,     2,     2,     2,     1,     0,     1,
       1,     3,     5,     4,     6,     5,     2,     0,     5,     5,
       6,     8,     0,     1,     1,     3,     2,     3,     3,     0,
       2,     4,     2,     1,     4,     1,     7,     4,     2,     4,
       3,     4,     1,     2,     1,     2,     3,     5,     5,     5,
       6,     7,     3,     6,     2,     1,     2,     6,     2,     3,
       1,     3,     3,     3,     3,     3,     3,     3,     2,     5,
       1,     3,     1,     3,     4,     4,     6,     1,     2,     3,
       2,     3,     1,     2,     2,     3,     4,     1,     1,     1,
       2,     3,     6,     1,     1,     1,     3,     2,     4,     2,
       2,     0,     1,     1,     3,     1,     3,     2,     2,     1,
       3,     3,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   175,     0,   157,     0,     0,     0,     0,   125,     0,
       0,    58,    59,    62,    60,    87,    87,   142,     0,     0,
     163,   164,    87,     8,    10,     0,     0,     2,     4,     7,
      12,    34,    56,     0,    72,   130,     0,   152,     0,     0,
       5,     0,     0,     0,     0,    32,    33,    92,    92,   105,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      23,     0,    21,    25,     0,     0,     0,     0,     0,     0,
      29,     0,    60,    71,   130,    70,    69,     0,    77,    74,
      76,   126,   128,     0,     0,   130,    73,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   169,   170,     1,     3,
      14,    53,    49,    51,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    57,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,     0,   140,   138,   160,
     158,   159,   153,   154,     0,   167,   171,     6,    18,    26,
      78,    78,     0,     0,    27,    28,     0,    87,   148,     0,
      30,    31,   150,     0,     0,    17,    22,    24,   102,     0,
     108,   112,     0,     0,   124,     0,     0,     0,     0,     0,
       0,    61,     0,     0,   129,     0,   179,   176,     0,    78,
     143,    60,     0,     0,     0,     0,     9,    11,     0,     0,
       0,     0,   116,    48,    47,    46,    45,    44,    43,    42,
      19,    13,    20,    64,    63,    67,    66,    65,   155,    68,
      41,    40,    39,    38,    37,    36,    35,   137,   136,   135,
     134,   133,   132,   131,     0,   161,   147,     0,   173,     0,
     172,     0,    80,    94,    79,    99,     0,    59,     0,    99,
       0,   110,    60,     0,     0,   122,     0,   149,   151,   103,
       0,     0,   113,   114,     0,     0,     0,     0,     0,    97,
      98,     0,     0,   177,    60,     0,   178,     0,   145,   144,
       0,     0,    83,     0,     0,    15,     0,    50,    52,   156,
     141,     0,   168,     0,    96,     0,     0,     0,     0,     0,
      86,    93,    99,     0,   107,     0,   111,   101,     0,   104,
       0,   115,   109,     0,     0,     0,   117,   119,   181,   183,
     180,   182,    82,   139,     0,    85,     0,     0,    54,     0,
     174,    95,    81,   100,    88,     0,     0,    89,     0,     0,
       0,   119,   118,   127,   120,   146,    84,    16,     0,     0,
     165,   162,    99,    90,   123,     0,     0,    55,     0,     0,
     121,   106,   166,    91
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    26,    27,    28,    94,    95,   112,   198,   327,    60,
     211,    61,    62,    63,   242,   200,   201,   199,   348,    31,
      32,    33,   243,   244,    34,   248,    87,   150,   302,   245,
     295,   298,    65,   260,    66,    67,    68,   173,   265,    69,
      35,    36,   138,   237,    70,    37,   142,    38,    39,   351,
      40,    41,    42,   239,   240,    43,   187,   188,   276
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -241
static const yytype_int16 yypact[] =
{
     364,  -241,   544,  -241,   916,   916,   916,   139,  -241,   730,
     947,  -241,  -241,  -241,   476,  -241,  -241,   -43,   -23,    -3,
    -241,  -241,  -241,  -241,  -241,   -24,   301,  -241,  -241,  -241,
    1255,   916,   140,   465,  -241,   547,     7,   120,   916,    24,
    -241,    44,    41,    44,    15,  -241,  -241,  -241,  -241,  -241,
      45,    55,   112,   112,    95,    98,   395,   112,   112,   395,
    -241,   467,  -241,  -241,   618,   621,   621,   621,   668,   621,
    -241,   947,   110,    59,   103,    59,    59,   100,    46,  -241,
    -241,    46,  -241,    11,    13,   539,  -241,   146,   122,   131,
     761,   947,   947,   138,    44,    44,  -241,  -241,  -241,  -241,
    -241,  -241,  -241,  -241,   145,   947,   947,   947,   947,   947,
     947,   947,    70,   140,   916,   916,   916,   916,   916,   172,
     916,   947,   947,   947,   947,   947,   947,   947,   947,   947,
     947,   947,   947,   947,   947,  -241,   947,  -241,  -241,  -241,
    -241,  -241,   169,   126,   947,  -241,   161,  -241,  -241,  -241,
     978,  1009,   947,   699,  -241,  -241,   947,   112,  -241,   618,
    -241,  -241,  -241,   618,   112,  -241,  -241,  -241,   157,   164,
    -241,  -241,  1208,   792,  -241,  1083,   222,   192,   233,   234,
     947,  -241,   947,   220,  -241,   947,   198,  -241,  1040,   947,
    -241,    69,  1094,  1276,  1297,   823,  -241,  -241,   947,   947,
     947,   947,  -241,   190,  -241,  -241,  -241,  -241,  -241,  -241,
    -241,  -241,  -241,    58,    58,    59,    59,    59,   139,   209,
    1320,  1320,  1320,  1320,  1320,  1320,  1320,  1320,  1320,  1320,
    1320,  1320,  1320,  1320,  1121,  -241,  1320,   243,  -241,   211,
     245,   854,  1320,  -241,   248,   266,   885,   268,   947,   266,
    1132,  -241,   367,  1234,  1159,  -241,   271,  -241,  -241,  -241,
     621,   235,  -241,  -241,  1170,   621,   947,   947,   947,  1320,
    1320,   230,     2,  -241,   219,   246,  -241,   238,  -241,  -241,
     236,   947,  -241,    16,   947,  1320,  1309,   436,   543,  -241,
    -241,   947,  -241,   240,  -241,    32,   947,   947,   112,   947,
    -241,   248,   266,   112,  -241,   242,  -241,  -241,   947,  -241,
     947,  -241,  -241,   179,   182,   185,  -241,   616,  -241,  -241,
    -241,  -241,  -241,  -241,   239,  -241,    93,    70,  -241,   295,
    -241,   220,  1320,  1320,  -241,   251,   112,  -241,   252,   215,
    1197,   256,  -241,  -241,  -241,  -241,  -241,  -241,   947,   139,
    -241,  -241,   266,  -241,  -241,   112,   112,  1320,   257,   112,
    -241,  -241,  -241,  -241
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -241,  -241,   261,  -241,  -241,  -241,  -241,  -241,  -241,     3,
     -37,  -241,   -50,   150,     0,  -241,  -241,  -241,  -241,  -241,
     133,    -6,   106,   -44,  -241,  -189,    -8,   255,     6,   160,
     302,  -240,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,
      43,  -241,  -241,    34,  -241,  -241,  -214,  -241,  -241,  -241,
    -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241,  -241
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -119
static const yytype_int16 yytable[] =
{
      30,    79,    64,    29,   289,    90,   284,    88,    89,   303,
      83,   166,   317,   296,    93,   168,   169,   170,   136,   174,
      45,    46,   180,   148,   182,    91,    30,   296,    96,    29,
      97,   140,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   182,   145,    92,   147,    74,    74,    74,
      80,     2,    74,    85,   137,   -87,   159,   299,   181,   163,
     183,    64,   336,   325,   176,    64,    64,    64,   172,    64,
     178,   175,   144,   179,    74,    45,    46,     2,   -87,   331,
     141,    74,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   146,
     192,   193,   194,   152,   116,   117,   118,   196,   197,   119,
     119,   120,   120,   153,   296,   203,   204,   205,   206,   207,
     208,   209,   359,    86,    85,   210,   278,    45,    46,   -87,
     177,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   358,   234,    73,    75,    76,
     346,   272,    82,   156,   236,     8,     9,   135,    77,   256,
     157,   283,   250,   253,    86,   185,   254,    74,    74,    74,
      74,    74,   139,    74,   113,     8,     9,  -119,    77,   120,
     186,   143,    78,   264,   114,   115,   116,   117,   118,   189,
     269,   119,   270,   120,     8,     9,   195,    77,   275,   341,
     296,    78,   342,   296,   149,   343,   296,   202,   285,   286,
     287,   288,   154,   155,   301,   218,   158,   160,   161,   162,
     309,   235,   140,   238,   167,   312,   106,   107,   108,   109,
     110,   111,   313,   314,   315,   355,   296,   259,   -87,   261,
     318,   266,   -77,   -77,   -77,   -77,   -77,   -77,   -77,   184,
     326,    83,   267,   268,   271,   273,    83,   213,   214,   215,
     216,   217,   120,   219,   291,   301,   293,   320,   292,   296,
      64,   141,   212,    86,   339,    64,   319,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   297,   300,
     308,   236,   316,   310,    85,   322,   345,    99,   323,    85,
     347,   329,   330,   321,   338,   277,   332,   333,   352,   354,
     344,    98,     1,   151,   362,   335,   349,   255,     2,   257,
     340,   249,    84,   258,   149,   324,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,     0,     0,     0,
     210,     0,     0,     0,     3,     4,     5,     0,     0,     0,
       0,     6,   350,   140,     0,     7,     8,     9,   357,    10,
      11,    12,    13,    14,     0,    15,    16,    17,     0,     0,
      18,    19,    20,    21,    22,     1,     0,     0,     0,     0,
       0,     2,     0,    23,    24,     0,   -87,     0,     0,    25,
     -77,   -77,   -77,   -77,   -77,   -77,   -77,     0,     0,     0,
       0,   305,   141,     0,     0,     0,     0,     3,     4,     5,
      45,    46,     0,     0,     6,     0,     0,     0,     7,     8,
       9,    86,    10,    11,    12,    13,    14,     0,    15,    16,
      17,     0,     0,    18,    19,    20,    21,    22,     3,     4,
       5,     0,     0,     0,     0,     6,    23,    24,     0,     7,
       8,     9,    25,    10,    11,    12,    13,    14,   334,    15,
      16,    17,     0,   337,    18,    19,    20,    21,    22,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   164,     0,
       0,     0,    45,    46,     2,   165,     0,   212,   121,   122,
     123,   124,   125,   126,   127,   -87,   353,     0,     0,   -77,
     -77,   -77,   -77,   -77,   -77,   -77,     0,     0,     0,     0,
       3,     4,     5,     0,     0,   360,   361,     6,     0,   363,
       0,     7,     8,     9,     0,    10,    11,    12,    13,    14,
      86,    15,    16,    17,    47,    48,    18,    19,    20,    21,
      22,    49,    50,    51,    52,    53,    54,     0,    55,     0,
       0,    56,    57,    58,    59,    44,     0,     0,     0,    45,
      46,     2,   128,   129,   130,   131,   132,   133,   134,     0,
     128,   129,   130,   131,   132,   133,   134,   104,   105,   106,
     107,   108,   109,   110,   111,     0,     0,     3,     4,     5,
       0,     0,     0,   135,     6,     0,   184,     0,     7,     8,
       9,   135,    10,    11,    12,    13,    14,     0,    15,    16,
      17,    47,    48,    18,    19,    20,    21,    22,    49,    50,
      51,    52,    53,    54,     0,    55,     0,     0,    56,    57,
      58,    59,   164,    45,    46,     0,    45,    46,     2,  -118,
    -118,  -118,  -118,  -118,  -118,  -118,     0,     0,     0,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
       0,     0,     0,     0,     3,     4,     5,     0,     0,     0,
     344,     6,     0,     0,     0,     7,     8,     9,     0,    10,
      11,    12,    13,    14,   171,    15,    16,    17,    47,    48,
      18,    19,    20,    21,    22,    49,    50,    51,    52,    53,
      54,     0,    55,     0,     0,    56,    57,    58,    59,     0,
       0,     3,     4,     5,     0,   251,     0,     0,     6,     0,
       0,     0,     7,     8,     9,     0,    10,    11,    12,    13,
      14,     0,    15,    16,    17,     0,     0,    18,    19,    20,
      21,    22,     3,     4,     5,     0,     0,     0,     0,     6,
       0,     0,     0,     7,     8,     9,     0,    10,    11,    12,
      13,   252,     0,    15,    16,    17,     0,     0,    18,    19,
      20,    21,    22,     3,     4,     5,     0,     0,     0,     0,
       6,     0,     0,     0,     7,     8,     9,     0,    71,    11,
      12,    13,    72,    81,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,     3,     4,     5,     0,     0,     0,
       0,     6,     0,     0,     0,     7,     8,     9,   190,    10,
      11,    12,    13,   191,     0,    15,    16,    17,     0,     0,
      18,    19,    20,    21,    22,     3,     4,     5,     0,     0,
       0,     0,     6,     0,     0,     0,     7,     8,     9,   263,
      10,    11,    12,    13,    14,     0,    15,    16,    17,     0,
       0,    18,    19,    20,    21,    22,     3,     4,     5,     0,
       0,     0,     0,     6,     0,     0,     0,     7,     8,     9,
     282,    10,    11,   247,    13,    14,     0,    15,    16,    17,
       0,     0,    18,    19,    20,    21,    22,     3,     4,     5,
       0,     0,     0,     0,     6,     0,     0,     0,     7,     8,
       9,   294,    10,    11,    12,    13,    14,     0,    15,    16,
      17,     0,     0,    18,    19,    20,    21,    22,     3,     4,
       5,     0,     0,     0,     0,     6,     0,     0,     0,     7,
       8,     9,   294,    10,    11,   247,    13,    14,     0,    15,
      16,    17,     0,     0,    18,    19,    20,    21,    22,     3,
       4,     5,     0,     0,     0,     0,     6,     0,     0,     0,
       7,     8,     9,     0,    71,    11,    12,    13,    72,     0,
      15,    16,    17,     0,     0,    18,    19,    20,    21,    22,
       3,     4,     5,     0,     0,     0,     0,     6,     0,     0,
       0,     7,     8,     9,     0,    10,    11,    12,    13,    14,
       0,    15,    16,    17,     0,     0,    18,    19,    20,    21,
      22,     3,     4,     5,     0,     0,     0,     0,     6,     0,
       0,     0,     7,     8,     9,     0,   241,    11,    12,    13,
      14,     0,    15,    16,    17,     0,     0,    18,    19,    20,
      21,    22,     3,     4,     5,     0,     0,     0,     0,     6,
       0,     0,     0,     7,     8,     9,     0,   246,    11,   247,
      13,    14,     0,    15,    16,    17,     0,     0,    18,    19,
      20,    21,    22,     3,     4,     5,     0,     0,     0,     0,
       6,     0,     0,     0,     7,     8,     9,     0,    10,    11,
      12,    13,   274,     0,    15,    16,    17,     0,     0,    18,
      19,    20,    21,    22,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,     0,     0,     0,     0,
     181,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   279,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,     0,     0,     0,     0,   290,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   304,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,     0,     0,     0,     0,   307,     0,     0,     0,
       0,     0,     0,     0,   262,     0,     0,   311,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     306,     0,     0,     0,   356,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   100,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   280,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   281,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   328,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111
};

static const yytype_int16 yycheck[] =
{
       0,     7,     2,     0,   218,    48,   195,    15,    16,   249,
      10,    61,    10,    11,    22,    65,    66,    67,    11,    69,
       5,     6,    11,     8,    11,    48,    26,    11,    52,    26,
      54,    37,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    11,    41,    48,    43,     4,     5,     6,
       7,     7,     9,    10,    47,     9,    56,   246,    47,    59,
      47,    61,   302,    47,    72,    65,    66,    67,    68,    69,
      78,    71,    48,    81,    31,     5,     6,     7,     9,    47,
      37,    38,    13,    14,    15,    16,    17,    18,    19,    48,
      90,    91,    92,    48,    36,    37,    38,    94,    95,    41,
      41,    43,    43,    48,    11,   105,   106,   107,   108,   109,
     110,   111,   352,    44,    71,   112,    47,     5,     6,     9,
      77,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   349,   136,     4,     5,     6,
      47,   185,     9,    48,   144,    45,    46,    44,    48,   157,
      52,   195,   152,   153,    44,     9,   156,   114,   115,   116,
     117,   118,    42,   120,    31,    45,    46,    41,    48,    43,
      48,    38,    52,   173,    34,    35,    36,    37,    38,    48,
     180,    41,   182,    43,    45,    46,    48,    48,   188,    10,
      11,    52,    10,    11,    44,    10,    11,    52,   198,   199,
     200,   201,    52,    53,   248,    33,    56,    57,    58,    59,
     260,    42,   218,    52,    64,   265,    26,    27,    28,    29,
      30,    31,   266,   267,   268,    10,    11,    70,     9,    65,
      11,     9,    13,    14,    15,    16,    17,    18,    19,    47,
     284,   241,     9,     9,    24,    47,   246,   114,   115,   116,
     117,   118,    43,   120,    11,   299,    11,    11,    47,    11,
     260,   218,   112,    44,   308,   265,    47,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    12,    11,
       9,   281,    52,    48,   241,    47,    47,    26,    52,   246,
     327,   291,    52,    47,    52,   189,   296,   297,    47,    47,
      44,     0,     1,    48,    47,   299,    11,   157,     7,   159,
     310,   151,    10,   163,   164,   281,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    -1,    -1,    -1,
     327,    -1,    -1,    -1,    33,    34,    35,    -1,    -1,    -1,
      -1,    40,    47,   349,    -1,    44,    45,    46,   348,    48,
      49,    50,    51,    52,    -1,    54,    55,    56,    -1,    -1,
      59,    60,    61,    62,    63,     1,    -1,    -1,    -1,    -1,
      -1,     7,    -1,    72,    73,    -1,     9,    -1,    -1,    78,
      13,    14,    15,    16,    17,    18,    19,    -1,    -1,    -1,
      -1,    24,   349,    -1,    -1,    -1,    -1,    33,    34,    35,
       5,     6,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,
      46,    44,    48,    49,    50,    51,    52,    -1,    54,    55,
      56,    -1,    -1,    59,    60,    61,    62,    63,    33,    34,
      35,    -1,    -1,    -1,    -1,    40,    72,    73,    -1,    44,
      45,    46,    78,    48,    49,    50,    51,    52,   298,    54,
      55,    56,    -1,   303,    59,    60,    61,    62,    63,    23,
      24,    25,    26,    27,    28,    29,    30,    31,     1,    -1,
      -1,    -1,     5,     6,     7,     8,    -1,   327,    13,    14,
      15,    16,    17,    18,    19,     9,   336,    -1,    -1,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    -1,
      33,    34,    35,    -1,    -1,   355,   356,    40,    -1,   359,
      -1,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      44,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    -1,    71,    -1,
      -1,    74,    75,    76,    77,     1,    -1,    -1,    -1,     5,
       6,     7,    13,    14,    15,    16,    17,    18,    19,    -1,
      13,    14,    15,    16,    17,    18,    19,    24,    25,    26,
      27,    28,    29,    30,    31,    -1,    -1,    33,    34,    35,
      -1,    -1,    -1,    44,    40,    -1,    47,    -1,    44,    45,
      46,    44,    48,    49,    50,    51,    52,    -1,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    -1,    71,    -1,    -1,    74,    75,
      76,    77,     1,     5,     6,    -1,     5,     6,     7,    13,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      -1,    -1,    -1,    -1,    33,    34,    35,    -1,    -1,    -1,
      44,    40,    -1,    -1,    -1,    44,    45,    46,    -1,    48,
      49,    50,    51,    52,     6,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    -1,    71,    -1,    -1,    74,    75,    76,    77,    -1,
      -1,    33,    34,    35,    -1,     6,    -1,    -1,    40,    -1,
      -1,    -1,    44,    45,    46,    -1,    48,    49,    50,    51,
      52,    -1,    54,    55,    56,    -1,    -1,    59,    60,    61,
      62,    63,    33,    34,    35,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,    50,
      51,    52,    -1,    54,    55,    56,    -1,    -1,    59,    60,
      61,    62,    63,    33,    34,    35,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    -1,    -1,    59,
      60,    61,    62,    63,    33,    34,    35,    -1,    -1,    -1,
      -1,    40,    -1,    -1,    -1,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    -1,    54,    55,    56,    -1,    -1,
      59,    60,    61,    62,    63,    33,    34,    35,    -1,    -1,
      -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    -1,    54,    55,    56,    -1,
      -1,    59,    60,    61,    62,    63,    33,    34,    35,    -1,
      -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    -1,    54,    55,    56,
      -1,    -1,    59,    60,    61,    62,    63,    33,    34,    35,
      -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    -1,    54,    55,
      56,    -1,    -1,    59,    60,    61,    62,    63,    33,    34,
      35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    -1,    54,
      55,    56,    -1,    -1,    59,    60,    61,    62,    63,    33,
      34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,    -1,
      44,    45,    46,    -1,    48,    49,    50,    51,    52,    -1,
      54,    55,    56,    -1,    -1,    59,    60,    61,    62,    63,
      33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,    -1,
      -1,    44,    45,    46,    -1,    48,    49,    50,    51,    52,
      -1,    54,    55,    56,    -1,    -1,    59,    60,    61,    62,
      63,    33,    34,    35,    -1,    -1,    -1,    -1,    40,    -1,
      -1,    -1,    44,    45,    46,    -1,    48,    49,    50,    51,
      52,    -1,    54,    55,    56,    -1,    -1,    59,    60,    61,
      62,    63,    33,    34,    35,    -1,    -1,    -1,    -1,    40,
      -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,    50,
      51,    52,    -1,    54,    55,    56,    -1,    -1,    59,    60,
      61,    62,    63,    33,    34,    35,    -1,    -1,    -1,    -1,
      40,    -1,    -1,    -1,    44,    45,    46,    -1,    48,    49,
      50,    51,    52,    -1,    54,    55,    56,    -1,    -1,    59,
      60,    61,    62,    63,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
      47,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    47,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    -1,    47,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    47,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    -1,    -1,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     6,    -1,    -1,    47,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
       6,    -1,    -1,    -1,    47,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    11,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    11,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    11,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     7,    33,    34,    35,    40,    44,    45,    46,
      48,    49,    50,    51,    52,    54,    55,    56,    59,    60,
      61,    62,    63,    72,    73,    78,    80,    81,    82,    88,
      93,    98,    99,   100,   103,   119,   120,   124,   126,   127,
     129,   130,   131,   134,     1,     5,     6,    57,    58,    64,
      65,    66,    67,    68,    69,    71,    74,    75,    76,    77,
      88,    90,    91,    92,    93,   111,   113,   114,   115,   118,
     123,    48,    52,    99,   119,    99,    99,    48,    52,   100,
     119,    53,    99,    93,   109,   119,    44,   105,   105,   105,
      48,    48,    48,   105,    83,    84,    52,    54,     0,    81,
      11,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    85,    99,    34,    35,    36,    37,    38,    41,
      43,    13,    14,    15,    16,    17,    18,    19,    13,    14,
      15,    16,    17,    18,    19,    44,    11,    47,   121,    42,
     100,   119,   125,    99,    48,    88,    48,    88,     8,    92,
     106,   106,    48,    48,    92,    92,    48,    52,    92,    93,
      92,    92,    92,    93,     1,     8,    91,    92,    91,    91,
      91,     6,    93,   116,    91,    93,   105,   119,   105,   105,
      11,    47,    11,    47,    47,     9,    48,   135,   136,    48,
      47,    52,    93,    93,    93,    48,    88,    88,    86,    96,
      94,    95,    52,    93,    93,    93,    93,    93,    93,    93,
      88,    89,    92,    99,    99,    99,    99,    99,    33,    99,
      93,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93,    93,    93,    93,    42,    93,   122,    52,   132,
     133,    48,    93,   101,   102,   108,    48,    50,   104,   108,
      93,     6,    52,    93,    93,    92,   105,    92,    92,    70,
     112,    65,     6,    47,    93,   117,     9,     9,     9,    93,
      93,    24,   102,    47,    52,    93,   137,   101,    47,    47,
      11,    11,    47,   102,   104,    93,    93,    93,    93,   125,
      47,    11,    47,    11,    47,   109,    11,    12,   110,   104,
      11,   102,   107,   110,    47,    24,     6,    47,     9,    91,
      48,    47,    91,   102,   102,   102,    52,    10,    11,    47,
      11,    47,    47,    52,   122,    47,   102,    87,    20,    93,
      52,    47,    93,    93,    92,   107,   110,    92,    52,   102,
      93,    10,    10,    10,    44,    47,    47,    89,    97,    11,
      47,   128,    47,    92,    47,    10,    47,    93,   125,   110,
      92,    92,    47,    92
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 7:

/* Line 1455 of yacc.c  */
#line 151 "parse.y"
    { /* this do nothing action removes a vacuous warning
                  from Bison */
             }
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 156 "parse.y"
    { be_setup(scope = SCOPE_BEGIN) ; }
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 159 "parse.y"
    { switch_code_to_main() ; }
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 162 "parse.y"
    { be_setup(scope = SCOPE_END) ; }
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 165 "parse.y"
    { switch_code_to_main() ; }
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 168 "parse.y"
    { code_jmp(_JZ, (INST*)0) ; }
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 171 "parse.y"
    { patch_jmp( code_ptr ) ; }
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 175 "parse.y"
    {
	       INST *p1 = CDP((yyvsp[(1) - (2)].start)) ;
             int len ;

	       code_push(p1, code_ptr - p1, scope, active_funct) ;
               code_ptr = p1 ;

               code2op(_RANGE, 1) ;
               code_ptr += 3 ;
               len = code_pop(code_ptr) ;
             code_ptr += len ;
               code1(_STOP) ;
             p1 = CDP((yyvsp[(1) - (2)].start)) ;
               p1[2].op = code_ptr - (p1+1) ;
             }
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 191 "parse.y"
    { code1(_STOP) ; }
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 194 "parse.y"
    {
	       INST *p1 = CDP((yyvsp[(1) - (6)].start)) ;

	       p1[3].op = CDP((yyvsp[(6) - (6)].start)) - (p1+1) ;
               p1[4].op = code_ptr - (p1+1) ;
             }
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 205 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 207 "parse.y"
    { (yyval.start) = code_offset ; /* does nothing won't be executed */
              print_flag = getline_flag = paren_cnt = 0 ;
              yyerrok ; }
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 214 "parse.y"
    { (yyval.start) = code_offset ;
                       code1(_PUSHINT) ; code1(0) ;
                       code2(_PRINT, bi_print) ;
                     }
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 226 "parse.y"
    { code1(_POP) ; }
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 228 "parse.y"
    { (yyval.start) = code_offset ; }
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 230 "parse.y"
    { (yyval.start) = code_offset ;
                print_flag = getline_flag = 0 ;
                paren_cnt = 0 ;
                yyerrok ;
              }
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 236 "parse.y"
    { (yyval.start) = code_offset ; BC_insert('B', code_ptr+1) ;
               code2(_JMP, 0) /* don't use code_jmp ! */ ; }
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 239 "parse.y"
    { (yyval.start) = code_offset ; BC_insert('C', code_ptr+1) ;
               code2(_JMP, 0) ; }
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 242 "parse.y"
    { if ( scope != SCOPE_FUNCT )
                     compile_error("return outside function body") ;
             }
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 246 "parse.y"
    { if ( scope != SCOPE_MAIN )
                   compile_error( "improper use of next" ) ;
                (yyval.start) = code_offset ;
                code1(_NEXT) ;
              }
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 252 "parse.y"
    { if ( scope != SCOPE_MAIN )
                   compile_error( "improper use of nextfile" ) ;
                (yyval.start) = code_offset ;
                code1(_NEXTFILE) ;
              }
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 263 "parse.y"
    { code1(_ASSIGN) ; }
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 264 "parse.y"
    { code1(_ADD_ASG) ; }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 265 "parse.y"
    { code1(_SUB_ASG) ; }
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 266 "parse.y"
    { code1(_MUL_ASG) ; }
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 267 "parse.y"
    { code1(_DIV_ASG) ; }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 268 "parse.y"
    { code1(_MOD_ASG) ; }
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 269 "parse.y"
    { code1(_POW_ASG) ; }
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 270 "parse.y"
    { code1(_EQ) ; }
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 271 "parse.y"
    { code1(_NEQ) ; }
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 272 "parse.y"
    { code1(_LT) ; }
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 273 "parse.y"
    { code1(_LTE) ; }
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 274 "parse.y"
    { code1(_GT) ; }
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 275 "parse.y"
    { code1(_GTE) ; }
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 278 "parse.y"
    {
	    INST *p3 = CDP((yyvsp[(3) - (3)].start)) ;

            if ( p3 == code_ptr - 2 )
            {
               if ( p3->op == _MATCH0 )  p3->op = _MATCH1 ;

               else /* check for string */
               if ( p3->op == _PUSHS )
               { CELL *cp = ZMALLOC(CELL) ;

                 cp->type = C_STRING ;
                 cp->ptr = p3[1].ptr ;
                 cast_to_RE(cp) ;
                 code_ptr -= 2 ;
                 code2(_MATCH1, cp->ptr) ;
                 ZFREE(cp) ;
               }
               else  code1(_MATCH2) ;
            }
            else code1(_MATCH2) ;

            if ( !(yyvsp[(2) - (3)].ival) ) code1(_NOT) ;
          }
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 305 "parse.y"
    { code1(_TEST) ;
                code_jmp(_LJNZ, (INST*)0) ;
              }
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 309 "parse.y"
    { code1(_TEST) ; patch_jmp(code_ptr) ; }
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 312 "parse.y"
    { code1(_TEST) ;
		code_jmp(_LJZ, (INST*)0) ;
	      }
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 316 "parse.y"
    { code1(_TEST) ; patch_jmp(code_ptr) ; }
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 318 "parse.y"
    { code_jmp(_JZ, (INST*)0) ; }
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 319 "parse.y"
    { code_jmp(_JMP, (INST*)0) ; }
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 321 "parse.y"
    { patch_jmp(code_ptr) ; patch_jmp(CDP((yyvsp[(7) - (7)].start))) ; }
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 326 "parse.y"
    { code1(_CAT) ; }
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 330 "parse.y"
    {  (yyval.start) = code_offset ; code2(_PUSHD, (yyvsp[(1) - (1)].ptr)) ; }
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 332 "parse.y"
    { (yyval.start) = code_offset ; code2(_PUSHS, (yyvsp[(1) - (1)].ptr)) ; }
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 334 "parse.y"
    { check_var((yyvsp[(1) - (1)].stp)) ;
            (yyval.start) = code_offset ;
            if ( is_local((yyvsp[(1) - (1)].stp)) )
            { code2op(L_PUSHI, (yyvsp[(1) - (1)].stp)->offset) ; }
            else code2(_PUSHI, (yyvsp[(1) - (1)].stp)->stval.cp) ;
          }
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 342 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 346 "parse.y"
    { (yyval.start) = code_offset ; code2(_MATCH0, (yyvsp[(1) - (1)].ptr)) ; }
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 349 "parse.y"
    { code1(_ADD) ; }
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 350 "parse.y"
    { code1(_SUB) ; }
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 351 "parse.y"
    { code1(_MUL) ; }
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 352 "parse.y"
    { code1(_DIV) ; }
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 353 "parse.y"
    { code1(_MOD) ; }
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 354 "parse.y"
    { code1(_POW) ; }
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 356 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ; code1(_NOT) ; }
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 358 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ; code1(_UPLUS) ; }
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 360 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ; code1(_UMINUS) ; }
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 365 "parse.y"
    { check_var((yyvsp[(1) - (2)].stp)) ;
             (yyval.start) = code_offset ;
             code_address((yyvsp[(1) - (2)].stp)) ;

             if ( (yyvsp[(2) - (2)].ival) == '+' )  code1(_POST_INC) ;
             else  code1(_POST_DEC) ;
           }
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 373 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ;
              if ( (yyvsp[(1) - (2)].ival) == '+' ) code1(_PRE_INC) ;
              else  code1(_PRE_DEC) ;
            }
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 380 "parse.y"
    { if ((yyvsp[(2) - (2)].ival) == '+' ) code1(F_POST_INC ) ;
             else  code1(F_POST_DEC) ;
           }
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 384 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ;
             if ( (yyvsp[(1) - (2)].ival) == '+' ) code1(F_PRE_INC) ;
             else  code1( F_PRE_DEC) ;
           }
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 391 "parse.y"
    { (yyval.start) = code_offset ;
          check_var((yyvsp[(1) - (1)].stp)) ;
          code_address((yyvsp[(1) - (1)].stp)) ;
        }
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 399 "parse.y"
    { (yyval.ival) = 0 ; }
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 404 "parse.y"
    { (yyval.ival) = 1 ; }
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 406 "parse.y"
    { (yyval.ival) = (yyvsp[(1) - (3)].ival) + 1 ; }
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 411 "parse.y"
    { BI_REC *p = (yyvsp[(1) - (5)].bip) ;
          (yyval.start) = (yyvsp[(2) - (5)].start) ;
          if ( (int)p->min_args > (yyvsp[(4) - (5)].ival) || (int)p->max_args < (yyvsp[(4) - (5)].ival) )
            compile_error(
            "wrong number of arguments in call to %s" ,
            p->name ) ;
          if ( p->min_args != p->max_args ) /* variable args */
              { code1(_PUSHINT) ;  code1((yyvsp[(4) - (5)].ival)) ; }
          code2(_BUILTIN , p->fp) ;
        }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 430 "parse.y"
    {   (yyval.start) = (yyvsp[(2) - (4)].start) ;
	      compile_error("no argments in call to sprintf()") ;
	  }
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 434 "parse.y"
    {   /* the usual case */
	      const Form* form = (Form*) (yyvsp[(4) - (6)].ptr) ;
	      (yyval.start) = (yyvsp[(2) - (6)].start) ;
	      if (form && form->num_args != (yyvsp[(5) - (6)].ival)) {
   compile_error("wrong number of arguments to sprintf, needs %d, has %d",
                    form->num_args+1, (yyvsp[(5) - (6)].ival)+1) ;
              }
	      code2op(_PUSHINT, (yyvsp[(5) - (6)].ival) + 1) ;
	      code2(_BUILTIN, bi_sprintf) ;
	  }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 445 "parse.y"
    {
	      (yyval.start) = (yyvsp[(2) - (5)].start) ;
	      code2op(_PUSHINT, (yyvsp[(4) - (5)].ival)) ;
	      code2(_BUILTIN, bi_sprintf1) ;
	  }
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 453 "parse.y"
    {
	           STRING* str = (STRING*) (yyvsp[(1) - (2)].ptr) ;
		   const Form* form = parse_form(str) ;
		   free_STRING(str) ;
		   (yyval.ptr) = (PTR) form ;
		   code2(PUSHFM, form) ;
		}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 466 "parse.y"
    { (yyval.start) = code_offset ; }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 470 "parse.y"
    { code2(_PRINT, bi_print) ;
              print_flag = 0 ;
              (yyval.start) = (yyvsp[(2) - (5)].start) ;
            }
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 484 "parse.y"
    { code2(_PRINT, bi_printf1) ;
              print_flag = 0 ;
              (yyval.start) = (yyvsp[(2) - (5)].start) ;
	      if ((yyvsp[(3) - (5)].ival) == 0) {
	          compile_error("no arguments in call to printf") ;
	      }
	    }
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 492 "parse.y"
    {  const Form* form = (Form*) (yyvsp[(3) - (6)].ptr) ;
	        if (form && form->num_args != (yyvsp[(4) - (6)].ival)) {
   compile_error("wrong number of arguments to printf, needs %d, has %d",
                    form->num_args+1, (yyvsp[(4) - (6)].ival)+1) ;
		}
	        code2(_PRINT, bi_printf) ;
	        print_flag = 0 ;
		(yyval.start) = (yyvsp[(2) - (6)].start) ;
	     }
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 502 "parse.y"
    {  const Form* form = (Form*) (yyvsp[(4) - (8)].ptr) ;
	        if (form && form->num_args != (yyvsp[(5) - (8)].ival)) {
   compile_error("wrong number of arguments to printf, needs %d, has %d",
                    form->num_args+1, (yyvsp[(5) - (8)].ival)+1) ;
		}
	        code2(_PRINT, bi_printf) ;
	        print_flag = 0 ;
		(yyval.start) = (yyvsp[(2) - (8)].start) ;
	     }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 514 "parse.y"
    { (yyval.start) = code_offset ;
	     print_flag = 1 ;
	   }
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 520 "parse.y"
    { code2op(_PUSHINT, (yyvsp[(1) - (1)].ival) + 1) ;
	   }
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 524 "parse.y"
    { code2op(_PUSHINT, (yyvsp[(1) - (1)].ival)) ; }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 526 "parse.y"
    { (yyval.ival) = (yyvsp[(2) - (3)].arg2p)->cnt ; zfree((yyvsp[(2) - (3)].arg2p),sizeof(ARG2_REC)) ;
             code2op(_PUSHINT, (yyval.ival)) ;
           }
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 530 "parse.y"
    { (yyval.ival)=0 ; code2op(_PUSHINT, 0) ; }
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 534 "parse.y"
    { (yyval.arg2p) = (ARG2_REC*) zmalloc(sizeof(ARG2_REC)) ;
             (yyval.arg2p)->start = (yyvsp[(1) - (3)].start) ;
             (yyval.arg2p)->cnt = 2 ;
           }
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 539 "parse.y"
    { (yyval.arg2p) = (yyvsp[(1) - (3)].arg2p) ; (yyval.arg2p)->cnt++ ; }
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 544 "parse.y"
    { code2op(_PUSHINT, (yyvsp[(1) - (2)].ival)) ; }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 551 "parse.y"
    {  (yyval.start) = (yyvsp[(3) - (4)].start) ; eat_nl() ; code_jmp(_JZ, (INST*)0) ; }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 556 "parse.y"
    { patch_jmp( code_ptr ) ;  }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 559 "parse.y"
    { eat_nl() ; code_jmp(_JMP, (INST*)0) ; }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 564 "parse.y"
    { patch_jmp(code_ptr) ;
		  patch_jmp(CDP((yyvsp[(4) - (4)].start))) ;
		}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 572 "parse.y"
    { eat_nl() ; BC_new() ; }
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 577 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (7)].start) ;
          code_jmp(_JNZ, CDP((yyvsp[(2) - (7)].start))) ;
          BC_clear(code_ptr, CDP((yyvsp[(5) - (7)].start))) ; }
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 583 "parse.y"
    { eat_nl() ; BC_new() ;
                  (yyval.start) = (yyvsp[(3) - (4)].start) ;

                  /* check if const expression */
                  if ( code_ptr - 2 == CDP((yyvsp[(3) - (4)].start)) &&
                       code_ptr[-2].op == _PUSHD &&
                       *(double*)code_ptr[-1].ptr != 0.0
                     )
                     code_ptr -= 2 ;
                  else
		  { INST *p3 = CDP((yyvsp[(3) - (4)].start)) ;
		    code_push(p3, code_ptr-p3, scope, active_funct) ;
		    code_ptr = p3 ;
                    code2(_JMP, (INST*)0) ; /* code2() not code_jmp() */
		  }
                }
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 603 "parse.y"
    {
		  int  saved_offset ;
		  int len ;
		  INST *p1 = CDP((yyvsp[(1) - (2)].start)) ;
		  INST *p2 = CDP((yyvsp[(2) - (2)].start)) ;

                  if ( p1 != p2 )  /* real test in loop */
		  {
		    p1[1].op = code_ptr-(p1+1) ;
		    saved_offset = code_offset ;
		    len = code_pop(code_ptr) ;
		    code_ptr += len ;
		    code_jmp(_JNZ, CDP((yyvsp[(2) - (2)].start))) ;
		    BC_clear(code_ptr, CDP(saved_offset)) ;
		  }
		  else /* while(1) */
		  {
		    code_jmp(_JMP, p1) ;
		    BC_clear(code_ptr, CDP((yyvsp[(2) - (2)].start))) ;
		  }
                }
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 629 "parse.y"
    {
		  int cont_offset = code_offset ;
                  unsigned len = code_pop(code_ptr) ;
		  INST *p2 = CDP((yyvsp[(2) - (4)].start)) ;
		  INST *p4 = CDP((yyvsp[(4) - (4)].start)) ;

                  code_ptr += len ;

		  if ( p2 != p4 )  /* real test in for2 */
		  {
                    p4[-1].op = code_ptr - p4 + 1 ;
		    len = code_pop(code_ptr) ;
		    code_ptr += len ;
                    code_jmp(_JNZ, CDP((yyvsp[(4) - (4)].start))) ;
		  }
		  else /*  for(;;) */
		  code_jmp(_JMP, p4) ;

		  BC_clear(code_ptr, CDP(cont_offset)) ;

                }
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 652 "parse.y"
    { (yyval.start) = code_offset ; }
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 654 "parse.y"
    { (yyval.start) = (yyvsp[(3) - (4)].start) ; code1(_POP) ; }
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 657 "parse.y"
    { (yyval.start) = code_offset ; }
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 659 "parse.y"
    {
             if ( code_ptr - 2 == CDP((yyvsp[(1) - (2)].start)) &&
                  code_ptr[-2].op == _PUSHD &&
                  * (double*) code_ptr[-1].ptr != 0.0
                )
                    code_ptr -= 2 ;
             else
	     {
	       INST *p1 = CDP((yyvsp[(1) - (2)].start)) ;
	       code_push(p1, code_ptr-p1, scope, active_funct) ;
	       code_ptr = p1 ;
	       code2(_JMP, (INST*)0) ;
	     }
           }
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 676 "parse.y"
    { eat_nl() ; BC_new() ;
	     code_push((INST*)0,0, scope, active_funct) ;
	   }
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 680 "parse.y"
    { INST *p1 = CDP((yyvsp[(1) - (2)].start)) ;

	     eat_nl() ; BC_new() ;
             code1(_POP) ;
             code_push(p1, code_ptr - p1, scope, active_funct) ;
             code_ptr -= code_ptr - p1 ;
           }
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 693 "parse.y"
    { check_array((yyvsp[(3) - (3)].stp)) ;
             code_array((yyvsp[(3) - (3)].stp)) ;
             code1(A_TEST) ;
            }
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 698 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (5)].arg2p)->start ;
             code2op(A_CAT, (yyvsp[(2) - (5)].arg2p)->cnt) ;
             zfree((yyvsp[(2) - (5)].arg2p), sizeof(ARG2_REC)) ;

             check_array((yyvsp[(5) - (5)].stp)) ;
             code_array((yyvsp[(5) - (5)].stp)) ;
             code1(A_TEST) ;
           }
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 709 "parse.y"
    {
             if ( (yyvsp[(4) - (5)].ival) > 1 )
             { code2op(A_CAT, (yyvsp[(4) - (5)].ival)) ; }

             check_array((yyvsp[(1) - (5)].stp)) ;
             if( is_local((yyvsp[(1) - (5)].stp)) )
             { code2op(LAE_PUSHA, (yyvsp[(1) - (5)].stp)->offset) ; }
             else code2(AE_PUSHA, (yyvsp[(1) - (5)].stp)->stval.array) ;
             (yyval.start) = (yyvsp[(2) - (5)].start) ;
           }
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 722 "parse.y"
    {
             if ( (yyvsp[(4) - (5)].ival) > 1 )
             { code2op(A_CAT, (yyvsp[(4) - (5)].ival)) ; }

             check_array((yyvsp[(1) - (5)].stp)) ;
             if( is_local((yyvsp[(1) - (5)].stp)) )
             { code2op(LAE_PUSHI, (yyvsp[(1) - (5)].stp)->offset) ; }
             else code2(AE_PUSHI, (yyvsp[(1) - (5)].stp)->stval.array) ;
             (yyval.start) = (yyvsp[(2) - (5)].start) ;
           }
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 734 "parse.y"
    {
             if ( (yyvsp[(4) - (6)].ival) > 1 )
             { code2op(A_CAT,(yyvsp[(4) - (6)].ival)) ; }

             check_array((yyvsp[(1) - (6)].stp)) ;
             if( is_local((yyvsp[(1) - (6)].stp)) )
             { code2op(LAE_PUSHA, (yyvsp[(1) - (6)].stp)->offset) ; }
             else code2(AE_PUSHA, (yyvsp[(1) - (6)].stp)->stval.array) ;
             if ( (yyvsp[(6) - (6)].ival) == '+' )  code1(_POST_INC) ;
             else  code1(_POST_DEC) ;

             (yyval.start) = (yyvsp[(2) - (6)].start) ;
           }
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 751 "parse.y"
    {
               (yyval.start) = (yyvsp[(3) - (7)].start) ;
               if ( (yyvsp[(5) - (7)].ival) > 1 ) { code2op(A_CAT, (yyvsp[(5) - (7)].ival)) ; }
               check_array((yyvsp[(2) - (7)].stp)) ;
               code_array((yyvsp[(2) - (7)].stp)) ;
               code1(A_DEL) ;
             }
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 759 "parse.y"
    {
		(yyval.start) = code_offset ;
		check_array((yyvsp[(2) - (3)].stp)) ;
		code_array((yyvsp[(2) - (3)].stp)) ;
		code1(DEL_A) ;
	     }
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 770 "parse.y"
    { eat_nl() ; BC_new() ;
                      (yyval.start) = code_offset ;

                      check_var((yyvsp[(3) - (6)].stp)) ;
                      code_address((yyvsp[(3) - (6)].stp)) ;
                      check_array((yyvsp[(5) - (6)].stp)) ;
                      code_array((yyvsp[(5) - (6)].stp)) ;

                      code2(SET_ALOOP, (INST*)0) ;
                    }
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 784 "parse.y"
    {
		INST *p2 = CDP((yyvsp[(2) - (2)].start)) ;

	        p2[-1].op = code_ptr - p2 + 1 ;
                BC_clear( code_ptr+2 , code_ptr) ;
		code_jmp(ALOOP, p2) ;
		code1(POP_AL) ;
              }
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 801 "parse.y"
    { (yyval.start) = code_offset ; code2(F_PUSHA, (yyvsp[(1) - (1)].cp)) ; }
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 803 "parse.y"
    { check_var((yyvsp[(2) - (2)].stp)) ;
             (yyval.start) = code_offset ;
             if ( is_local((yyvsp[(2) - (2)].stp)) )
             { code2op(L_PUSHI, (yyvsp[(2) - (2)].stp)->offset) ; }
             else code2(_PUSHI, (yyvsp[(2) - (2)].stp)->stval.cp) ;

	     CODE_FE_PUSHA() ;
           }
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 812 "parse.y"
    {
             if ( (yyvsp[(5) - (6)].ival) > 1 )
             { code2op(A_CAT, (yyvsp[(5) - (6)].ival)) ; }

             check_array((yyvsp[(2) - (6)].stp)) ;
             if( is_local((yyvsp[(2) - (6)].stp)) )
             { code2op(LAE_PUSHI, (yyvsp[(2) - (6)].stp)->offset) ; }
             else code2(AE_PUSHI, (yyvsp[(2) - (6)].stp)->stval.array) ;

	     CODE_FE_PUSHA()  ;

             (yyval.start) = (yyvsp[(3) - (6)].start) ;
           }
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 826 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ;  CODE_FE_PUSHA() ; }
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 828 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 832 "parse.y"
    { field_A2I() ; }
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 835 "parse.y"
    { code1(F_ASSIGN) ; }
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 836 "parse.y"
    { code1(F_ADD_ASG) ; }
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 837 "parse.y"
    { code1(F_SUB_ASG) ; }
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 838 "parse.y"
    { code1(F_MUL_ASG) ; }
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 839 "parse.y"
    { code1(F_DIV_ASG) ; }
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 840 "parse.y"
    { code1(F_MOD_ASG) ; }
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 841 "parse.y"
    { code1(F_POW_ASG) ; }
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 848 "parse.y"
    { code2(_BUILTIN, bi_split) ; }
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 852 "parse.y"
    { (yyval.start) = (yyvsp[(3) - (5)].start) ;
              check_array((yyvsp[(5) - (5)].stp)) ;
              code_array((yyvsp[(5) - (5)].stp))  ;
            }
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 859 "parse.y"
    { code2(_PUSHI, &fs_shadow) ; }
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 861 "parse.y"
    {
                  if ( CDP((yyvsp[(2) - (3)].start)) == code_ptr - 2 )
                  {
                    if (code_ptr[-2].op == _MATCH0) {
                        RE_as_arg() ;
			{
			    /* see if // needs conversion */
			    CELL* cp = (CELL*) code_ptr[-1].ptr ;
			    if (REempty(cp->ptr)) {
			        cp->type = C_SNULL ;
				cp->ptr = 0 ;
			    }
			}
		    }
                    else
                    if ( code_ptr[-2].op == _PUSHS )
                    { CELL *cp = ZMALLOC(CELL) ;

                      cp->type = C_STRING ;
                      cp->ptr = code_ptr[-1].ptr ;
                      cast_for_split(cp) ;
                      code_ptr[-2].op = _PUSHC ;
                      code_ptr[-1].ptr = (PTR) cp ;
                    }
                  }
                }
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 896 "parse.y"
    { (yyval.start) = code_offset ;
	    code2(_PUSHI,field) ;
            code2(_BUILTIN,bi_length) ;
	  }
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 901 "parse.y"
    { (yyval.start) = code_offset ;
	    code2(_PUSHI,field) ;
            code2(_BUILTIN,bi_length) ;
	  }
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 906 "parse.y"
    { (yyval.start) = (yyvsp[(3) - (4)].start) ;
            code2(_BUILTIN,bi_length) ;
	  }
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 910 "parse.y"
    {
	      SYMTAB* stp = (yyvsp[(3) - (4)].stp) ;
	      (yyval.start) = code_offset ;
	      switch(stp->type) {
	          case ST_VAR:
		      code2(_PUSHI, stp->stval.cp) ;
		      code2(_BUILTIN, bi_length) ;
		      break ;
		  case ST_ARRAY:
		      code2(A_PUSHA, stp->stval.array) ;
		      code2(_BUILTIN, bi_alength) ;
		      break ;
		  case ST_LOCAL_VAR:
		      code2op(L_PUSHI, stp->offset) ;
		      code2(_BUILTIN, bi_length) ;
		      break ;
		  case ST_LOCAL_ARRAY:
		      code2op(LA_PUSHA, stp->offset) ;
		      code2(_BUILTIN, bi_alength) ;
		      break ;

		  case ST_NONE:
		      /* could go on modified resolve list, but too much work to
		         figure that out.  Will be patched at run time */
		       code2(PI_LOAD, stp) ;
		       code2(_BUILTIN, bi_length) ;
		       break ;
		   case ST_LOCAL_NONE:
		       {   /* ditto, patched at run-time */
		           Local_PI* pi = (Local_PI *)zmalloc(sizeof(Local_PI)) ;
			   pi->fbp = active_funct ;
			   pi->offset = stp->offset ;
			   code2(LPI_LOAD, pi) ;
			   code2(_BUILTIN, bi_length) ;
			}
			break ;
		   default:
		       type_error(stp) ;
		       break ;
	      }
	   }
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 958 "parse.y"
    { (yyval.start) = (yyvsp[(3) - (6)].start) ;
          code2(_BUILTIN, bi_match) ;
        }
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 965 "parse.y"
    {
	       INST *p1 = CDP((yyvsp[(1) - (1)].start)) ;

               if ( p1 == code_ptr - 2 )
               {
                 if ( p1->op == _MATCH0 ) RE_as_arg() ;
                 else
                 if ( p1->op == _PUSHS )
                 { CELL *cp = ZMALLOC(CELL) ;

                   cp->type = C_STRING ;
                   cp->ptr = p1[1].ptr ;
                   cast_to_RE(cp) ;
                   p1->op = _PUSHC ;
                   p1[1].ptr = (PTR) cp ;
                 }
               }
             }
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 988 "parse.y"
    { (yyval.start) = code_offset ;
                      code1(_EXIT0) ; }
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 991 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; code1(_EXIT) ; }
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 994 "parse.y"
    { (yyval.start) = code_offset ;
                      code1(_RET0) ; }
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 997 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; code1(_RET) ; }
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 1002 "parse.y"
    { (yyval.start) = code_offset ;
            code2(F_PUSHA, &field[0]) ;
            code1(_PUSHINT) ; code1(0) ;
            code2(_BUILTIN, bi_getline) ;
            getline_flag = 0 ;
          }
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 1009 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (2)].start) ;
            code1(_PUSHINT) ; code1(0) ;
            code2(_BUILTIN, bi_getline) ;
            getline_flag = 0 ;
          }
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 1015 "parse.y"
    { code1(_PUSHINT) ; code1(F_IN) ;
            code2(_BUILTIN, bi_getline) ;
            /* getline_flag already off in yylex() */
          }
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 1020 "parse.y"
    { code2(F_PUSHA, &field[0]) ;
            code1(_PUSHINT) ; code1(PIPE_IN) ;
            code2(_BUILTIN, bi_getline) ;
          }
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 1025 "parse.y"
    {
            code1(_PUSHINT) ; code1(PIPE_IN) ;
            code2(_BUILTIN, bi_getline) ;
          }
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 1031 "parse.y"
    { getline_flag = 1 ; }
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 1036 "parse.y"
    { (yyval.start) = code_offset ;
                   code2(F_PUSHA, field+0) ;
                 }
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 1040 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 1048 "parse.y"
    {
	     INST *p5 = CDP((yyvsp[(5) - (6)].start)) ;
	     INST *p6 = CDP((yyvsp[(6) - (6)].start)) ;

             if ( p6 - p5 == 2 && p5->op == _PUSHS  )
             { /* cast from STRING to REPL at compile time */
               CELL *cp = ZMALLOC(CELL) ;
               cp->type = C_STRING ;
               cp->ptr = p5[1].ptr ;
               cast_to_REPL(cp) ;
               p5->op = _PUSHC ;
               p5[1].ptr = (PTR) cp ;
             }
             code2(_BUILTIN, (yyvsp[(1) - (6)].fp)) ;
             (yyval.start) = (yyvsp[(3) - (6)].start) ;
           }
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 1066 "parse.y"
    { (yyval.fp) = bi_sub ; }
    break;

  case 164:

/* Line 1455 of yacc.c  */
#line 1067 "parse.y"
    { (yyval.fp) = bi_gsub ; }
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 1072 "parse.y"
    { (yyval.start) = code_offset ;
                  code2(F_PUSHA, &field[0]) ;
                }
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 1077 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ; }
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 1085 "parse.y"
    {
		   resize_fblock((yyvsp[(1) - (2)].fbp)) ;
                   restore_ids() ;
		   switch_code_to_main() ;
                 }
    break;

  case 168:

/* Line 1455 of yacc.c  */
#line 1094 "parse.y"
    { eat_nl() ;
                   scope = SCOPE_FUNCT ;
                   active_funct = (yyvsp[(1) - (4)].fbp) ;
                   *main_code_p = active_code ;

		   (yyvsp[(1) - (4)].fbp)->nargs = (yyvsp[(3) - (4)].ival) ;
                   if ( (yyvsp[(3) - (4)].ival) )
                        (yyvsp[(1) - (4)].fbp)->typev = (char *)
			    memset( zmalloc((yyvsp[(3) - (4)].ival)), ST_LOCAL_NONE, (yyvsp[(3) - (4)].ival)) ;
                   else (yyvsp[(1) - (4)].fbp)->typev = (char *) 0 ;

		   code_ptr = code_base =
                       (INST *) zmalloc(INST_BYTES(PAGESZ));
		   code_limit = code_base + PAGESZ ;
		   code_warn = code_limit - CODEWARN ;
                 }
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 1113 "parse.y"
    { FBLOCK  *fbp ;

                   if ( (yyvsp[(2) - (2)].stp)->type == ST_NONE )
                   {
                         (yyvsp[(2) - (2)].stp)->type = ST_FUNCT ;
                         fbp = (yyvsp[(2) - (2)].stp)->stval.fbp =
                             (FBLOCK *) zmalloc(sizeof(FBLOCK)) ;
                         fbp->name = (yyvsp[(2) - (2)].stp)->name ;
			 fbp->code = (INST*) 0 ;
                   }
                   else
                   {
                         type_error( (yyvsp[(2) - (2)].stp) ) ;

                         /* this FBLOCK will not be put in
                            the symbol table */
                         fbp = (FBLOCK*) zmalloc(sizeof(FBLOCK)) ;
                         fbp->name = "" ;
                   }
                   (yyval.fbp) = fbp ;
                 }
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 1136 "parse.y"
    { (yyval.fbp) = (yyvsp[(2) - (2)].fbp) ;
                   if ( (yyvsp[(2) - (2)].fbp)->code )
                       compile_error("redefinition of %s" , (yyvsp[(2) - (2)].fbp)->name) ;
                 }
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 1142 "parse.y"
    { (yyval.ival) = 0 ; }
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 1147 "parse.y"
    { (yyvsp[(1) - (1)].stp) = save_id((yyvsp[(1) - (1)].stp)->name) ;
                (yyvsp[(1) - (1)].stp)->type = ST_LOCAL_NONE ;
                (yyvsp[(1) - (1)].stp)->offset = 0 ;
                (yyval.ival) = 1 ;
              }
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 1153 "parse.y"
    { if ( is_local((yyvsp[(3) - (3)].stp)) )
                  compile_error("%s is duplicated in argument list",
                    (yyvsp[(3) - (3)].stp)->name) ;
                else
                { (yyvsp[(3) - (3)].stp) = save_id((yyvsp[(3) - (3)].stp)->name) ;
                  (yyvsp[(3) - (3)].stp)->type = ST_LOCAL_NONE ;
                  (yyvsp[(3) - (3)].stp)->offset = (yyvsp[(1) - (3)].ival) ;
                  (yyval.ival) = (yyvsp[(1) - (3)].ival) + 1 ;
                }
              }
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 1166 "parse.y"
    {  /* we may have to recover from a bungled function
		       definition */
		   /* can have local ids, before code scope
		      changes  */
		    restore_ids() ;

		    switch_code_to_main() ;
		 }
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 1179 "parse.y"
    { (yyval.start) = (yyvsp[(2) - (3)].start) ;
             code2(_CALL, (yyvsp[(1) - (3)].fbp)) ;

             if ( (yyvsp[(3) - (3)].ca_p) )  code1((yyvsp[(3) - (3)].ca_p)->arg_num+1) ;
             else  code1(0) ;

	     check_fcall((yyvsp[(1) - (3)].fbp), scope, code_move_level, active_funct,
			 (yyvsp[(3) - (3)].ca_p), token_lineno) ;
           }
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 1191 "parse.y"
    { (yyval.ca_p) = (CA_REC *) 0 ; }
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 1193 "parse.y"
    { (yyval.ca_p) = (yyvsp[(2) - (2)].ca_p) ;
                 (yyval.ca_p)->link = (yyvsp[(1) - (2)].ca_p) ;
                 (yyval.ca_p)->arg_num = (yyvsp[(1) - (2)].ca_p) ? (yyvsp[(1) - (2)].ca_p)->arg_num+1 : 0 ;
               }
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 1208 "parse.y"
    { (yyval.ca_p) = (CA_REC *) 0 ; }
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 1210 "parse.y"
    { (yyval.ca_p) = ZMALLOC(CA_REC) ;
                (yyval.ca_p)->link = (yyvsp[(1) - (3)].ca_p) ;
                (yyval.ca_p)->type = CA_EXPR  ;
                (yyval.ca_p)->arg_num = (yyvsp[(1) - (3)].ca_p) ? (yyvsp[(1) - (3)].ca_p)->arg_num+1 : 0 ;
		(yyval.ca_p)->call_offset = code_offset ;
              }
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 1217 "parse.y"
    { (yyval.ca_p) = ZMALLOC(CA_REC) ;
                (yyval.ca_p)->link = (yyvsp[(1) - (3)].ca_p) ;
                (yyval.ca_p)->arg_num = (yyvsp[(1) - (3)].ca_p) ? (yyvsp[(1) - (3)].ca_p)->arg_num+1 : 0 ;

                code_call_id((yyval.ca_p), (yyvsp[(2) - (3)].stp)) ;
              }
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 1226 "parse.y"
    { (yyval.ca_p) = ZMALLOC(CA_REC) ;
                (yyval.ca_p)->type = CA_EXPR ;
		(yyval.ca_p)->call_offset = code_offset ;
              }
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 1232 "parse.y"
    { (yyval.ca_p) = ZMALLOC(CA_REC) ;
                code_call_id((yyval.ca_p), (yyvsp[(1) - (2)].stp)) ;
              }
    break;



/* Line 1455 of yacc.c  */
#line 3756 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1240 "parse.y"


/* resize the code for a user function */

static void  resize_fblock( FBLOCK * fbp )
{
  CODEBLOCK *p = ZMALLOC(CODEBLOCK) ;
  unsigned dummy ;

  code2op(_RET0, _HALT) ;
	/* make sure there is always a return */

  *p = active_code ;
  fbp->code = code_shrink(p, &dummy) ;
      /* code_shrink() zfrees p */

  if ( dump_code_flag ) add_to_fdump_list(fbp) ;
}


/* convert FE_PUSHA  to  FE_PUSHI
   or F_PUSH to F_PUSHI
*/

static void  field_A2I(void)
{ CELL *cp ;

  if ( code_ptr[-1].op == FE_PUSHA &&
       code_ptr[-1].ptr == (PTR) 0)
  /* On most architectures, the two tests are the same; a good
     compiler might eliminate one.  On LM_DOS, and possibly other
     segmented architectures, they are not */
  { code_ptr[-1].op = FE_PUSHI ; }
  else
  {
    cp = (CELL *) code_ptr[-1].ptr ;

    if ( cp == field  || (cp > NF && cp <= LAST_PFIELD) )
    {
         code_ptr[-2].op = _PUSHI  ;
    }
    else if ( cp == NF )
    { code_ptr[-2].op = NF_PUSHI ; code_ptr-- ; }

    else
    {
      code_ptr[-2].op = F_PUSHI ;
      code_ptr -> op = field_addr_to_index( (CELL *)code_ptr[-1].ptr ) ;
      code_ptr++ ;
    }
  }
}

/* we've seen an ID in a context where it should be a VAR,
   check that's consistent with previous usage */

static void check_var(SYMTAB * p )
{
      switch(p->type)
      {
        case ST_NONE : /* new id */
            p->type = ST_VAR ;
            p->stval.cp = ZMALLOC(CELL) ;
            p->stval.cp->type = C_NOINIT ;
            break ;

        case ST_LOCAL_NONE :
            p->type = ST_LOCAL_VAR ;
            active_funct->typev[p->offset] = ST_LOCAL_VAR ;
            break ;

        case ST_VAR :
        case ST_LOCAL_VAR :  break ;

        default :
            type_error(p) ;
            break ;
      }
}

/* we've seen an ID in a context where it should be an ARRAY,
   check that's consistent with previous usage */
static  void  check_array(SYMTAB *p)
{
      switch(p->type)
      {
        case ST_NONE :  /* a new array */
            p->type = ST_ARRAY ;
            p->stval.array = new_ARRAY() ;
            break ;

        case  ST_ARRAY :
        case  ST_LOCAL_ARRAY :
            break ;

        case  ST_LOCAL_NONE  :
            p->type = ST_LOCAL_ARRAY ;
            active_funct->typev[p->offset] = ST_LOCAL_ARRAY ;
            break ;

        default : type_error(p) ; break ;
      }
}

static void code_array(SYMTAB* p)
{
  if ( is_local(p) ) code2op(LA_PUSHA, p->offset) ;
  else  code2(A_PUSHA, p->stval.array) ;
}


/* we've seen an ID as an argument to a user defined function */

static void  code_call_id(CA_REC * p, SYMTAB * ip )
{ static CELL dummy ;

  p->call_offset = code_offset ;
     /* This always get set now.  So that fcall:relocate_arglist
	works. */

  switch( ip->type )
  {
    case  ST_VAR  :
            p->type = CA_EXPR ;
            code2(_PUSHI, ip->stval.cp) ;
            break ;

    case  ST_LOCAL_VAR  :
            p->type = CA_EXPR ;
            code2op(L_PUSHI, ip->offset) ;
            break ;

    case  ST_ARRAY  :
            p->type = CA_ARRAY ;
            code2(A_PUSHA, ip->stval.array) ;
            break ;

    case  ST_LOCAL_ARRAY :
            p->type = CA_ARRAY ;
            code2op(LA_PUSHA, ip->offset) ;
            break ;

    /* not enough info to code it now; it will have to
       be patched later */

    case  ST_NONE :
            p->type = ST_NONE ;
            p->sym_p = ip ;
            code2(_PUSHI, &dummy) ;
            break ;

    case  ST_LOCAL_NONE :
            p->type = ST_LOCAL_NONE ;
            p->type_p = & active_funct->typev[ip->offset] ;
            code2op(L_PUSHI, ip->offset) ;
            break ;


#ifdef   DEBUG
    default :
            bozo("code_call_id") ;
#endif

  }
}

/* an RE by itself was coded as _MATCH0 , change to
   push as an expression */

static void RE_as_arg(void)
{ CELL *cp = ZMALLOC(CELL) ;

  code_ptr -= 2 ;
  cp->type = C_RE ;
  cp->ptr = code_ptr[1].ptr ;
  code2(_PUSHC, cp) ;
}

/* reset the active_code back to the MAIN block */
static void
switch_code_to_main(void)
{
   switch(scope)
   {
     case SCOPE_BEGIN :
	*begin_code_p = active_code ;
	active_code = *main_code_p ;
	break ;

     case SCOPE_END :
	*end_code_p = active_code ;
	active_code = *main_code_p ;
	break ;

     case SCOPE_FUNCT :
	active_code = *main_code_p ;
	break ;

     case SCOPE_MAIN :
	break ;
   }
   active_funct = (FBLOCK*) 0 ;
   scope = SCOPE_MAIN ;
}


void
parse(void)
{
   if ( yyparse() || compile_error_count != 0 ) mawk_exit(2) ;

   scan_cleanup() ;
   set_code() ;
   /* code must be set before call to resolve_fcalls() */
   if ( resolve_list )  resolve_fcalls() ;

   if ( compile_error_count != 0 ) mawk_exit(2) ;
   if ( dump_code_flag ) { dump_code() ; mawk_exit(0) ; }
}


