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
    ARRAY = 258,
    DO = 259,
    ELSE = 260,
    END = 261,
    FUNCTION = 262,
    IF = 263,
    NOT = 264,
    OF = 265,
    PROCEDURE = 266,
    PROGRAM = 267,
    THEN = 268,
    VAR = 269,
    WHILE = 270,
    IDENTIFIER = 271,
    ASSIGNMENT = 272,
    COLON = 273,
    COMMA = 274,
    DIGSEQ = 275,
    DOT = 276,
    DOTDOT = 277,
    EQUAL = 278,
    GE = 279,
    GT = 280,
    LBRAC = 281,
    LE = 282,
    LPAREN = 283,
    LT = 284,
    MINUS = 285,
    PLUS = 286,
    RBRAC = 287,
    REALNUMBER = 288,
    RPAREN = 289,
    SEMICOLON = 290,
    SLASH = 291,
    STAR = 292,
    notEQUAL = 293,
    STRING = 294,
    real = 295,
    INTEGER = 296,
    begin = 297,
    string_v = 298
  };
#endif
/* Tokens.  */
#define ARRAY 258
#define DO 259
#define ELSE 260
#define END 261
#define FUNCTION 262
#define IF 263
#define NOT 264
#define OF 265
#define PROCEDURE 266
#define PROGRAM 267
#define THEN 268
#define VAR 269
#define WHILE 270
#define IDENTIFIER 271
#define ASSIGNMENT 272
#define COLON 273
#define COMMA 274
#define DIGSEQ 275
#define DOT 276
#define DOTDOT 277
#define EQUAL 278
#define GE 279
#define GT 280
#define LBRAC 281
#define LE 282
#define LPAREN 283
#define LT 284
#define MINUS 285
#define PLUS 286
#define RBRAC 287
#define REALNUMBER 288
#define RPAREN 289
#define SEMICOLON 290
#define SLASH 291
#define STAR 292
#define notEQUAL 293
#define STRING 294
#define real 295
#define INTEGER 296
#define begin 297
#define string_v 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 19 "0416001_parser.y" /* yacc.c:1909  */
 
    struct node * node_t ;

#line 144 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
