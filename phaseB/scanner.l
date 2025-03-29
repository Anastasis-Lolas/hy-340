%{
#include "List/list.h"
#include <vector>
#include "parser.hpp"
#include <cstdio>

int mylineno = 1;  // keep track of lines

vector<int> coms;
vector<list_token_t *> tokens_list;

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

void yyerror(const char *s);
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
NOT_EQUALS          "!=" 
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

LEFT_BRACE          "{"
RIGHT_BRACE         "}"
LEFT_BRACKET        "["
RIGHT_BRACKET       "]"
LEFT_PARENTHESIS    "("
RIGHT_PARENTHESIS   ")"
SEMICOLON           ";"
COMMA               ","
DOUBLE_DOT          ".."
DOT                 "."
NAMESPACE           "::"
COLON               ":"
LINE_COMMENT        "//".*
COMMENT_START       "/*"
COMMENT_END         "*/"
STRING              \"(\\.|[^"\\])*\"  
WHITESPACE          [ \t\n\r\v]+              


%%


\n                            {}
{IF}                          { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "IF", "enumerated"); return  IF;}
{ELSE}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "ELSE", "enumerated"); return ELSE; }
{WHILE}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "WHILE", "enumerated"); return WHILE;}
{FOR}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FOR", "enumerated"); return FOR;}
{FUNCTION}                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FUNCTION", "enumerated"); return FUNCTION; }
{RETURN}                      { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "RETURN", "enumerated"); return RETURN; }
{BREAK}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "BREAK", "enumerated"); return BREAK;}
{CONTINUE}                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "CONTINUE", "enumerated"); return CONTINUE;}
{AND}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "AND", "enumerated"); return AND; }
{NOT}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "NOT", "enumerated");  return NOT; }
{OR}                          { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "OR", "enumerated"); return OR ;}
{LOCAL}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "LOCAL", "enumerated"); return LOCAL;}
{TRUE}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "TRUE", "enumerated"); return TRUE; }
{FALSE}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FALSE", "enumerated"); return FALSE;}
{NIL}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "NIL", "enumerated");  return NIL;}




{INTCONST}                    {ins_token(tokens_list, yylineno, mylineno++, yytext, "CONST", "INT", "int"); yylval = atoi(yytext); return INTEGER; } 
{REALCONST}                   {ins_token(tokens_list, yylineno, mylineno++, yytext, "CONST", "REAL", "real"); return REALCONST;}
{IDENT}                       {ins_token(tokens_list, yylineno, mylineno++, yytext, "IDENT", "yytext", "char*"); return IDENT;}

{ASSIGN}                      { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "ASSIGN", "enumerated"); return ASSIGN;} 
{PLUS}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "PLUS", "enumerated"); return PLUS; }
{MINUS}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MINUS", "enumerated"); return MINUS;}
{MULT}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MULT", "enumerated"); return MULT;}
{DIV}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "DIV", "enumerated"); return DIV; }
{MOD}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MOD", "enumerated"); return MOD; }
{EQUAL}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "EQUAL", "enumerated"); return EQUAL;}
{NOT_EQUALS}                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "NOT_EQUALS", "enumerated"); return NOT_EQUALS; }
{PLUS_PLUS}                   { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "PLUS_PLUS", "enumerated"); return PLUS_PLUS;}
{MINUS_MINUS}                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MINUS_MINUS", "enumerated"); return MINUS_MINUS;}
{GREATER}                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "GREATER", "enumerated"); return GREATER; }
{LESS}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "LESS", "enumerated"); return LESS;}
{GREATER_EQUAL}               { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "GREATER_EQUAL", "enumerated"); return GREATER_EQUAL; }
{LESS_EQUAL}                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "LESS_EQUAL", "enumerated");  return LESS_EQUAL;}

