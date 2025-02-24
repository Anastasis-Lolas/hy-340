#include "list.h"

list_token_t * insert_token(list_token_t * head ,int nline,int nToken,char * content,char * type){

    list_token_t * new_token = new list_token_t;

    new_token->numline = nline;
    new_token->numToken   = nToken;
    new_token->content = content;
    new_token->type = type;
    new_token->next = NULL;
    
    list_token_t * temp_head = head;

    if(temp_head == NULL){
        head = new_token;
        return head;
    }else{
        while(temp_head->next!= NULL){
            temp_head = temp_head->next;
        }

        temp_head->next = new_token;
        return head;
    }
}