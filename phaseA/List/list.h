#include <iostream>
#include <cstring>


using namespace std;

typedef struct alpha_token_t{

    unsigned int numline;
    unsigned int numToken;
    char * content;
    char * type;
    char * arrow ;
    struct alpha_token_t * next;

}list_token_t;

list_token_t * insert_token(list_token_t * head ,int nline,int nToken,const char * content,const char * type);

void print(list_token_t *head) ;

void print_token(list_token_t * node);
