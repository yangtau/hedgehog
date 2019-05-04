/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "hedgehog.y" /* yacc.c:339  */

/* Created by Tau on 05/02/2019 */

// #define YYDEBUG 1
#include <stdint.h>
#include <stdio.h>
#include "hedgehog.h"


#line 76 "y.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ASSIGN = 258,
    AND = 259,
    OR = 260,
    EQ = 261,
    GE = 262,
    LE = 263,
    NQ = 264,
    GT = 265,
    LT = 266,
    MUL = 267,
    DIV = 268,
    MOD = 269,
    ADD = 270,
    SUB = 271,
    POWER = 272,
    NOT = 273,
    LP = 274,
    RP = 275,
    LB = 276,
    RB = 277,
    IF = 278,
    ELSE_IF = 279,
    ELSE = 280,
    FOR = 281,
    BREAK = 282,
    CONTINUE = 283,
    RETURN = 284,
    AS = 285,
    WITH = 286,
    IN = 287,
    ON = 288,
    SWITCH = 289,
    FUNC = 290,
    COMMA = 291,
    CR = 292,
    TAB = 293,
    SEMIC = 294,
    IDENTIFIER = 295,
    DOUBLE = 296,
    INT = 297,
    BOOL = 298,
    NULL_V = 299,
    STRING = 300
  };
#endif
/* Tokens.  */
#define ASSIGN 258
#define AND 259
#define OR 260
#define EQ 261
#define GE 262
#define LE 263
#define NQ 264
#define GT 265
#define LT 266
#define MUL 267
#define DIV 268
#define MOD 269
#define ADD 270
#define SUB 271
#define POWER 272
#define NOT 273
#define LP 274
#define RP 275
#define LB 276
#define RB 277
#define IF 278
#define ELSE_IF 279
#define ELSE 280
#define FOR 281
#define BREAK 282
#define CONTINUE 283
#define RETURN 284
#define AS 285
#define WITH 286
#define IN 287
#define ON 288
#define SWITCH 289
#define FUNC 290
#define COMMA 291
#define CR 292
#define TAB 293
#define SEMIC 294
#define IDENTIFIER 295
#define DOUBLE 296
#define INT 297
#define BOOL 298
#define NULL_V 299
#define STRING 300

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 10 "hedgehog.y" /* yacc.c:355  */

    String* identifier;
    Value value;
    void *statement;
    void *expression;
    StatementList *statement_list;
    ArgumentList *argument_list;
    ParameterList *parameter_list;

#line 216 "y.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 233 "y.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  48
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   253

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  64
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  115

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
      45
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    56,    56,    61,    67,    71,    77,    82,    88,    92,
      94,    96,    98,   102,   106,   110,   116,   120,   124,   130,
     134,   140,   148,   152,   159,   163,   170,   174,   181,   187,
     193,   195,   201,   203,   209,   211,   215,   221,   223,   227,
     231,   235,   241,   243,   247,   253,   255,   259,   263,   269,
     271,   275,   281,   283,   289,   291,   295,   299,   305,   307,
     309,   311,   313,   317,   321
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ASSIGN", "AND", "OR", "EQ", "GE", "LE",
  "NQ", "GT", "LT", "MUL", "DIV", "MOD", "ADD", "SUB", "POWER", "NOT",
  "LP", "RP", "LB", "RB", "IF", "ELSE_IF", "ELSE", "FOR", "BREAK",
  "CONTINUE", "RETURN", "AS", "WITH", "IN", "ON", "SWITCH", "FUNC",
  "COMMA", "CR", "TAB", "SEMIC", "IDENTIFIER", "DOUBLE", "INT", "BOOL",
  "NULL_V", "STRING", "$accept", "GLOBAL_LIST", "STATEMENT_BLOCK",
  "STATEMENT_LIST", "STATEMENT", "FOR_STATEMENT", "IF_STATEMENT",
  "FUNCTION_DEFINE_STATEMENT", "PARAMETER_LIST", "ARGUMENT_LIST",
  "EXPRESSION", "OR_EXPRESSION", "AND_EXPRESSION", "EQUAL_EXPRESSION",
  "GREATER_EXPRESSION", "ADD_EXPRESSION", "MUL_EXPRESSION",
  "UNARY_EXPRESSION", "POWER_EXPRESSION", "VALUE_EXPRESSION", "VALUE",
  "FUNCTION_CALL_EXPRESSION", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300
};
# endif

