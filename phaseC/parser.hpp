/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 30 "parser.y"

    #include "Symtable/TableEntry/SymbolTableEntry.h"
    #include "Quads/expression.h"
    #include "Quads/quad.h"
    #include "expr_handler.h"

#line 56 "parser.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    INTEGER = 258,                 /* INTEGER  */
    DOUBLE = 259,                  /* DOUBLE  */
    REALCONST = 260,               /* REALCONST  */
    IDENT = 261,                   /* IDENT  */
    IF = 262,                      /* IF  */
    ELSE = 263,                    /* ELSE  */
    WHILE = 264,                   /* WHILE  */
    FUNCTION = 265,                /* FUNCTION  */
    FOR = 266,                     /* FOR  */
    RETURN = 267,                  /* RETURN  */
    BREAK = 268,                   /* BREAK  */
    CONTINUE = 269,                /* CONTINUE  */
    AND = 270,                     /* AND  */
    NOT = 271,                     /* NOT  */
    OR = 272,                      /* OR  */
    NIL = 273,                     /* NIL  */
    TRUE = 274,                    /* TRUE  */
    FALSE = 275,                   /* FALSE  */
    LOCAL = 276,                   /* LOCAL  */
    ASSIGN = 277,                  /* ASSIGN  */
    PLUS = 278,                    /* PLUS  */
    MINUS = 279,                   /* MINUS  */
    MULT = 280,                    /* MULT  */
    DIV = 281,                     /* DIV  */
    MOD = 282,                     /* MOD  */
    EQUAL = 283,                   /* EQUAL  */
    NOT_EQUALS = 284,              /* NOT_EQUALS  */
    PLUS_PLUS = 285,               /* PLUS_PLUS  */
    MINUS_MINUS = 286,             /* MINUS_MINUS  */
    GREATER = 287,                 /* GREATER  */
    LESS = 288,                    /* LESS  */
    GREATER_EQUAL = 289,           /* GREATER_EQUAL  */
    LESS_EQUAL = 290,              /* LESS_EQUAL  */
    LEFT_BRACE = 291,              /* LEFT_BRACE  */
    RIGHT_BRACE = 292,             /* RIGHT_BRACE  */
    LEFT_BRACKET = 293,            /* LEFT_BRACKET  */
    RIGHT_BRACKET = 294,           /* RIGHT_BRACKET  */
    LEFT_PARENTHESIS = 295,        /* LEFT_PARENTHESIS  */
    RIGHT_PARENTHESIS = 296,       /* RIGHT_PARENTHESIS  */
    SEMICOLON = 297,               /* SEMICOLON  */
    COMMA = 298,                   /* COMMA  */
    DOUBLE_DOT = 299,              /* DOUBLE_DOT  */
    DOT = 300,                     /* DOT  */
    NAMESPACE = 301,               /* NAMESPACE  */
    COLON = 302,                   /* COLON  */
    STRING = 303,                  /* STRING  */
    UNDEFINED = 304,               /* UNDEFINED  */
    UMINUS = 305                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 39 "parser.y"
 
	std::string *stringValue;
	int intValue ;
    double doubleVal;
    SymbolTableEntry_T symEntry;
    std::vector<void *>* idList;
    expr* exprVal;
    stmt_t * s;
    forloop_t * loop_t;
    call_t* callVal;

#line 135 "parser.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
