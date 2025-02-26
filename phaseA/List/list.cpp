#include "list.h"

list_token_t *insert_token(list_token_t *head, int nline, int nToken, const string content, const string type, const string upper_content, const string arrow)
{

    list_token_t *new_token = new list_token_t;
    list_token_t *temp_head = head;

    new_token->numline = nline;
    new_token->numToken = nToken;

    new_token->arrow = "<-";

    // this will change after since int,const,char,float etc are not enumerated
    new_token->arrow += arrow;

    new_token->content = content;

    new_token->upper_content = upper_content;

    new_token->type = type;

    new_token->next = NULL;

    if (temp_head == NULL)
    {
        temp_head = new_token;
        head = temp_head;

        // print_token(head);

        return head;
    }
    else
    {

        while (temp_head->next != NULL)
        {
            temp_head = temp_head->next;
        }

        temp_head->next = new_token;
        // print_token(temp_head);
        return head;
    }
}

void print(list_token_t *head)
{
    list_token_t *itr = head;
    while (itr != nullptr)
    {
        /*print edw*/
        print_token(itr);
        itr = itr->next;
    }
}

void print_token(list_token_t *node)
{
    cout << node->numline << ": #" << node->numToken << " \"" << node->content << "\"  ";

    if (node->type != "IDENT")
    {
        cout << node->type << " " << node->upper_content << " " << node->arrow << endl;
    }
    else
    {
        cout << node->type << " \"" << node->content << "\" " << node->arrow << endl;
    }
}