#define YYPACT_NINF -42

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-42)))

#define YYTABLE_NINF -25

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     158,   178,   178,   208,   178,    18,   -28,   -26,   172,   -17,
       6,   -42,   -42,   -42,   -42,   -42,    68,   -42,   -42,    41,
     -42,     7,    -6,    44,    49,    45,    62,    29,    69,   -42,
      38,   -42,   -42,   -42,    55,   -42,   -42,    56,    17,    98,
     -42,    -4,   -42,   -42,   -42,     2,    66,   188,   -42,   -42,
     178,    71,   178,    50,   -42,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   178,   178,   178,   178,   -14,   -42,
     -42,   -42,   128,   -42,   208,   -42,   -42,    -8,   -42,    20,
      44,    17,   -42,    57,   -42,    49,    45,    62,    62,    29,
      29,    29,    29,    69,    69,   -42,   -42,   -42,   -42,   -42,
     -42,    61,    71,   -42,    28,   -42,   178,   -42,   208,   -42,
      71,    44,    71,   -42,   -42
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      57,    59,    58,    60,    61,    62,     0,     2,    10,     9,
      11,     0,     0,    28,    30,    32,    34,    37,    42,    45,
      49,    52,    55,    54,    57,    50,    51,     0,     0,     0,
      18,     0,    12,    13,    14,     0,     0,     0,     1,     3,
       0,     0,     0,     0,     8,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    56,
      19,     5,     0,     6,     0,    17,    15,     0,    63,     0,
      26,     0,    21,    29,    25,    31,    33,    35,    36,    41,
      39,    40,    38,    43,    44,    46,    47,    48,    53,     4,
       7,     0,     0,    24,     0,    64,     0,    20,     0,    22,
       0,    27,     0,    23,    16
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -42,   -42,   -35,   -42,   -15,   -42,   -42,   -42,    24,    52,
      -3,     0,    47,    51,    31,   -41,    35,    13,   -42,    54,
     -42,   -42
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    16,    40,    72,    17,    18,    19,    20,    21,    79,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      37,    49,    41,    70,    38,     3,    75,    55,    45,   -24,
      52,    42,   102,    43,    35,    36,    82,    39,    89,    90,
      91,    92,    55,    46,    73,    47,    34,    11,    12,    13,
      14,    15,   103,    54,     1,    74,     2,     3,    39,    39,
     105,    76,   -24,    53,    63,    64,   107,    80,   110,    55,
      81,    57,    80,    56,    58,    68,   106,   100,    10,    11,
      12,    13,    14,    15,    53,    50,    51,   109,    48,    59,
      60,   101,    61,    62,    47,   113,    69,   114,    95,    96,
      97,    65,    66,    67,     1,    77,     2,     3,    87,    88,
      84,     4,    39,   106,     5,     6,     7,     8,    93,    94,
     108,   104,    85,     9,    83,   112,   111,    86,    10,    11,
      12,    13,    14,    15,     1,     0,     2,     3,     0,     0,
      71,     4,    98,     0,     5,     6,     7,     8,     0,     0,
       0,     0,     0,     9,     0,     0,     0,     0,    10,    11,
      12,    13,    14,    15,     1,     0,     2,     3,     0,     0,
      99,     4,     0,     0,     5,     6,     7,     8,     0,     0,
       0,     0,     0,     9,     0,     0,     0,     0,    10,    11,
      12,    13,    14,    15,     1,     0,     2,     3,     0,     0,
       0,     4,     0,     0,     5,     6,     7,     8,     1,     0,
       2,     3,     0,     9,     1,     0,     2,     3,    10,    11,
      12,    13,    14,    15,     1,     0,     2,     3,    78,     0,
       0,    44,    34,    11,    12,    13,    14,    15,    34,    11,
      12,    13,    14,    15,     1,     0,     2,     3,    34,    11,
      12,    13,    14,    15,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,    11,
      12,    13,    14,    15
};

