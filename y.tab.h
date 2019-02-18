/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 10 "hedgehog.y" /* yacc.c:1909  */

    String* identifier;
    Value value;
    void *statement;
    void *expression;
    StatementList *statement_list;
    ArgumentList *argument_list;
    ParameterList *parameter_list;

#line 154 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
