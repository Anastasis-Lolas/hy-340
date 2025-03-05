%{
#include "List/list.h"
#include <vector>
#define YY_DECL int alpha_yylex(void * ylval)
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
START_BLOCK         "/*"
COMMENT_START       "/*"
COMMENT_END         "*/"
STRING              \"(\\.|[^"\\])*\"  
WHITESPACE          [ \t\n\r\v]+              


%%


\n                      {}
"if"                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "IF", "enumerated"); }
"else"                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "ELSE", "enumerated"); }
"while"                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "WHILE", "enumerated"); }
"for"                   { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FOR", "enumerated"); }
"function"              { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FUNCTION", "enumerated"); }
"return"                { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "RETURN", "enumerated"); }
"break"                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "BREAK", "enumerated"); }
"continue"              { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "CONTINUE", "enumerated"); }
"and"                   { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "AND", "enumerated"); }
"not"                   { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "NOT", "enumerated"); }
"or"                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "OR", "enumerated"); }
"local"                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "LOCAL", "enumerated"); }
"true"                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "TRUE", "enumerated"); }
"false"                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FALSE", "enumerated"); }
"nil"                   { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "NIL", "enumerated"); }




{INTCONST}              {ins_token(tokens_list, yylineno, mylineno++, yytext, "CONST", "INT", "int");} 
{REALCONST}             {ins_token(tokens_list, yylineno, mylineno++, yytext, "CONST", "REAL", "real");}
{IDENT}                 {ins_token(tokens_list, yylineno, mylineno++, yytext, "IDENT", "yytext", "char*");}

"="                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "ASSIGN", "enumerated"); } 
"+"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "PLUS", "enumerated"); }
"-"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MINUS", "enumerated"); }
"*"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MULT", "enumerated"); }
"/"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "DIV", "enumerated"); }
"%"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MOD", "enumerated"); }
"=="                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "EQUAL", "enumerated"); }
"!="                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "NOT_EQUALS", "enumerated"); }
"++"                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "PLUS_PLUS", "enumerated"); }
"--"                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MINUS_MINUS", "enumerated"); }
">"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "GREATER", "enumerated"); }
"<"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "LESS", "enumerated"); }
">="                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "GREATER_EQUAL", "enumerated"); }
"<="                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "LESS_EQUAL", "enumerated"); }

"{"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_BRACE", "enumerated"); }
"}"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_BRACE", "enumerated"); }
"["                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_BRACKET", "enumerated"); }
"]"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_BRACKET", "enumerated"); }
"("                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_PARENTHESIS", "enumerated"); }
")"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_PARENTHESIS", "enumerated"); }
";"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "SEMICOLON", "enumerated"); }
","                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "COMMA", "enumerated"); }
".."                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "DOUBLE_DOT", "enumerated"); }
"."                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "DOT", "enumerated"); }
"::"                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "NAMESPACE", "enumerated"); }
":"                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "COLON", "enumerated"); }

"/*"                    {
                            coms.push_back(yylineno);
                            BEGIN(BLOCK_COMMENT);
                        }
<BLOCK_COMMENT>"/*"     {
                            coms.push_back(yylineno);
                        }
<BLOCK_COMMENT>"*/"     { 
                            if(!coms.empty())
                            {
                                int line = coms.back();
                                coms.pop_back();
                                std::string content = std::to_string(line) + " - " + std::to_string(yylineno);
                                if(coms.empty())
                                {
                                    ins_token(tokens_list, yylineno, mylineno++, content, "COMMENT", "BLOCK_COMMENT", "enumerated");
                                    BEGIN(INITIAL);
                                }else{
                                    // NESTED_COMMENT
                                    ins_token(tokens_list, yylineno, mylineno++, content, "COMMENT", "NESTED_COMMENT", "enumerated");
                                }
                            }else{
                                std::cerr << "???" << std::endl;
                                return -1;
                            }
                        }

<BLOCK_COMMENT>.        {}
<BLOCK_COMMENT>\n       {}
<BLOCK_COMMENT><<EOF>>  {
                            std::string msg = "Comment block is open.";
                            cout<<msg<<endl;
                            return -1;
                        }

"//"                    {
                            BEGIN(LINE_COMMENT_COND);
                        }

<LINE_COMMENT_COND>.    {}
<LINE_COMMENT_COND>\n   {
                            ins_token(tokens_list, yylineno-1, mylineno++, "", "COMMENT", "LINE_COMMENT", "enumerated");
                            BEGIN(INITIAL);
                        }


{STRING} {

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
}

\"[^\"]* {
    std::cerr << "Error: Unterminated string at line " << yylineno << std::endl;
    return -1;  
}

{WHITESPACE}            {}

<<EOF>>                 {return 0;}                       

<*>.                    {
                            ins_token(tokens_list, yylineno, mylineno++, yytext, "UNDEFINED", "CHARACTER", "null");
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
    
    std::cout<< "------------------- Lexical Analysis --------------------"<<endl;
    alpha_yylex(nullptr); 
    print(tokens_list);

    return 0;
}