static const yytype_int8 yycheck[] =
{
       3,    16,     5,    38,     4,    19,    41,     5,     8,     3,
       3,    39,    20,    39,     1,     2,    51,    21,    59,    60,
      61,    62,     5,    40,    39,    19,    40,    41,    42,    43,
      44,    45,    40,    39,    16,    39,    18,    19,    21,    21,
      20,    39,    36,    36,    15,    16,    81,    47,    20,     5,
      50,     6,    52,     4,     9,    17,    36,    72,    40,    41,
      42,    43,    44,    45,    36,    24,    25,   102,     0,     7,
       8,    74,    10,    11,    19,   110,    20,   112,    65,    66,
      67,    12,    13,    14,    16,    19,    18,    19,    57,    58,
      40,    23,    21,    36,    26,    27,    28,    29,    63,    64,
      39,    77,    55,    35,    52,   108,   106,    56,    40,    41,
      42,    43,    44,    45,    16,    -1,    18,    19,    -1,    -1,
      22,    23,    68,    -1,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      42,    43,    44,    45,    16,    -1,    18,    19,    -1,    -1,
      22,    23,    -1,    -1,    26,    27,    28,    29,    -1,    -1,
      -1,    -1,    -1,    35,    -1,    -1,    -1,    -1,    40,    41,
      42,    43,    44,    45,    16,    -1,    18,    19,    -1,    -1,
      -1,    23,    -1,    -1,    26,    27,    28,    29,    16,    -1,
      18,    19,    -1,    35,    16,    -1,    18,    19,    40,    41,
      42,    43,    44,    45,    16,    -1,    18,    19,    20,    -1,
      -1,    39,    40,    41,    42,    43,    44,    45,    40,    41,
      42,    43,    44,    45,    16,    -1,    18,    19,    40,    41,
      42,    43,    44,    45,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    40,    41,
      42,    43,    44,    45
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    16,    18,    19,    23,    26,    27,    28,    29,    35,
      40,    41,    42,    43,    44,    45,    47,    50,    51,    52,
      53,    54,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    40,    63,    63,    56,    57,    21,
      48,    56,    39,    39,    39,    57,    40,    19,     0,    50,
      24,    25,     3,    36,    39,     5,     4,     6,     9,     7,
       8,    10,    11,    15,    16,    12,    13,    14,    17,    20,
      48,    22,    49,    50,    39,    48,    39,    19,    20,    55,
      57,    57,    48,    55,    40,    58,    59,    60,    60,    61,
      61,    61,    61,    62,    62,    63,    63,    63,    65,    22,
      50,    56,    20,    40,    54,    20,    36,    48,    39,    48,
      20,    57,    56,    48,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    46,    47,    47,    48,    48,    49,    49,    50,    50,
      50,    50,    50,    50,    50,    50,    51,    51,    51,    52,
      52,    52,    53,    53,    54,    54,    55,    55,    56,    56,
      57,    57,    58,    58,    59,    59,    59,    60,    60,    60,
      60,    60,    61,    61,    61,    62,    62,    62,    62,    63,
      63,    63,    64,    64,    65,    65,    65,    65,    66,    66,
      66,    66,    66,    67,    67
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     3,     2,     1,     2,     2,     1,
       1,     1,     2,     2,     2,     3,     7,     3,     2,     3,
       4,     3,     5,     6,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     2,     1,     3,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     3,     4
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 56 "hedgehog.y" /* yacc.c:1646  */
    {
    	StatementList* list = getCurrentInterpreter()->list;
    	(yyval.statement_list) = list->add(list, (yyvsp[0].statement));
    }
#line 1425 "y.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 61 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.statement_list) = (yyvsp[-1].statement_list)->add((yyvsp[-1].statement_list), (yyvsp[0].statement));
    }
#line 1433 "y.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 67 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.statement_list) = (yyvsp[-1].statement_list);
    }
#line 1441 "y.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 71 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.statement_list) = NULL;
    }
#line 1449 "y.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 77 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.statement_list) =  initStatementList();
    	(yyval.statement_list)->add((yyval.statement_list), (yyvsp[0].statement));
    }
#line 1458 "y.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 82 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.statement_list) = (yyvsp[-1].statement_list)->add((yyvsp[-1].statement_list), (yyvsp[0].statement));
    }
#line 1466 "y.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 88 "hedgehog.y" /* yacc.c:1646  */
    {
   	(yyval.statement) = initExpressionStatement((yyvsp[-1].expression));
    }
#line 1474 "y.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 98 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.statement) = initBreakStatement();
    }
#line 1482 "y.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 102 "hedgehog.y" /* yacc.c:1646  */
    {
   	(yyval.statement) = initContinueStatement();
    }
#line 1490 "y.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 106 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.statement) = initReturnStatement(NULL);
    }
