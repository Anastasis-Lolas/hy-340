#include <cstring>
#include <iostream>
#include <vector>

using namespace std;


typedef struct alpha_token_t {
    unsigned int numline;
    unsigned int numToken;
    string content;
    string type;
    string upper_content;
    string arrow;
    struct alpha_token_t *next;
} list_token_t;


void ins_token(vector<list_token_t *> &tokenList, int nline, int nToken,
               const string content, const string type,
               const string upper_content, const string arrow);

list_token_t *insert_token(list_token_t *head, int nline, int nToken,
                           const string content, const string type,
                           const string upper_content, const string arrow);

void print(vector<list_token_t *> print_tokes);

void print_token(list_token_t *node);
