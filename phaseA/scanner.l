%{
#include "header_alpha.h"
#define YY_DECL int yylex()
int mylineno = 1;  // keep track of lines

%}

/* Flex options */
%option noyywrap 
%option yylineno

/* Define Flex macros */

/*KEYWORDS*/
IF                  "if"
ELSE                "else"
WHILE               "while"
FOR                 "for"
FUNCTION            "function"
RETURN              "return"
BREAK               "break"
CONTINUE            "continue"
AND                 "and"
NOT                 "not"
OR                  "or"
LOCAL               "local"
TRUE                "true" 
FALSE               "false"
NIL                 "nil"

/*OPERATORS*/
ASSIGN              "="
PLUS                "+"
MINUS               "-"
MULT                "*"
DIV                 "/"
MOD                 "%"
EQUAL               "=="
NEQUAL              "!=" /*NOT SURE POS LEGETAI*/
PLUS_PLUS           "++"
MINUS_MINUS         "--"
GREATER             ">"
LESS                "<"
GREATER_EQUAL       ">="
LESS_EQUAL          "<="


INTCONST      [0-9]+


REALCONST     [0-9]+\.[0-9]+ 


/*PUNCTUATION*/

LEFT_BRACE    "{"
RIGHT_BRACE   "}"
LEFT_BRACKET  "["
RIGHT_BRACKET "]"
LEFT_PAREN    "("
RIGHT_PAREN   ")"
SEMICOLON     ";"
COMMA         ","
DOUBLE_DOT    ".."
DOT           "."
NAMESPACE     "::"
COLON         ":"
LINE_COMMENT  "//".*
START_BLOCK   "/*"


IDENT         [a-zA-Z0-9][a-zA-Z0-9_]*

COMMENT_LINE        "//".*
COMMENT_START       "/*"
COMMENT_END         "*/"


/* EDW OI KANONES GIA KATHE ENA */ 

/* PROXEIRA DOKIMES / OXI SWSTA */


%%
\n             { mylineno++; }


[0-9]+      { cout << "INTCONST" << yytext << endl; }
[a-zA-Z]+   { cout << "IDENT" << yytext << endl; }

{COMMENT_LINE} { /* Ignore // comments */ }

%%





int main(int argc, char** argv) {

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            std::cerr << "Cannot open file: " << argv[1] << std::endl;
            return 1;
        }
    } else {
        yyin = stdin;
    }

    yylex(); 

    return 0;
}
