%{
#include "List/list.h"
#include <vector>
#define YY_DECL int yylex()
int mylineno = 1;  // keep track of lines

list_token_t *head_token = nullptr;
vector<int> coms;

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

%x BLOCK_COMMENT
%x LINE_COMMENT_COND

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
"if"                    {}
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
{IDENT}             {
                        head_token = insert_token(head_token, yylineno, mylineno++, yytext,"IDENT", yytext, "char*");
                    }

"="                     { printToken(yytext, "OPERATOR", "ASSIGN"); }
"+"                     { printToken(yytext, "OPERATOR", "PLUS"); }
"-"                     { printToken(yytext, "OPERATOR", "MINUS"); }
"*"                     { head_token = insert_token(head_token, yylineno, mylineno++, yytext,"OPERATOR", "MULT", "enumerated");}
"/"                     { head_token = insert_token(head_token, yylineno, mylineno++, yytext,"OPERATOR", "DIV", "enumerated");}
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

{WHITESPACE}



"/*"            {
                    coms.push_back(yylineno);
                    BEGIN(BLOCK_COMMENT);
                }
<BLOCK_COMMENT>"/*"     {
                            coms.push_back(yylineno);
                        }
<BLOCK_COMMENT>"*/"     { 
                            if(!coms.empty())
                            {
                                int line = coms.front();
                                coms.erase(coms.begin());
                                std::string content = std::to_string(line) + " - " + std::to_string(yylineno);
                                if(coms.empty())
                                {
                                    head_token = insert_token(head_token, yylineno, mylineno++, content,"COMMENT", "BLOCK_COMMENT", "enumerated");
                                    BEGIN(INITIAL);
                                }else{
                                    // NESTED_COMMENT
                                    head_token = insert_token(head_token,yylineno, mylineno++, content,"COMMENT", "NESTED_COMMENT", "enumerated");
                                }
                            }else{
                                std::cerr << "???" << std::endl;
                            }
                        }

<BLOCK_COMMENT>.      {}
<BLOCK_COMMENT>\n     {}
<BLOCK_COMMENT><<EOF>>  {
                            std::string msg = "Comment block is open.";
                            cout<<msg<<endl; 
                        }

"//"            {
                    BEGIN(LINE_COMMENT_COND);
                }

<LINE_COMMENT_COND>.  {}
<LINE_COMMENT_COND>\n   {
                            head_token = insert_token(head_token, yylineno - 1, mylineno++, "","COMMENT", "LINE_COMMENT", "enumerated");
                            BEGIN(INITIAL);
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
