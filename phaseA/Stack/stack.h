
typedef struct comments
{
    int linum;
    struct comments *next;
} comments_t;

void init_node(comments_t **node);

void add_element(comments_t **head, int linum);

int pop_element(comments_t **head);

void print_stack(comments_t *head);

void free_stack(comments_t **head);

int stack_size(comments_t *head);

int is_empty(comments_t *head);