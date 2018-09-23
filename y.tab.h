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
    IDENT_TOK = 1,
    DECL_TOK = 2,
    IMPL_TOK = 3,
    CHR_TOK = 4,
    INT_TOK = 5,
    INTLIT_TOK = 6,
    CHRLIT_TOK = 7,
    GET_TOK = 8,
    PUT_TOK = 9,
    BGE_TOK = 10,
    BGT_TOK = 11,
    BLE_TOK = 12,
    BLT_TOK = 13,
    BNE_TOK = 14,
    BEQ_TOK = 15,
    WHILE_TOK = 16,
    IF_TOK = 17,
    ELSE_TOK = 18,
    STRLIT_TOK = 19,
    INCR_TOK = 20,
    DECR_TOK = 21,
    FOR_TOK = 22,
    RET_TOK = 23,
    CNST_TOK = 24,
    AND_TOK = 25,
    OR_TOK = 26,
    NOT_TOK = 27,
    INDEX_TOK = 28
  };
#endif
/* Tokens.  */
#define IDENT_TOK 1
#define DECL_TOK 2
#define IMPL_TOK 3
#define CHR_TOK 4
#define INT_TOK 5
#define INTLIT_TOK 6
#define CHRLIT_TOK 7
#define GET_TOK 8
#define PUT_TOK 9
#define BGE_TOK 10
#define BGT_TOK 11
#define BLE_TOK 12
#define BLT_TOK 13
#define BNE_TOK 14
#define BEQ_TOK 15
#define WHILE_TOK 16
#define IF_TOK 17
#define ELSE_TOK 18
#define STRLIT_TOK 19
#define INCR_TOK 20
#define DECR_TOK 21
#define FOR_TOK 22
#define RET_TOK 23
#define CNST_TOK 24
#define AND_TOK 25
#define OR_TOK 26
#define NOT_TOK 27
#define INDEX_TOK 28

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 13 "YGrammar.y" /* yacc.c:1909  */

// real-type union-field-name
  int Integer;
  char * Text;
  struct IdList * IdList;
  enum BaseTypes BaseType;
  struct InstrSeq * InstrSeq;
  struct ExprResult * ExprResult;
  struct CondResult * CondResult;
  enum BranchType BranchType;

#line 126 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
