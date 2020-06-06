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
/* "%code requires" blocks.  */
#line 58 "semanticRules.y" /* yacc.c:1909  */

	#include <vector>
	using namespace std;

#line 49 "y.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    WHILE = 258,
    IF_KEY = 259,
    ELSE_KEY = 260,
    INT_KEY = 261,
    FLOAT_KEY = 262,
    SEMI_COLON = 263,
    COMA = 264,
    BRACKET_OPEN = 265,
    BRACKET_CLOSE = 266,
    CURLY_OPEN = 267,
    CURLY_CLOSE = 268,
    SYSTEM_OUT = 269,
    INT = 270,
    FLOAT = 271,
    RELOP = 272,
    ASSIGN = 273,
    ADDOP = 274,
    MULOP = 275,
    ID = 276
  };
#endif
/* Tokens.  */
#define WHILE 258
#define IF_KEY 259
#define ELSE_KEY 260
#define INT_KEY 261
#define FLOAT_KEY 262
#define SEMI_COLON 263
#define COMA 264
#define BRACKET_OPEN 265
#define BRACKET_CLOSE 266
#define CURLY_OPEN 267
#define CURLY_CLOSE 268
#define SYSTEM_OUT 269
#define INT 270
#define FLOAT 271
#define RELOP 272
#define ASSIGN 273
#define ADDOP 274
#define MULOP 275
#define ID 276

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 85 "semanticRules.y" /* yacc.c:1909  */


    int intunion;
    float floatunion;
    char * stringsunion;
    struct {
		vector<int> *nextlist;
	} statement_type;
    
    struct {
		vector<int> *trueList, *falseList;
	} bool_type;
    
    struct {
		int sType;
	}exp_type;

#line 121 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
