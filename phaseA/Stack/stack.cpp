#include "stack.h"
#include <iostream>

void init_node(comments_t **node)
{
    *node = new comments_t;
    if (!node)
    {
        std::cerr << "Memory allocation failed!" << std::endl;
    }
    (*node)->linum = 0;
    (*node)->next = NULL;
}

void add_element(comments_t **head, int linum)
{
    comments_t *new_node;
    init_node(&new_node);
    if (!new_node)
        return;

    new_node->linum = linum;
    new_node->next = *head;
    *head = new_node;
}

// flag
int pop_element(comments_t **head)
{
    if (*head == nullptr)
    {
        std::cerr << "Stack is empty! Cannot pop." << std::endl;
        return -1;
    }

    comments_t *temp = *head;
    int linum = temp->linum;
    *head = (*head)->next;
    delete temp;

    return linum;
}

void print_stack(comments_t *head)
{
    while (head)
    {
        std::cout << "linum: " << head->linum << std::endl;
        head = head->next;
    }
}

void free_stack(comments_t **head)
{
    while (*head)
    {
        comments_t *temp = *head;
        *head = (*head)->next;
        delete temp;
    }
}

int is_empty(comments_t *head)
{
    if (head == nullptr)
    {
        return 1;
    }
    return 0;
}

int stack_size(comments_t *head)
{
    int count = 0;
    while (head)
    {
        count++;
        head = head->next;
    }
    return count;
}