#line 1498 "y.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 110 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.statement) = initReturnStatement((yyvsp[-1].expression));
    }
#line 1506 "y.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 116 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.statement) = initForStatement((yyvsp[-5].expression), (yyvsp[-3].expression), (yyvsp[-1].expression), (yyvsp[0].statement_list));
    }
#line 1514 "y.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 120 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.statement) = initForStatement(NULL, (yyvsp[-1].expression), NULL, (yyvsp[0].statement_list));
    }
#line 1522 "y.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 124 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.statement) = initForStatement(NULL, NULL, NULL, (yyvsp[0].statement_list));
    }
#line 1530 "y.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 130 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.statement) = initIfStatement((yyvsp[-1].expression), (yyvsp[0].statement_list));
    }
#line 1538 "y.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 134 "hedgehog.y" /* yacc.c:1646  */
    {
        IfStatement* ifS = (yyvsp[-3].statement);
    	ifS->addElsIf(ifS, (yyvsp[-1].expression), (yyvsp[0].statement_list));
        (yyval.statement)=(yyvsp[-3].statement);
    }
#line 1548 "y.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 140 "hedgehog.y" /* yacc.c:1646  */
    {
        IfStatement* ifS =  (yyvsp[-2].statement);
    	ifS->addElse(ifS, (yyvsp[0].statement_list));
        (yyval.statement)=(yyvsp[-2].statement);
    }
#line 1558 "y.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 148 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.statement) = initFunctionDefineStatement((yyvsp[-3].identifier), NULL, (yyvsp[0].statement_list));
    }
#line 1566 "y.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 152 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.statement) = initFunctionDefineStatement((yyvsp[-4].identifier), (yyvsp[-2].parameter_list), (yyvsp[0].statement_list));
    }
#line 1574 "y.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 159 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.parameter_list) = initParameterList((yyvsp[0].identifier));
    }
#line 1582 "y.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 163 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.parameter_list) = (yyvsp[-2].parameter_list)->add((yyvsp[-2].parameter_list), (yyvsp[0].identifier));
    }
#line 1590 "y.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 170 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.argument_list) = initArgumentList((yyvsp[0].expression));
    }
#line 1598 "y.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 174 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.argument_list) = (yyvsp[-2].argument_list)->add((yyvsp[-2].argument_list), (yyvsp[0].expression));
    }
#line 1606 "y.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 187 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.expression) = initMultiAssignExpression((yyvsp[-2].parameter_list), (yyvsp[0].argument_list));
    }
#line 1614 "y.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 195 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(OR_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1622 "y.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 203 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(AND_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1630 "y.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 211 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(EQ_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1638 "y.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 215 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(NQ_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1646 "y.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 223 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(LT_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1654 "y.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 227 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(LE_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1662 "y.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 231 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(GT_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1670 "y.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 235 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(GE_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1678 "y.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 243 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(ADD_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1686 "y.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 247 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(SUB_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1694 "y.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 255 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(MUL_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1702 "y.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 259 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(DIV_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1710 "y.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 263 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(MOD_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1718 "y.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 271 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initUnaryExpression(SUB_OPERATOR, (yyvsp[0].expression));
    }
#line 1726 "y.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 275 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initUnaryExpression(NOT_OPERATOR, (yyvsp[0].expression));
    }
#line 1734 "y.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 283 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initBinaryExpression(POWER_OPERATOR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 1742 "y.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 291 "hedgehog.y" /* yacc.c:1646  */
    {
       (yyval.expression) = initValueExpression((yyvsp[0].value));
    }
#line 1750 "y.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 295 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = (yyvsp[-1].expression);
    }
#line 1758 "y.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 299 "hedgehog.y" /* yacc.c:1646  */
    {
        (yyval.expression) = initVariableExpression((yyvsp[0].identifier));
    }
#line 1766 "y.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 317 "hedgehog.y" /* yacc.c:1646  */
    {
	    (yyval.expression) = initFunctionCallExpression((yyvsp[-2].identifier), NULL);
    }
#line 1774 "y.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 321 "hedgehog.y" /* yacc.c:1646  */
    {
    	(yyval.expression) = initFunctionCallExpression((yyvsp[-3].identifier), (yyvsp[-1].argument_list));
    }
#line 1782 "y.tab.c" /* yacc.c:1646  */
    break;


#line 1786 "y.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 326 "hedgehog.y" /* yacc.c:1906  */
