#include "list.h"

list_token_t *insert_token(list_token_t *head, int nline, int nToken,const char *content,const char *type)
{

    list_token_t *new_token = new list_token_t;

    new_token->numline = nline;
    new_token->numToken = nToken;

    // this will change after since int,const,char,float etc are not enumerated 
    new_token->arrow = new char[strlen("<--enumerated") + 1];
    strcpy(new_token->arrow, "<--enumerated"); 

    new_token->content = new char[strlen(content) +1];
    strcpy(new_token->content, content);

    new_token->type = new char[strlen(type) +1];
    strcpy(new_token->type, type);




    new_token->next = NULL;

    list_token_t *temp_head = head;
    

    if (temp_head == NULL)
    {
        head = new_token;
        print_token(head);
        
        return head;
    }else
    {
        while (temp_head->next != NULL)
        {
            temp_head = temp_head->next;
        }

        temp_head->next = new_token;
        print_token(temp_head);
        return head;
    }
}

void print(list_token_t *head)
{
    list_token_t *itr = head;
    while (itr != nullptr)
    {
        /*print edw*/
        cout << itr->content << endl;
        itr = itr->next;
    }
}


void print_token(list_token_t *node){
    cout << node->numline<< ": #" << node->numToken << " " << node->arrow << endl;
}