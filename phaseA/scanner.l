%{
#include "List/list.h"
#include "Stack/stack.h"
#define YY_DECL int yylex()
int mylineno = 1;  // keep track of lines

list_token_t *head_token = new list_token_t;
comments_t *head_stack = new comments_t;

enum TokenTypes {
    KEYWORD, OPERATOR, CONSTANT, IDENTIFIER, PUNCTUATION
};

enum EscapeChar {
    NEWLINE = '\n',
    BACKSLASH = '\\',
    DOUBLE_QUOTE = '"',
    TAB = '\t',
    VERTICAL_TAB = '\v',
    CARRIAGE_RETURN = '\r'
};

void printToken(const string& text, const string& type, const string& name) {
    cout << yylineno << ": #" << mylineno++ << "  \"" << text << "\"\t" << type << "  " << name << "  <-enumerated" << endl;}
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
NEQUAL              "!=" 
PLUS_PLUS           "++"
MINUS_MINUS         "--"
GREATER             ">"
LESS                "<"
GREATER_EQUAL       ">="
LESS_EQUAL          "<="

INTCONST      [0-9]+
REALCONST     [0-9]+\.[0-9]+ 
IDENT         [a-zA-Z0-9][a-zA-Z0-9_]*

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
COMMENT_START       "/*"
COMMENT_END         "*/"
WHITESPACE         [ \t\n\r]+              


%%


\n                      {}
"if"                    { head_token = insert_token(head_token,yylineno,mylineno++,yytext,"KEYWORD"); }
"else"                  { printToken(yytext, "KEYWORD", "ELSE"); }
"while"                 { printToken(yytext, "KEYWORD", "WHILE"); }
"for"                   { printToken(yytext, "KEYWORD", "FOR"); }
"function"              { printToken(yytext, "KEYWORD", "FUNCTION"); }
"return"                { printToken(yytext, "KEYWORD", "RETURN"); }
"break"                 { printToken(yytext, "KEYWORD", "BREAK"); }
"continue"              { printToken(yytext, "KEYWORD", "CONTINUE"); }
"and"                   { printToken(yytext, "KEYWORD", "AND"); }
"not"                   { printToken(yytext, "KEYWORD", "NOT"); }
"or"                    { printToken(yytext, "KEYWORD", "OR"); }
"local"                 { printToken(yytext, "KEYWORD", "LOCAL"); }
"true"                  { printToken(yytext, "KEYWORD", "TRUE"); }
"false"                 { printToken(yytext, "KEYWORD", "FALSE"); }
"nil"                   { printToken(yytext, "KEYWORD", "NIL"); }



{INTCONST}          {cout << yylineno << ": #" << mylineno++ << "\t\"" << yytext << "\"\tINTCONST " << yytext <<" <-int"<< endl;} 
{REALCONST}          {cout << yylineno << ": #" << mylineno++ << "\t\"" << yytext << "\"\tREALCONST " << yytext <<" <-int"<< endl;}
{IDENT}             {cout << yylineno << ": #" << mylineno++ << "\t \"" << yytext << "\"\tIDENT \t\"" << yytext <<"\" <-char*"<< endl;}

"="                     { printToken(yytext, "OPERATOR", "ASSIGN"); }
"+"                     { printToken(yytext, "OPERATOR", "PLUS"); }
"-"                     { printToken(yytext, "OPERATOR", "MINUS"); }
"*"                     { printToken(yytext, "OPERATOR", "MULT"); }
"/"                     { printToken(yytext, "OPERATOR", "DIV"); }
"%"                     { printToken(yytext, "OPERATOR", "MOD"); }
"=="                    { printToken(yytext, "OPERATOR", "EQUAL"); }
"!="                    { printToken(yytext, "OPERATOR", "NEQUAL"); }
"++"                    { printToken(yytext, "OPERATOR", "PLUS_PLUS"); }
"--"                    { printToken(yytext, "OPERATOR", "MINUS_MINUS"); }
">"                     { printToken(yytext, "OPERATOR", "GREATER"); }
"<"                     { printToken(yytext, "OPERATOR", "LESS"); }
">="                    { printToken(yytext, "OPERATOR", "GREATER_EQUAL"); }
"<="                    { printToken(yytext, "OPERATOR", "LESS_EQUAL"); }

"{"                     { printToken(yytext, "PUNCTUATION", "LEFT_BRACE"); }
"}"                     { printToken(yytext, "PUNCTUATION", "RIGHT_BRACE"); }
"["                     { printToken(yytext, "PUNCTUATION", "LEFT_BRACKET"); }
"]"                     { printToken(yytext, "PUNCTUATION", "RIGHT_BRACKET"); }
"("                     { printToken(yytext, "PUNCTUATION", "LEFT_PAREN"); }
")"                     { printToken(yytext, "PUNCTUATION", "RIGHT_PAREN"); }
";"                     { printToken(yytext, "PUNCTUATION", "SEMICOLON"); }
","                     { printToken(yytext, "PUNCTUATION", "COMMA"); }
".."                    { printToken(yytext, "PUNCTUATION", "DOUBLE_DOT"); }
"."                     { printToken(yytext, "PUNCTUATION", "DOT"); }
"::"                    { printToken(yytext, "PUNCTUATION", "NAMESPACE"); }
":"                     { printToken(yytext, "PUNCTUATION", "COLON"); }

{LINE_COMMENT}  { 
                    if(is_empty(head_stack))
                    {
                        insert_token(head_token, yylineno, ++numToken, "","COMMENT", "LINE_COMMENT", "<-enumerated");
                    }
                    //else ignore
                }
{WHITESPACE}            



{START_BLOCK}   {
                    add_element(head_stack, yylineno);
                }

{COMMENT_END}   {
                    int line = pop_element(head);
                    if(line != -1)
                    {

                        if(stack_size(head) == 0)
                        {
                            std::string content = std::to_string(line) + " - " + std::to_string(yylineno);
                            insert_token(head_token, yylineno, ++numToken, content,"COMMENT", "BLOCK_COMMENT", "<-enumerated");
                        }else{
                            // NESTED_COMMENT
                            insert_token(head_token,yylineno, ++numToken, "","COMMENT", "NESTED_COMMENT", "<-enumerated");
                        }
                    }else{
                        // Duo tokens:
                        // (1) "*" --> MULT
                        insert_token(head_token,yylineno, ++numToken, "*", "OPERATOR  MULT", "<-enumerated");
                        // (2) "/" --> DIV
                        insert_token(head_token,yylineno, ++numToken, "/", "OPERATOR  DIV", "<-enumerated");

                    }
                }
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
    print(head_token);

    return 0;
}
