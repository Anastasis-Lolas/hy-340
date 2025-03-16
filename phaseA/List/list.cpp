#include "list.h"

void print(vector<list_token_t *> print_tokens) {
    for (auto &token : print_tokens) {
        print_token(token);
    }
    cout << endl;
}

void ins_token(vector<list_token_t *> &tokenList, int nline, int nToken,
               const string content, const string type,
               const string upper_content, const string arrow) {
    list_token_t *new_token = new list_token_t;
    new_token->numline = nline;
    new_token->numToken = nToken;
    new_token->arrow = "<-";
    new_token->arrow += arrow;

    new_token->content = content;

    new_token->upper_content = upper_content;

    new_token->type = type;

    new_token->next = NULL;

    tokenList.push_back(new_token);

    // print_token(new_token);
}
