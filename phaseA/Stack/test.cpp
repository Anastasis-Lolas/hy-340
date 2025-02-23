#include "stack.h"
#include <iostream>

int main()
{
    comments_t *head = nullptr;

    add_element(&head, 10);
    add_element(&head, 20);
    add_element(&head, 30);

    std::cout << "Stack contents:" << std::endl;
    print_stack(head);
    std::cout << "Number of nodes in stack: " << stack_size(head) << std::endl;

    std::cout << "Popped: " << pop_element(&head) << std::endl;
    std::cout << "Stack contents after popping:" << std::endl;
    print_stack(head);
    std::cout << "Number of nodes in stack: " << stack_size(head) << std::endl;

    free_stack(&head);

    return 0;
}
