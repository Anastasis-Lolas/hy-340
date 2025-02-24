#include <iostream>
#include <string>


using namespace std;

typedef struct alpha_token_t{

    unsigned int numline;
    unsigned int numToken;
    char * content;
    char * type;
    struct alpha_token_t * next;

}list_token_t;

list_token_t * insert_token(list_token_t * head ,int nline,int nToken,char * content,char * type);



