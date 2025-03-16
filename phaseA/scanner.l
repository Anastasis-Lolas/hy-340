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
COMMENT_START       "/*"
COMMENT_END         "*/"
STRING              \"(\\.|[^"\\])*\"  
WHITESPACE          [ \t\n\r\v]+              


%%


\n                            {}
{IF}                          { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "IF", "enumerated"); }
{ELSE}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "ELSE", "enumerated"); }
{WHILE}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "WHILE", "enumerated"); }
{FOR}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FOR", "enumerated"); }
{FUNCTION}                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FUNCTION", "enumerated"); }
{RETURN}                      { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "RETURN", "enumerated"); }
{BREAK}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "BREAK", "enumerated"); }
{CONTINUE}                    { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "CONTINUE", "enumerated"); }
{AND}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "AND", "enumerated"); }
{NOT}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "NOT", "enumerated"); }
{OR}                          { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "OR", "enumerated"); }
{LOCAL}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "LOCAL", "enumerated"); }
{TRUE}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "TRUE", "enumerated"); }
{FALSE}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "FALSE", "enumerated"); }
{NIL}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "KEYWORD", "NIL", "enumerated"); }




{INTCONST}                    {ins_token(tokens_list, yylineno, mylineno++, yytext, "CONST", "INT", "int");} 
{REALCONST}                   {ins_token(tokens_list, yylineno, mylineno++, yytext, "CONST", "REAL", "real");}
{IDENT}                       {ins_token(tokens_list, yylineno, mylineno++, yytext, "IDENT", "yytext", "char*");}

{ASSIGN}                      { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "ASSIGN", "enumerated"); } 
{PLUS}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "PLUS", "enumerated"); }
{MINUS}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MINUS", "enumerated"); }
{MULT}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MULT", "enumerated"); }
{DIV}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "DIV", "enumerated"); }
{MOD}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MOD", "enumerated"); }
{EQUAL}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "EQUAL", "enumerated"); }
{NOT_EQUALS}                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "NOT_EQUALS", "enumerated"); }
{PLUS_PLUS}                   { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "PLUS_PLUS", "enumerated"); }
{MINUS_MINUS}                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "MINUS_MINUS", "enumerated"); }
{GREATER}                     { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "GREATER", "enumerated"); }
{LESS}                        { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "LESS", "enumerated"); }
{GREATER_EQUAL}               { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "GREATER_EQUAL", "enumerated"); }
{LESS_EQUAL}                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "OPERATOR", "LESS_EQUAL", "enumerated"); }

{LEFT_BRACE}                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_BRACE", "enumerated"); }
{RIGHT_BRACE}                 { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_BRACE", "enumerated"); }
{LEFT_BRACKET}                { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_BRACKET", "enumerated"); }
{RIGHT_BRACKET}               { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_BRACKET", "enumerated"); }
{LEFT_PARENTHESIS}            { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "LEFT_PARENTHESIS", "enumerated"); }
{RIGHT_PARENTHESIS}           { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "RIGHT_PARENTHESIS", "enumerated"); }
{SEMICOLON}                   { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "SEMICOLON", "enumerated"); }
{COMMA}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "COMMA", "enumerated"); }
{DOUBLE_DOT}                  { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "DOUBLE_DOT", "enumerated"); }
{DOT}                         { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "DOT", "enumerated"); }
{NAMESPACE}                   { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "NAMESPACE", "enumerated"); }
{COLON}                       { ins_token(tokens_list, yylineno, mylineno++, yytext, "PUNCTUATION", "COLON", "enumerated"); }

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
                                    }

                                    \"[^\"]* {
                                        std::cerr << "Error: Unterminated string at line " << yylineno << std::endl;
                                        return -1;  
                                }

{WHITESPACE}                    {}

<<EOF>>                         {return 0;}                       

<*>.                            {
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