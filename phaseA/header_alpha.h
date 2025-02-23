#include <iostream>
#include <string>


using namespace std;


struct alpha_token_t {
    unsigned int     numline;
    unsigned int     numToken;
    char          *content;
    char          *type;
    struct alpha_token_t *alpha_yylex;
  
  };
