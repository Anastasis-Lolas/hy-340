#include "list.h"
#include <iostream>

int main(){


    list_token_t *head = nullptr;

    insert(&head,10);
    insert(&head,20);
    insert(&head,30);

    std::cout<<"List Testing Contents : " << std::endl;
    print_token_list(head);


    free_list(&head);

    print_token_list(head);
}