{LEFT_BRACE}                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_BRACE", "enumerated"); return LEFT_BRACE; }
{RIGHT_BRACE}                { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_BRACE", "enumerated"); return RIGHT_BRACE; }
{LEFT_BRACKET}               { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_BRACKET", "enumerated"); return LEFT_BRACKET; }
{RIGHT_BRACKET}              { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_BRACKET", "enumerated"); return RIGHT_BRACKET; }
{LEFT_PARENTHESIS}           { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_PARENTHESIS", "enumerated"); return LEFT_PARENTHESIS; }
{RIGHT_PARENTHESIS}          { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_PARENTHESIS", "enumerated"); return RIGHT_PARENTHESIS; }
{SEMICOLON}                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "SEMICOLON", "enumerated"); return SEMICOLON; }
{COMMA}                      { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "COMMA", "enumerated"); return COMMA; }
{DOUBLE_DOT}                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "DOUBLE_DOT", "enumerated"); return DOUBLE_DOT; }
{DOT}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "DOT", "enumerated"); return DOT;}
{NAMESPACE}                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "NAMESPACE", "enumerated"); return NAMESPACE; }
{COLON}                      { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "COLON", "enumerated"); return COLON; }


{COMMENT_START}                 {
                                    coms.push_back(yylineno);
                                    BEGIN(BLOCK_COMMENT);
                                }
<BLOCK_COMMENT>{COMMENT_START}  {
                                    coms.push_back(yylineno);
                                }
<BLOCK_COMMENT>{COMMENT_END}      { 
                                    if(!coms.empty())
                                    {
                                        int line = coms.back();
                                        coms.pop_back();
                                        std::string content = std::to_string(line) + " - " + std::to_string(yylineno);
                                        if(coms.empty())
                                        {
                                            ins_token(tokens_list, yylineno, mylineno++, content, "COMMENT", "BLOCK_COMMENT", "enumerated");
                                            return BLOCK_COMMENT;
                                            BEGIN(INITIAL);
                                        }else{
                                            // NESTED_COMMENT
                                            ins_token(tokens_list, yylineno, mylineno++, content, "COMMENT", "NESTED_COMMENT", "enumerated");
                                            return NESTED_COMMENT;
                                        }
                                    }else{
                                        std::cerr << "???" << std::endl;
                                        return -1;
                                    }
                                }

<BLOCK_COMMENT>.                {}
<BLOCK_COMMENT>\n               {}
<BLOCK_COMMENT><<EOF>>          {
                                    std::string msg = "Comment block is open.";
                                    cout<<msg<<endl;
                                    return -1;
                                }

{LINE_COMMENT}                  {
                                    BEGIN(LINE_COMMENT_COND);
                                }

<LINE_COMMENT_COND>.            {}
<LINE_COMMENT_COND>\n           {
                                    ins_token(tokens_list, yylineno-1, mylineno++, "", "COMMENT", "LINE_COMMENT", "enumerated");
                                    return LINE_COMMENT;
                                    BEGIN(INITIAL);
                                }


{STRING}                        {

                                        std::string str = yytext;
                                        str = str.substr(1, str.length() - 2);

                                    
                                        std::string processedStr;
                                        for (size_t i = 0; i < str.length(); ++i) {
                                            if (str[i] == '\\' && i + 1 < str.length()) {
                                                switch (str[i + 1]) {
                                                    case 'a':  processedStr += '\a'; break;
                                                    case 'n':  processedStr += '\n'; break;
                                                    case 't':  processedStr += '\t'; break;
                                                    case 'r':  processedStr += '\r'; break;
                                                    case 'v':  processedStr += '\v'; break;
                                                    case 'f':  processedStr += '\f'; break;
                                                    case '\\': processedStr += '\\'; break;
                                                    case '\'': processedStr += '\''; break;
                                                    case '"':  processedStr += '\"'; break;
                                                    default:   
                                                        std::cerr << "Warning: Invalid escape sequence \\" << str[i + 1] << " at line " << yylineno << std::endl;
                                                        processedStr += '\\';
                                                        processedStr += str[i + 1];  
                                                        break;
                                                }
                                                ++i;  
                                            } else {
                                                processedStr += str[i];
                                            }
                                        }
                                        ins_token(tokens_list, yylineno, mylineno++, processedStr, "", "STRING", "string");
                                        return STRING;
                                    }

                                    \"[^\"]* {
                                        std::cerr << "Error: Unterminated string at line " << yylineno << std::endl;
                                        return -1;  
                                }

{WHITESPACE}                    {}

<<EOF>>                         {return 0;}                       

<*>.                            {
                                    ins_token(tokens_list, yylineno, mylineno++, yytext, "UNDEFINED", "CHARACTER", "null");
                                    return UNDEFINED;
                                }
%%

